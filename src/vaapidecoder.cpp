#include "vaapidecoder.h"
#include <glog/logging.h>
#include <QTimer>


static enum AVPixelFormat hw_pix_fmt;

int VaapiDecoder::hw_decoder_init(AVCodecContext *ctx, const enum AVHWDeviceType type)
{
    int err = 0;

    if ((err = av_hwdevice_ctx_create(&hw_device_ctx, type,
                                      NULL, NULL, 0)) < 0) {
        LOG(INFO) << "Failed to create specified HW device";
        return err;
    }
    ctx->hw_device_ctx = av_buffer_ref(hw_device_ctx);

    return err;
}

enum AVPixelFormat VaapiDecoder::get_hw_format(AVCodecContext *ctx,
                                        const enum AVPixelFormat *pix_fmts)
{
    const enum AVPixelFormat *p;

    for (p = pix_fmts; *p != -1; p++) {
        if (*p == hw_pix_fmt)
            return *p;
    }

    fprintf(stderr, "Failed to get HW surface format.\n");
    return AV_PIX_FMT_NONE;
}

int VaapiDecoder::decode_write(AVCodecContext *avctx, AVPacket *packet)
{
    if (NULL == avctx || NULL == packet) {
        return -1;
    }

    int ret = avcodec_send_packet(avctx, packet);
    if (ret < 0) {
        LOG(INFO) << "VaapiDecoder::decode_write send packet error";
        return ret;
    }

    while (ret >= 0) {
        AVFrame *frame = NULL;
        AVFrame *sw_frame = NULL;
        if (NULL == (frame = av_frame_alloc()) || (NULL == (sw_frame = av_frame_alloc()))) {
            LOG(INFO) << "VaapiDecoder::decode_write alloc frame error";
            ret = AVERROR(ENOMEM);
            return ret;
        }

        ret = avcodec_receive_frame(avctx, frame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            av_frame_free(&frame);
            av_frame_free(&sw_frame);
            return 0;
        }
        else if (ret < 0) {
            LOG(INFO) << "VaapiDecoder::decode_write decode error";
            av_frame_free(&frame);
            av_frame_free(&sw_frame);
            return ret;
        }

        AVFrame *tmp_frame = NULL;
        if (frame->format == hw_pix_fmt) {
            /* retrieve data from GPU to CPU */
            if ((ret = av_hwframe_transfer_data(sw_frame, frame, 0)) < 0) {
                LOG(INFO) << "VaapiDecoder::decode_write transfer data error";
                av_frame_free(&frame);
                av_frame_free(&sw_frame);
                return ret;
            }
            tmp_frame = sw_frame;
        } else
            tmp_frame = frame;

//        int frame_width = tmp_frame->width;
//        int frame_height = tmp_frame->height;
//        struct SwsContext *img_convert_ctx =
//                sws_getContext(frame_width, frame_height, (AVPixelFormat)tmp_frame->format, frame_width, frame_height,
//                                                            AV_PIX_FMT_RGB32, SWS_BICUBIC, 0, 0, 0);;
//        int dstStride[1]{frame_width * 4};
//        uint8_t *dst[1]{(uint8_t*)malloc(frame_height * dstStride[0])};
//        sws_scale(img_convert_ctx, (const uint8_t * const *)tmp_frame->data, tmp_frame->linesize,
//                  0, frame_height, dst, dstStride);
//        QImage image(frame_width, frame_height, QImage::Format_RGB32);
//        memcpy(image.bits(), dst[0], frame_width * frame_height * 4);
//        emit imageGot();

//        av_frame_free(&frame);
//        av_frame_free(&sw_frame);
//        sws_freeContext(img_convert_ctx);
//        free(dst[0]);

        struct SwsContext *img_convert_ctx =
                sws_getContext(tmp_frame->width, tmp_frame->height, (AVPixelFormat)tmp_frame->format,
                               tmp_frame->width, tmp_frame->height, AV_PIX_FMT_RGB32, SWS_BICUBIC, 0, 0, 0);

        AVFrame decoded_frame;
        frame_width = decoder_ctx->width;
        frame_height = decoder_ctx->height;
        decoded_frame.format = AV_PIX_FMT_RGB32;
        decoded_frame.width = frame_width;
        decoded_frame.height = frame_height;
        if ((ret = av_image_alloc(decoded_frame.data, decoded_frame.linesize,
                                  frame_width, frame_height, AV_PIX_FMT_RGB32, 32)) < 0) {
            LOG(INFO) << "VaapiDecoder::decode_write alloc decoded frame failed";
            ret = AVERROR(ENOMEM);
            av_frame_free(&frame);
            av_frame_free(&sw_frame);
            return ret;
        }
        sws_scale(img_convert_ctx, (const uint8_t * const *)tmp_frame->data, tmp_frame->linesize, 0, tmp_frame->height,
                  decoded_frame.data, decoded_frame.linesize);
//        emit imageGot(); //FIXME block until draw image finished
        pushFrame(decoded_frame);
//        QImage image(decoded_frame.data[0], tmp_frame->width, tmp_frame->height, QImage::Format_RGB32);
//        QImage image(frame_width, frame_height, QImage::Format_RGB32);
//        memcpy(image.bits(), dst_frame.data[0], frame_width * frame_height * 4); //让QImage管理自己到内存，不使用dst_frame.data[0]
//        emit imageGot(image);

        av_frame_free(&frame);
        av_frame_free(&sw_frame);
        sws_freeContext(img_convert_ctx);
    }

    return ret;
}


