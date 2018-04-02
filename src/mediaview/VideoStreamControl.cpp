//
// Created by jianghualuo on 18-3-17.
//

#include "VideoStreamControl.h"
#include <chrono>
#include <thread>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <libavutil/error.h>
#include <glog/logging.h>
#include <QMessageBox>
#include <QTimer>

static boost::mutex mtxStreamReady;
static boost::condition_variable cvStreamReady;
int64_t lastFrameTime = -1;
bool getFrameTimeout = false;
//bool VideoStreamControl::streamReady = false;
VideoStreamControl::VideoStreamControl(QObject *parent) :
        QObject(parent),
        url(""),
        videoStreamIndex(-1),
        pAVFormatContext(nullptr),
        pSwsContext(nullptr),
        pAVCodecContext(nullptr),
        streamReady(false),
        readStreamThread(new QThread(this))
{
//    av_log_set_level(AV_LOG_SKIP_REPEATED);
    av_register_all();//注册库中所有可用的文件格式和解码器
    avformat_network_init();
    pAVFrame = av_frame_alloc();//仅分配AVFrame，而不分配buffer
    av_init_packet(&pAVPacket);
    avInputFormat = av_find_input_format("rtsp");
    moveToThread(readStreamThread);
    readStreamThread->start(QThread::HighestPriority);
}

VideoStreamControl::~VideoStreamControl()
{
    avformat_close_input(&pAVFormatContext);
    sws_freeContext(pSwsContext);
}

//can only be called in _readStream()
bool VideoStreamControl::init()
{
//    boost::unique_lock<boost::mutex> lock(mtxStreamReady);

    LOG(INFO) << "VideoStreamControl::init url:" << url;
    LOG(INFO) << "VideoStreamControl::init start ###########################";

    LOG(INFO) << "0.free pAVFormatContext ###########################";
    if (nullptr != pAVFormatContext) {
        avformat_close_input(&pAVFormatContext);
        pAVFormatContext = nullptr;
        sws_freeContext(pSwsContext);
        pSwsContext = nullptr;
    }
    lastFrameTime = -1;
    getFrameTimeout = false;

    LOG(INFO) << "1.avformat_open_input ###########################";
//    int result = avformat_open_input(&pAVFormatContext, url.c_str(), nullptr, nullptr);
    AVDictionary *options = nullptr;
    av_dict_set(&options, "buffer_size", "102400", 0);
//    av_dict_set(&options, "stimeout", "1000000", 0); //1s无响应则超时
//    av_dict_set(&options, "timeout", "1", 0);
    pAVFormatContext = avformat_alloc_context();
//    pAVFormatContext->interrupt_callback.opaque = this;
//    pAVFormatContext->interrupt_callback.callback = readFrameTimeout;
    int result = avformat_open_input(&pAVFormatContext, url.c_str(), avInputFormat, &options);
    if (result < 0) {
        goto error;
    }

    //优化avformat_find_stream_info速度
    //数据包不入缓冲区,avformat_find_stream_info接口内部读取的每一帧数据只用于分析，不显示,会出现进入页面立马卡顿一下的问题
//    pAVFormatContext->flags |= AVFMT_FLAG_NOBUFFER;
    //限制avformat_find_stream_info接口内部读取的最大数据量,有些情况时，会导致这些数据不足以分析这个流的信息
    pAVFormatContext->probesize = 1024;

    //获取视频流信息
    LOG(INFO) << "2.avformat_find_stream_info ###########################";
    result = avformat_find_stream_info(pAVFormatContext, nullptr);
    if (result < 0) {
        goto error;
    }

    //获取视频流索引
    LOG(INFO) << "3.get videoStreamIndex ###########################";
    videoStreamIndex = -1;
    for (uint i = 0; i < pAVFormatContext->nb_streams; i++) {
        if (pAVFormatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStreamIndex = i;
            break;
        }
    }

    if (videoStreamIndex == -1){
        goto error;
    }

    //获取视频流的分辨率大小
    pAVCodecContext = pAVFormatContext->streams[videoStreamIndex]->codec;
    videoWidth = pAVCodecContext->width;
    videoHeight = pAVCodecContext->height;

    LOG(INFO) << "4.avpicture_alloc ###########################";
    avpicture_alloc(&pAVPicture, AV_PIX_FMT_RGB24, videoWidth, videoHeight);

    AVCodec *pAVCodec;

    //获取视频流解码器
    LOG(INFO) << "5.avcodec_find_decoder ###########################";
    pAVCodec = avcodec_find_decoder(pAVCodecContext->codec_id);
    pSwsContext = sws_getContext(videoWidth, videoHeight, AV_PIX_FMT_YUVJ420P, videoWidth, videoHeight, AV_PIX_FMT_RGB24, SWS_BICUBIC, 0, 0, 0);

    //打开对应解码器
    LOG(INFO) << "6.avcodec_open2 ###########################";
    result = avcodec_open2(pAVCodecContext, pAVCodec, nullptr);
    if (result < 0) {
        goto error;
    }

    LOG(INFO) << "init video stream success!!!!";

    streamReady = true;
//    cvStreamReady.notify_all();
    return true;

    error:
    streamReady = false;
    cvStreamReady.notify_all();
    char errorStr[1024] = {0};
    av_strerror(result, errorStr, 1024);
    LOG(INFO) << "VideoStreamControl::init got an error:" << result << " :" << errorStr;
    return false;
}

