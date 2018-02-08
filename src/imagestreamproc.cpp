#include "imagestreamproc.h"
#include "mainwindow.h"
#include "../thirdparty/commlog.h"
#include <chrono>
#include <thread>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread.hpp>
#include <libavutil/error.h>
#include <glog/logging.h>
#include <QMessageBox>
#include <QTimer>

static boost::mutex mtxStreamReady;
static boost::condition_variable cvStreamReady;
//boost::condition_variable_any cvStreamReady;

//rtsp://184.72.239.149/vod/mp4://BigBuckBunny_175k.mov
//rtsp://192.168.0.1/livestream/12
bool ImageStreamProc::streamReady = false;
ImageStreamProc::ImageStreamProc(QObject *parent) :
    QObject(parent),
    url("rtsp://192.168.0.1/livestream/12"),
    videoStreamIndex(-1),
    pAVFormatContext(nullptr),
    pSwsContext(nullptr),
    pAVCodecContext(nullptr),
    readStreamThread(new QThread)
{
    av_log_set_level(AV_LOG_SKIP_REPEATED);
    av_register_all();//注册库中所有可用的文件格式和解码器
    avformat_network_init();
    pAVFrame = av_frame_alloc();//仅分配AVFrame，而不分配buffer
    av_init_packet(&pAVPacket);
    avInputFormat = av_find_input_format("rtsp");
    moveToThread(readStreamThread);
    readStreamThread->start(QThread::HighestPriority);
}

ImageStreamProc::~ImageStreamProc()
{
    avformat_close_input(&pAVFormatContext);
    avformat_free_context(pAVFormatContext);
    avcodec_free_context(&pAVCodecContext);
    sws_freeContext(pSwsContext);
    delete readStreamThread;
}

//can only be called in _readStream()
bool ImageStreamProc::init()
{
//    boost::unique_lock<boost::mutex> lock(mtxStreamReady);

    LOG(INFO) << "ImageStreamProc::init start ###########################";

    LOG(INFO) << "0.free pAVFormatContext ###########################";
    if (nullptr != pAVFormatContext) {
        avformat_close_input(&pAVFormatContext);
        avformat_free_context(pAVFormatContext);
        pAVFormatContext = nullptr;
    }

    LOG(INFO) << "1.avformat_open_input ###########################";
//    int result = avformat_open_input(&pAVFormatContext, url.c_str(), nullptr, nullptr);
    int result = avformat_open_input(&pAVFormatContext, url.c_str(), avInputFormat, nullptr);
    if (result < 0) {
        goto error;
    }

    //优化avformat_find_stream_info速度
    //数据包不入缓冲区,avformat_find_stream_info接口内部读取的每一帧数据只用于分析，不显示,会出现进入页面立马卡顿一下的问题
//    pAVFormatContext->flags |= AVFMT_FLAG_NOBUFFER;
    //限制avformat_find_stream_info接口内部读取的最大数据量,有些情况时，会导致这些数据不足以分析这个流的信息
    pAVFormatContext->probesize = 4096;

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
    pSwsContext = sws_getContext(videoWidth, videoHeight, AV_PIX_FMT_YUV420P, videoWidth, videoHeight, AV_PIX_FMT_RGB24, SWS_BICUBIC, 0, 0, 0);

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
//    cvStreamReady.notify_all();
    char errorStr[1024] = {0};
    av_strerror(result, errorStr, 1024);
    LOG(INFO) << "ImageStreamProc::init got an error:" << result << " :" << errorStr;
    return false;
}

bool ImageStreamProc::readStream()
{
    LOG(INFO) << "ImageStreamProc::readStream read image stream";
    QTimer::singleShot(0, this, SLOT(_readStream()));
    return true;
}

void ImageStreamProc::_readStream()
{
    LOG(INFO) << "ImageStreamProc::_readStream start";
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

//    LOG(INFO) << "ImageStreamProc::_readStream try " << 20 - i + 1;

    emit readStreamDone(i);
    if (!i) {
        QMessageBox::warning(nullptr, "网络错误", "无视频流", QMessageBox::Ok);
    }
}

void ImageStreamProc::play()
{
    readStream();

    //一帧一帧读取视频
    int frameFinished = 0;
    while (true) {
        boost::unique_lock<boost::mutex> lock(mtxStreamReady);
        while (!streamReady) {
            LOG(INFO) << "7.streamReady waite befor###########################";
            cvStreamReady.wait(lock);
            LOG(INFO) << "7.streamReady waite after###########################";
        }
        if (av_read_frame(pAVFormatContext, &pAVPacket) >= 0){
            avcodec_decode_video2(pAVCodecContext, pAVFrame, &frameFinished, &pAVPacket);
            if (frameFinished) {
                sws_scale(pSwsContext, (const uint8_t* const *)pAVFrame->data, pAVFrame->linesize, 0, videoHeight, pAVPicture.data, pAVPicture.linesize);                //发送获取一帧图像信号
                QImage image(pAVPicture.data[0], videoWidth, videoHeight, QImage::Format_RGB888);
                mtxStreamReady.unlock();
                emit imageGot(image);
                boost::this_thread::sleep_for(boost::chrono::milliseconds(5));
                mtxStreamReady.lock();
            }
        }
        av_free_packet(&pAVPacket);//释放资源,否则内存会一直上升
        av_init_packet(&pAVPacket);
    }
}