VaapiDecoder::VaapiDecoder(QObject *parent) :
        QObject(parent),
        input_ctx(NULL),
        video(NULL),
        decoder_ctx(NULL),
        decoder(NULL),
        hw_device_ctx(NULL),
        video_stream(-1),
        streamInputReady(false),
        streamDecoderReady(false),
        frame_width(-1),
        frame_height(-1),
        url("rtsp://192.168.0.1/livestream/12"),
        deviceType("vaapi"),
        readStreamThread(new QThread)
{
//    av_log_set_level(AV_LOG_SKIP_REPEATED);
//    av_init_packet(&packet);
    curFrame.format = AV_PIX_FMT_NONE;
    moveToThread(readStreamThread);
    input_format = av_find_input_format("rtsp");
    readStreamThread->start(QThread::HighestPriority);
    readStream();
}

VaapiDecoder::~VaapiDecoder()
{
    //先关闭解码和取流线程，再释放资源
    readFrameThread.interrupt();
    readFrameThread.join();
    playThread.interrupt();
    playThread.join();
    decodeFrameThread.interrupt();
    decodeFrameThread.join();
    readStreamThread->quit();
    readStreamThread->wait();
    delete readStreamThread;
    deInit();
}

//返回0成功，其他值失败
int VaapiDecoder::init()
{
    int i;
    int ret;
    std::string errorMsg;

//    char *device_type = "vdpau";
    const char *device_type = deviceType.c_str();
    const char *input_file = url.c_str();

    LOG(INFO) << "1.av_hwdevice_find_type_by_name ###########################";
    type = av_hwdevice_find_type_by_name(device_type);
    if (type == AV_HWDEVICE_TYPE_NONE) {
        char msg[1024];
        int size = 0;
        size = snprintf(msg, sizeof(msg), "Device type %s is not supported.\n", device_type);
        size += snprintf(msg + size, sizeof(msg), "Available device types:");
        while((type = av_hwdevice_iterate_types(type)) != AV_HWDEVICE_TYPE_NONE)
            snprintf(msg + size, sizeof(msg), " %s", av_hwdevice_get_type_name(type));

        errorMsg = msg;
        ret = -1;
        goto error;
    }

    /* open the input file */
    LOG(INFO) << "2.avformat_open_input ###########################";
    if ((ret = avformat_open_input(&input_ctx, input_file, input_format, NULL)) != 0) {
        errorMsg += "Cannot open input file "; errorMsg += input_file;
        goto error;
    }
    input_ctx->probesize = 1024;

    LOG(INFO) << "3.avformat_find_stream_info ###########################";
    if ((ret = avformat_find_stream_info(input_ctx, NULL)) < 0) {
        errorMsg += "Cannot find input stream information";
        goto error;
    }

    /* find the video stream information */
    LOG(INFO) << "4.av_find_best_stream ###########################";
    ret = av_find_best_stream(input_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, &decoder, 0);
    if (ret < 0) {
        errorMsg += "Cannot find a video stream in the input file";
        goto error;
    }
    video_stream = ret;

    LOG(INFO) << "5.avcodec_get_hw_config ###########################";
    for (i = 0;; i++) {
        const AVCodecHWConfig *config = avcodec_get_hw_config(decoder, i);
        if (!config) {
            fprintf(stderr, "Decoder %s does not support device type %s.\n",
                    decoder->name, av_hwdevice_get_type_name(type));
            errorMsg += "Decoder "; errorMsg += decoder->name;
            errorMsg += " does not support device type "; errorMsg += av_hwdevice_get_type_name(type);
            goto error;
        }
        if (config->methods & AV_CODEC_HW_CONFIG_METHOD_HW_DEVICE_CTX &&
            config->device_type == type) {
            hw_pix_fmt = config->pix_fmt;
            break;
        }
    }

    LOG(INFO) << "6.avcodec_alloc_context3 ###########################";
    if (!(decoder_ctx = avcodec_alloc_context3(decoder))) {
        ret = AVERROR(ENOMEM);
        errorMsg += "avcodec_alloc_context3 failed";
        goto error;
    }

    LOG(INFO) << "7.avcodec_parameters_to_context ###########################";
    video = input_ctx->streams[video_stream];
    if ((ret = avcodec_parameters_to_context(decoder_ctx, video->codecpar)) < 0) {
        errorMsg += "avcodec_parameters_to_context failed";
        goto error;
    }

    decoder_ctx->get_format  = get_hw_format;
    av_opt_set_int(decoder_ctx, "refcounted_frames", 1, 0);

    LOG(INFO) << "8.hw_decoder_init ###########################";
    if (hw_decoder_init(decoder_ctx, type) < 0) {
        return -1;
    }

    LOG(INFO) << "9.avcodec_open2 ###########################";
    if ((ret = avcodec_open2(decoder_ctx, decoder, NULL)) < 0) {
        errorMsg += "Failed to open codec for stream #"; errorMsg += video_stream;
        goto error;
    }

    LOG(INFO) << "10.VaapiDecoder::play thread start###########################";
    readFrameThread = boost::thread(&VaapiDecoder::readFrame, this);
    decodeFrameThread = boost::thread(&VaapiDecoder::decodeFrame, this);
    playThread = boost::thread(&VaapiDecoder::play, this);

    LOG(INFO) << "VaapiDecoder::init init video stream success!!!!";
    streamDecoderReady = true;
    streamInputReady = true;
    return 0;

    error:
    LOG(INFO) << "VaapiDecoder::init failed error:" << errorMsg;
    streamInputReady = false;
    streamDecoderReady = false;
    char errorStr[1024] = {0};
    av_strerror(ret, errorStr, 1024);
    LOG(INFO) << "VaapiDecoder::init got an error:" << ret << " :" << errorStr;
    return ret;
}