bool VideoStreamControl::readStream()
{
    LOG(INFO) << "VideoStreamControl::readStream read image stream";
    QTimer::singleShot(0, this, SLOT(_readStream()));
    return true;
}

void VideoStreamControl::_readStream()
{
    LOG(INFO) << "VideoStreamControl::_readStream start";
    int i = 100; //如果无视频流，重复连接20次
    {
        boost::unique_lock<boost::mutex> lock(mtxStreamReady);
        streamReady = false;
        while (i && !init()) {
            --i;
            boost::this_thread::sleep_for(boost::chrono::milliseconds(10));
        }
        cvStreamReady.notify_all();
    }
    emit readStreamDone(i);
}

void VideoStreamControl::play() {
    readStream();

    //一帧一帧读取视频
    int frameFinished = 0;
    int readFrameResult = 0;
    int lastPts = 0;
    while (true) {
        boost::unique_lock<boost::mutex> lock(mtxStreamReady);
        while (!streamReady) {
            LOG(INFO) << "7.streamReady waite befor###########################";
            cvStreamReady.wait(lock);
            LOG(INFO) << "7.streamReady waite after###########################";
        }

        boost::this_thread::interruption_point();
        if ((readFrameResult = av_read_frame(pAVFormatContext, &pAVPacket)) == 0) {
            if (lastPts == pAVPacket.pts) {
                // 时间戳没变，说明已经暂停，不进行解码
                continue;
            }
            lastPts = pAVPacket.pts;
            avcodec_decode_video2(pAVCodecContext, pAVFrame, &frameFinished, &pAVPacket);
            lastFrameTime = av_gettime();
            if (frameFinished) {
//                LOG(INFO) << "VideoStreamControl::play after avcodec_decode_video2";
                sws_scale(pSwsContext, (const uint8_t *const *) pAVFrame->data, pAVFrame->linesize, 0, videoHeight,
                          pAVPicture.data, pAVPicture.linesize);
//                LOG(INFO) << "VideoStreamControl::play after sws_scale";
                QImage image(pAVPicture.data[0], videoWidth, videoHeight, QImage::Format_RGB888);
                double time = pAVPacket.pts * av_q2d(pAVFormatContext->streams[videoStreamIndex]->time_base); //秒
                lock.unlock();
                emit imageGot(image); //发送获取一帧图像信号
                emit videoTimestampChanged(time);
                boost::this_thread::sleep_for(boost::chrono::milliseconds(5));
                lock.lock();
            }
            av_free_packet(&pAVPacket);//释放资源,否则内存会一直上升
            av_init_packet(&pAVPacket);
        } else if (readFrameResult == AVERROR_EOF) {
            LOG(INFO) << "VideoStreamControl::play video finished";
            streamReady = false;
            emit videoFinished();
            QImage image;
            emit imageGot(image);
        } else {
            char errorStr[1024] = {0};
            av_strerror(readFrameResult, errorStr, 1024);
            LOG(INFO) << "VideoStreamControl::play got an error:" << readFrameResult << " :" << errorStr;
        }
    }
}

void VideoStreamControl::seek(int seconds) {
}

void VideoStreamControl::closeReadStreamThread() {
    readStreamThread->quit();
    readStreamThread->wait();
}

int VideoStreamControl::readFrameTimeout(void *ctx) {
    if (lastFrameTime != -1) {
        int duration = (av_gettime() - lastFrameTime) / 1000000.0;
        if (duration > 5) {
            LOG(INFO) << "VideoStreamControl::readFrameTimeout get frame timeout";
            getFrameTimeout = true;
            return 1;
        }
    }
    return 0;
}
