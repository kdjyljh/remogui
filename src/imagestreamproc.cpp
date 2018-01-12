#include "imagestreamproc.h"
#include "../thirdparty/commlog.h"
#include <chrono>
#include <thread>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
//#include <QDateTime>
//#include <QDebug>
#include <libavutil/error.h>

static boost::mutex mtxStreamReady;
static boost::condition_variable cvStreamReady;

AVFormatContext* oc = nullptr;

//rtsp://192.168.1.10/livestream/12
bool ImageStreamProc::streamReady = false;

static const std::string urllocal("rtsp://184.72.239.149/vod/mp4://BigBuckBunny_175k.mov");
const std::string ImageStreamProc::url123("rtsp://184.72.239.149/vod/mp4://BigBuckBunny_175k.mov");

ImageStreamProc::ImageStreamProc() :
//    QObject(parent),
    urlinstance("rtsp://184.72.239.149/vod/mp4://BigBuckBunny_175k.mov"),
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

    if (nullptr != pAVFormatContext) {
        avformat_close_input(&pAVFormatContext);
    }
    avformat_free_context(pAVFormatContext);

//    avformat_network_deinit();

//    videoStreamIndex = -1;
//    pAVFormatContext = nullptr;
//    pSwsContext = nullptr;
//    pAVCodecContext = nullptr;
    av_register_all();//注册库中所有可用的文件格式和解码器
    avformat_network_init();//初始化网络流格式,使用RTSP网络流时必须先执行
    pAVFrame = av_frame_alloc();//仅分配AVFrame，而不分配buffer

    pAVFormatContext = avformat_alloc_context();//申请一个AVFormatContext结构的内存,并进行简单初始化

    //打开视频流
    LOG(INFO) << "++++++++++++++++++ usrlocal:  " << urllocal;
    LOG(INFO) << "++++++++++++++++++ usrclass:  " << url123;
    LOG(INFO) << "++++++++++++++++++ usrinstance:  " << urlinstance;

    int result = avformat_open_input(&pAVFormatContext, "rtsp://184.72.239.149/vod/mp4://BigBuckBunny_175k.mov", NULL, NULL);
    if (result != 0) {
        LOG(INFO) << "open video stream failed!!!!!!";
        goto error;
    }

    //获取视频流信息
    result = avformat_find_stream_info(pAVFormatContext,NULL);
    if (result < 0){
        LOG(INFO) << "get video stream information failed!!!!!";
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
        LOG(INFO) << "get video stream index failed!!!!!";
        goto error;
    }

    oc = avformat_alloc_context();
    av_init_packet(&pAVPacket);

    av_read_play(pAVFormatContext);//play RTSP

    //获取视频流的分辨率大小
    pAVCodecContext = pAVFormatContext->streams[videoStreamIndex]->codec;
    //    avcodec_copy_context(pAVCodecContext, pAVFormatContext->streams[videoStreamIndex]->codec);
    videoWidth = pAVCodecContext->width;
    videoHeight = pAVCodecContext->height;

    avpicture_alloc(&pAVPicture,AV_PIX_FMT_RGB24,videoWidth,videoHeight);

    AVCodec *pAVCodec;

    //获取视频流解码器
    pAVCodec = avcodec_find_decoder(pAVCodecContext->codec_id);
//    pSwsContext = sws_getContext(videoWidth, videoHeight, AV_PIX_FMT_YUV420P, videoWidth, videoHeight, AV_PIX_FMT_RGB24, SWS_BICUBIC, 0, 0, 0);
    pSwsContext = sws_getContext(videoWidth, videoHeight, pAVCodecContext->pix_fmt, videoWidth, videoHeight, AV_PIX_FMT_RGB24, SWS_BICUBIC, 0, 0, 0);

    //打开对应解码器
    result = avcodec_open2(pAVCodecContext, pAVCodec, NULL);
    if (result<0){
        LOG(INFO) << "open decoder failed!!!!!";
        goto error;
    }

    LOG(INFO) << "init video stream success!!!!";

    streamReady = true;
    cvStreamReady.notify_all();
    LOG(INFO) << "ImageStreamProc::init lock relase streamReady = " << streamReady;


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
    AVStream* stream = NULL;
    while (true) {
        boost::unique_lock<boost::mutex> lock(mtxStreamReady);
        while (!streamReady)
            cvStreamReady.wait(lock);

//        if (av_read_frame(pAVFormatContext, &pAVPacket) >= 0){
//            avcodec_decode_video2(pAVCodecContext, pAVFrame, &frameFinished, &pAVPacket);
//            if (frameFinished){
//                sws_scale(pSwsContext, (const uint8_t* const *)pAVFrame->data, pAVFrame->linesize, 0, videoHeight, pAVPicture.data, pAVPicture.linesize);
//                //发送获取一帧图像信号
//                //                    QImage image(pAVPicture.data[0], videoWidth, videoHeight, QImage::Format_RGB888);
//                //                    emit imageGot(image);
//            }
////            av_free_packet(&pAVPacket);//释放资源,否则内存会一直上升
//        }

        if (av_read_frame(pAVFormatContext,&pAVPacket)>=0)
        {//read 100 frames
            if(pAVPacket.stream_index == videoStreamIndex){//packet is video
                if(stream == NULL)
                {//create stream in file
                    std::cout << "3 create stream" << std::endl;
                    stream = avformat_new_stream(oc,pAVFormatContext->streams[videoStreamIndex]->codec->codec);
                    avcodec_copy_context(stream->codec,pAVFormatContext->streams[videoStreamIndex]->codec);
                    stream->sample_aspect_ratio = pAVFormatContext->streams[videoStreamIndex]->codec->sample_aspect_ratio;
                }
                int check = 0;
                pAVPacket.stream_index = stream->id;
//                std::cout << "4 decoding" << std::endl;
                int result = avcodec_decode_video2(pAVFormatContext, pAVFrame, &check, &pAVPacket);
                std::cout << "Bytes decoded " << result << " check " << check << std::endl;
//                if(cnt > 100)//cnt < 0)
//                {
//                    sws_scale(pSwsContext, pAVFrame->data, pAVFrame->linesize, 0, ccontext->height, picrgb->data, picrgb->linesize);
//                    std::stringstream name;
//                    name << "test" << cnt << ".ppm";
//                    myfile.open(name.str());
//                    myfile << "P3 " << ccontext->width << " " << ccontext->height << " 255\n";
//                    for(int y = 0; y < ccontext->height; y++)
//                    {
//                        for(int x = 0; x < ccontext->width * 3; x++)
//                            myfile << (int)(picrgb->data[0] + y * picrgb->linesize[0])[x] << " ";
//                    }
//                    myfile.close();
//                }
//                cnt++;
            }
            av_free_packet(&pAVPacket);
            av_init_packet(&pAVPacket);
        }
    }
}