void VaapiDecoder::deInit()
{
    /* flush the decoder */
//    packet.data = NULL;
//    packet.size = 0;
//    decode_write(decoder_ctx, &packet);
//    av_packet_unref(&packet);

    avcodec_close(decoder_ctx);
    avcodec_free_context(&decoder_ctx);
    avformat_close_input(&input_ctx);
    av_buffer_unref(&hw_device_ctx);

    //没有初始化decoded_frame.data[0]，不能进行free
//    if (decoded_frame.format == AV_PIX_FMT_RGB32) {
//        av_freep(&decoded_frame.data[0]);
//    }

    input_ctx = NULL;
    video = NULL;
    decoder_ctx = NULL;
    decoder = NULL;
    hw_device_ctx = NULL;
    video_stream = -1;
    type = AV_HWDEVICE_TYPE_NONE;
    streamInputReady = false;
    streamDecoderReady = false;
}

void VaapiDecoder::readFrame()
{
    int ret = 0;
    while (ret >= 0) {
        boost::this_thread::interruption_point();

        AVPacket packet;
        {
            boost::unique_lock<boost::mutex> lock(mtxStreamInputReady);
            while (!streamInputReady) {
                cvStreamInputReady.wait(lock);
            }

            if ((ret = av_read_frame(input_ctx, &packet)) < 0)
                break;
        }

        pushPacket(packet);
    }
}

