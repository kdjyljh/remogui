#include "imagestreamproc.h"
#include <QDateTime>
#include <QDebug>

ImageStreamProc::ImageStreamProc(QObject *parent) :
    QObject(parent)
{
    videoStreamIndex = -1;
    av_register_all();//注册库中所有可用的文件格式和解码器
    avformat_network_init();//初始化网络流格式,使用RTSP网络流时必须先执行
    pAVFormatContext = avformat_alloc_context();//申请一个AVFormatContext结构的内存,并进行简单初始化
    pAVFrame = av_frame_alloc();//仅分配AVFrame，而不分配buffer
}

ImageStreamProc::~ImageStreamProc()
{
    avformat_free_context(pAVFormatContext);
    av_frame_free(&pAVFrame);
    sws_freeContext(pSwsContext);
}

bool ImageStreamProc::init()
{
    //打开视频流
    url = "rtsp://192.168.1.10/livestream/12";
    int result = avformat_open_input(&pAVFormatContext, url.toStdString().c_str(),NULL,NULL);
    if (result < 0){
        qDebug() << "open video stream failed!!!!!!";
        return false;
    }

    //获取视频流信息
    result = avformat_find_stream_info(pAVFormatContext,NULL);
    if (result < 0){
        qDebug() << "get video stream information failed!!!!!";
        return false;
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
        return false;
    }

    //获取视频流的分辨率大小
    pAVCodecContext = pAVFormatContext->streams[videoStreamIndex]->codec;
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
        return false;
    }

    qDebug()<<"init video stream success!!!!";
    return true;
}

void ImageStreamProc::play()
{

    if (!init()) {
        return;
    }
    //一帧一帧读取视频
    int frameFinished = 0;
    while (true){
        if (av_read_frame(pAVFormatContext, &pAVPacket) >= 0){
            if (pAVPacket.stream_index == videoStreamIndex){
                qDebug()<< "##############decode begin##############" <<QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
                avcodec_decode_video2(pAVCodecContext, pAVFrame, &frameFinished, &pAVPacket);
                if (frameFinished){
                    mutex.lock();
                    sws_scale(pSwsContext, (const uint8_t* const *)pAVFrame->data, pAVFrame->linesize, 0, videoHeight, pAVPicture.data, pAVPicture.linesize);
                    //发送获取一帧图像信号
                    QImage image(pAVPicture.data[0], videoWidth, videoHeight, QImage::Format_RGB888);
                    emit imageGot(image);
                    mutex.unlock();
                }
            }
        }
        av_free_packet(&pAVPacket);//释放资源,否则内存会一直上升
    }
}
