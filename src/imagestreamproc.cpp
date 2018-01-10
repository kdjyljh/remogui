#include "imagestreamproc.h"
#include "../thirdparty/commlog.h"
#include <chrono>
#include <thread>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <QDateTime>
#include <QDebug>
#include <libavutil/error.h>

static boost::mutex mtxStreamReady;
static boost::condition_variable cvStreamReady;

bool ImageStreamProc::streamReady = false;
ImageStreamProc::ImageStreamProc(QObject *parent) :
    QObject(parent),
    url("rtsp://192.168.1.10/livestream/12"),
    videoStreamIndex(-1),
    pAVFormatContext(nullptr),
    pSwsContext(nullptr),
    pAVCodecContext(nullptr)
{
    av_register_all();//注册库中所有可用的文件格式和解码器
    avformat_network_init();
    pAVFrame = av_frame_alloc();//仅分配AVFrame，而不分配buffer
}

ImageStreamProc::~ImageStreamProc()
{
    avformat_close_input(&pAVFormatContext);
    avformat_free_context(pAVFormatContext);
    avcodec_free_context(&pAVCodecContext);
    av_frame_free(&pAVFrame);
    sws_freeContext(pSwsContext);
}

bool ImageStreamProc::init()
{
    LOG(INFO) << "THE THREAD ID OF INIT";
    boost::unique_lock<boost::mutex> lock(mtxStreamReady);
    LOG(INFO) << "ImageStreamProc::init get lock";


    //    avformat_close_input(&pAVFormatContext);
    //    avcodec_free_context(&pAVCodecContext);
    //    avio_close(pAVFormatContext->pb);
    if (nullptr != pAVFormatContext)
//        av_read_pause(pAVFormatContext);
    avformat_close_input(&pAVFormatContext);
    avformat_free_context(pAVFormatContext);
//    sws_freeContext(pSwsContext);

    pAVFormatContext = avformat_alloc_context();//申请一个AVFormatContext结构的内存,并进行简单初始化


    avformat_network_deinit();
    avformat_network_init();//初始化网络流格式,使用RTSP网络流时必须先执行

    //打开视频流
    int result = avformat_open_input(&pAVFormatContext, "rtsp://192.168.1.10/livestream/12", NULL, NULL);
    if (result != 0) {
        qDebug() << "open video stream failed!!!!!!";
        goto error;
    }

    //获取视频流信息
    result = avformat_find_stream_info(pAVFormatContext,NULL);
    if (result < 0){
        qDebug() << "get video stream information failed!!!!!";
        goto error;
    }

    //获取视频流索引
    videoStreamIndex = -1;
    for (uint i = 0; i < pAVFormatContext->nb_streams; i++) {
        if (pAVFormatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStreamIndex = i;
            break;
        }
    }

    if (videoStreamIndex==-1){
        qDebug() << "get video stream index failed!!!!!";
        goto error;
    }

    //获取视频流的分辨率大小
    pAVCodecContext = pAVFormatContext->streams[videoStreamIndex]->codec;
    //    avcodec_copy_context(pAVCodecContext, pAVFormatContext->streams[videoStreamIndex]->codec);
    videoWidth = pAVCodecContext->width;
    videoHeight = pAVCodecContext->height;

    avpicture_alloc(&pAVPicture,AV_PIX_FMT_RGB24,videoWidth,videoHeight);

    AVCodec *pAVCodec;

    //获取视频流解码器
    pAVCodec = avcodec_find_decoder(pAVCodecContext->codec_id);
    pSwsContext = sws_getContext(videoWidth, videoHeight, AV_PIX_FMT_YUV420P, videoWidth, videoHeight, AV_PIX_FMT_RGB24, SWS_BICUBIC, 0, 0, 0);

    //打开对应解码器
    result = avcodec_open2(pAVCodecContext, pAVCodec, NULL);
    if (result<0){
        qDebug()<<"open decoder failed!!!!!";
        goto error;
    }

    qDebug()<<"init video stream success!!!!";

    streamReady = true;
    cvStreamReady.notify_all();
    LOG(INFO) << "ImageStreamProc::init lock relase streamReady = " << streamReady;

    av_read_frame(pAVFormatContext, &pAVPacket);

    return true;

error:
    streamReady = false;
    cvStreamReady.notify_all();
    char errorStr[1024] = {0};
    av_strerror(result, errorStr, 1024);
    LOG(INFO) << "ImageStreamProc::init got an error:" << result << " :" << errorStr;
    LOG(INFO) << "ImageStreamProc::init lock relase streamReady = " << streamReady;
    return false;
}

bool ImageStreamProc::readStream_1S()
{
    LOG(INFO) << "ImageStreamProc::readStream_1S read image stream again after 1s";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return init();
}

void ImageStreamProc::play()
{
//    if (!init()) {
//        return;
//    }

    init();

    //一帧一帧读取视频
    int frameFinished = 0;
//    while (true) {
        boost::unique_lock<boost::mutex> lock(mtxStreamReady);
//                    LOG(INFO) << "ImageStreamProc::play lock got";
        while (!streamReady) {
            cvStreamReady.wait(lock);
//                    LOG(INFO) << "ImageStreamProc::play lock release";
        av_read_frame(pAVFormatContext, &pAVPacket);

        if (av_read_frame(pAVFormatContext, &pAVPacket) >= 0){
            if (pAVPacket.stream_index == videoStreamIndex){
//                                qDebug()<< "##############decode begin##############" <<QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
                avcodec_decode_video2(pAVCodecContext, pAVFrame, &frameFinished, &pAVPacket);
                if (frameFinished){
                    sws_scale(pSwsContext, (const uint8_t* const *)pAVFrame->data, pAVFrame->linesize, 0, videoHeight, pAVPicture.data, pAVPicture.linesize);
                    //发送获取一帧图像信号
                    QImage image(pAVPicture.data[0], videoWidth, videoHeight, QImage::Format_RGB888);
                    emit imageGot(image);
                }
            }
        }
        av_free_packet(&pAVPacket);//释放资源,否则内存会一直上升
    }
}