bool VaapiDecoder::readStream()
{
    LOG(INFO) << "VaapiDecoder::readStream read image stream";
    QTimer::singleShot(0, this, SLOT(_readStream()));
    return true;
}

void VaapiDecoder::_readStream()
{
    LOG(INFO) << "VaapiDecoder::_readStream start";
    int i = 100; //如果无视频流，重复连接10次
    {
        boost::unique_lock<boost::mutex> locki(mtxStreamInputReady);
        boost::unique_lock<boost::mutex> lockd(mtxStreamDecoderReady);
        LOG(INFO) << "0.deInit stream ###########################";
        deInit();
        while (i && init()) {
            --i;
            boost::this_thread::sleep_for(boost::chrono::milliseconds(10));
        }
        cvStreamInputReady.notify_one();
        cvStreamDecoderReady.notify_one();
    }

    emit readStreamDone(i);
}

void VaapiDecoder::pushFrame(const AVFrame &frame) {
    boost::unique_lock<boost::mutex> lock(mtxFrameQueue);
    frameQueue.push_back(frame);
    cvFrameQueue.notify_one();
}

void VaapiDecoder::popFrame(AVFrame &frame) {
    boost::unique_lock<boost::mutex> lock(mtxFrameQueue);
    while (frameQueue.empty()) {
        cvFrameQueue.wait(lock);
    }

    frame = frameQueue.front();
    frameQueue.pop_front();
}

void VaapiDecoder::play() {
    while (true) {
        boost::this_thread::interruption_point();
        if (curFrame.format != AV_PIX_FMT_NONE) {
            av_freep(&curFrame.data[0]);
        }
        popFrame(curFrame);
        emit imageGot();
    }
}

AVFrame *VaapiDecoder::getCurFrame() {
    return &curFrame;
}

void VaapiDecoder::decodeFrame() {

    int ret = 0;
    while (ret >= 0) {
        boost::this_thread::interruption_point();

        AVPacket packet;
        popPacket(packet);
        {
            boost::unique_lock<boost::mutex> lock(mtxStreamDecoderReady);
            while (!streamDecoderReady) {
                cvStreamDecoderReady.wait(lock);
            }
            if (video_stream == packet.stream_index)
                ret = decode_write(decoder_ctx, &packet);
        }
        av_packet_unref(&packet);

        //休眠让其他线程能拿到cpu时间片
//        lock.unlock();
//        boost::this_thread::sleep_for(boost::chrono::milliseconds(5));
//        lock.lock();
    }
}

void VaapiDecoder::pushPacket(const AVPacket &packet) {
    boost::unique_lock<boost::mutex> lock(mtxPacketsQuue);
    packetsQuue.push_back(packet);
    cvPacketsQuue.notify_one();
}

void VaapiDecoder::popPacket(AVPacket &packet) {
    boost::unique_lock<boost::mutex> lock(mtxPacketsQuue);
    while (packetsQuue.empty()) {
        cvPacketsQuue.wait(lock);
    }
    packet = packetsQuue.front();
    packetsQuue.pop_front();
}
