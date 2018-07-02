#include "MediaStreamProc.h"
#include <glog/logging.h>
#include <QTimer>
#include <boost/asio.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include "commlog.h"

#ifdef linux
static enum AVPixelFormat hw_pix_fmt;
enum AVHWDeviceType type;
int MediaStreamProc::hw_decoder_init(AVCodecContext *ctx, const enum AVHWDeviceType type)
{
    int err = 0;

    if ((err = av_hwdevice_ctx_create(&hw_device_ctx, type,
                                      nullptr, nullptr, 0)) < 0) {
        LOG(INFO) << "Failed to create specified HW device";
        return err;
    }
    ctx->hw_device_ctx = av_buffer_ref(hw_device_ctx);

    return err;
}

enum AVPixelFormat MediaStreamProc::get_hw_format(AVCodecContext *ctx,
                                        const enum AVPixelFormat *pix_fmts)
{
    const enum AVPixelFormat *p;

    for (p = pix_fmts; *p != -1; p++) {
        if (*p == hw_pix_fmt)
            return *p;
    }

    LOG(INFO) << "Failed to get HW surface format.";
    return AV_PIX_FMT_NONE;
}

int MediaStreamProc::decode_write_vaapi(AVCodecContext *avctx, AVPacket *packet) {
    if (nullptr == avctx || nullptr == packet) {
        return -1;
    }

    int ret = avcodec_send_packet(avctx, packet);
    if (ret < 0) {
        LOG(INFO) << "MediaStreamProc::decode_write send packet error";
        return ret;
    }

    MediaFrame mediaFrame;
    memset(&mediaFrame, 0, sizeof(MediaFrame_AI_Info));
    mediaFrame.hasAiInfo = decodeAiInfoFrame(*packet, mediaFrame.ai_info);

    while (ret >= 0) {
        AVFrame *frame = nullptr;
        AVFrame *sw_frame = nullptr;
        if (nullptr == (frame = av_frame_alloc()) || (nullptr == (sw_frame = av_frame_alloc()))) {
            LOG(INFO) << "MediaStreamProc::decode_write alloc frame error";
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
            LOG(INFO) << "MediaStreamProc::decode_write decode error";
            av_frame_free(&frame);
            av_frame_free(&sw_frame);
            return ret;
        }

        AVFrame *tmp_frame = nullptr;
        if (frame->format == hw_pix_fmt) {
            /* retrieve data from GPU to CPU */
            if ((ret = av_hwframe_transfer_data(sw_frame, frame, 0)) < 0) {
                LOG(INFO) << "MediaStreamProc::decode_write transfer data error";
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
                               tmp_frame->width, tmp_frame->height, AV_PIX_FMT_RGB32, SWS_FAST_BILINEAR, 0, 0, 0);

        AVFrame decoded_frame;
        frame_width = decoder_ctx->width;
        frame_height = decoder_ctx->height;
        decoded_frame.format = AV_PIX_FMT_RGB32;
        decoded_frame.width = frame_width;
        decoded_frame.height = frame_height;
        if ((ret = av_image_alloc(decoded_frame.data, decoded_frame.linesize,
                                  frame_width, frame_height, AV_PIX_FMT_RGB32, 32)) < 0) {
            LOG(INFO) << "MediaStreamProc::decode_write alloc decoded frame failed";
            ret = AVERROR(ENOMEM);
            av_frame_free(&frame);
            av_frame_free(&sw_frame);
            return ret;
        }
        sws_scale(img_convert_ctx, (const uint8_t * const *)tmp_frame->data, tmp_frame->linesize, 0, tmp_frame->height,
                  decoded_frame.data, decoded_frame.linesize);
        mediaFrame.image = decoded_frame;
        pushFrame(mediaFrame);
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

int MediaStreamProc::vaapiInit() {
    int i;
    int ret;
    std::string errorMsg;

//    char *device_type = "vdpau";
    const char *device_type = deviceType.c_str();
    const char *input_file = url.c_str();
    int setResult = -1;
    char errorStr[1024] = {0};

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
    if ((ret = avformat_open_input(&input_ctx, input_file, nullptr, nullptr)) != 0) {
        errorMsg += "Cannot open input file "; errorMsg += input_file;
        goto error;
    }
    input_ctx->probesize = 1024;

    LOG(INFO) << "3.avformat_find_stream_info ###########################";
    if ((ret = avformat_find_stream_info(input_ctx, nullptr)) < 0) {
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
    if ((ret = avcodec_open2(decoder_ctx, decoder, nullptr)) < 0) {
        errorMsg += "Failed to open codec for stream #"; errorMsg += video_stream;
        goto error;
    }

//    setResult = av_opt_set(decoder_ctx->priv_data, "preset", "slow", AV_OPT_SEARCH_CHILDREN);
//    setResult = av_opt_set_int(decoder_ctx, "crf", 2, 0);
//    setResult = av_opt_set(decoder_ctx->codec, "preset", "slow", 0);
    av_strerror(setResult, errorStr, 1024);
    LOG(INFO) << "set error" << errorStr;
    setResult = av_opt_set(decoder_ctx->priv_data, "tune", "zerolatency", 0);
    setResult = av_opt_set(decoder_ctx->priv_data, "crf", "51", 0);

    LOG(INFO) << "MediaStreamProc::vaapiInit init video stream success!!!!";
    LOG(INFO) << "MediaStreamProc::vaapiInit decoder name:" << decoder_ctx->codec->name;

    streamDecoderReady = true;
    streamInputReady = true;
    return 0;

    error:
    LOG(INFO) << "MediaStreamProc::vaapiInit failed error:" << errorMsg;
    streamInputReady = false;
    streamDecoderReady = false;
    av_strerror(ret, errorStr, 1024);
    LOG(INFO) << "MediaStreamProc::vaapiInit got an error:" << ret << " :" << errorStr;
    return ret;
}
#endif

//rtsp://192.168.0.1/livestream/12
//rtsp://184.72.239.149/vod/mp4://BigBuckBunny_175k.mov
///home/jianghualuo/work/data/videos/bandicam.avi
///home/jianghualuo/work/data/videos/FigureSkating.mp4
MediaStreamProc::MediaStreamProc(QObject *parent) :
        QObject(parent),
        input_ctx(nullptr),
        video(nullptr),
        decoder_ctx(nullptr),
        decoder(nullptr),
        hw_device_ctx(nullptr),
        video_stream(-1),
        streamInputReady(false),
        streamDecoderReady(false),
        frame_width(-1),
        frame_height(-1),
        frameQueueSize(30),
        deviceType("vaapi"),
        decoderType(DecoderType_None),
        readStreamThread(new QThread(this))
{
    LOG(INFO) << "MediaStreamProc::MediaStreamProc constructor this:" << this;
//    av_log_set_level(AV_LOG_SKIP_REPEATED);
    try {
        boost::property_tree::ptree root;
        boost::property_tree::read_json("remo_gui.json", root);
        url = root.get<std::string>("VideoStreamUrl");
        decoderCfg = root.get<std::string>("VideoStreamDecoder");
    } catch (boost::property_tree::ptree_error &e) {
        LOG(INFO) << "MediaStreamProc::MediaStreamProc json parse error:" << e.what();
    }

    curFrame.image.format = AV_PIX_FMT_NONE;
    moveToThread(readStreamThread);
    input_format = av_find_input_format("rtsp");
    readStreamThread->start(QThread::HighestPriority);
    readStream(); //异步读取视频流
}

MediaStreamProc::~MediaStreamProc()
{
    readStreamThread->quit();
    readStreamThread->wait();
//    //先关闭解码和取流线程，再释放资源
//    readFrameThread.interrupt();
//    readFrameThread.join();
//    playThread.interrupt();
////    playThread.join(); //不使用jion，可能产生死锁
//    playThread.detach(); //使用detach释放线程资源
//    decodeFrameThread.interrupt();
//    decodeFrameThread.join();
    deInit();
}

//返回0成功，其他值失败
int MediaStreamProc::init()
{
    int ret = -1;
#ifdef linux
    if (decoderCfg.empty() || decoderCfg == "Auto") {
        if ((ret = vaapiInit())) {
            LOG(INFO) << "MediaStreamProc::init vaapiInit failed try normal!!!!!!!!!!!!!";
            deInit();
            if ((ret = normalInit())) {
                LOG(INFO) << "MediaStreamProc::init normalInit failed!!!!!!!!!!!!!";
            } else {
                decoderType = DecoderType_Normal;
            }
        } else {
            decoderType = DecoderType_Vaapi;
        }
    } else if ("Normal" == decoderCfg) {
        ret = normalInit();
        decoderType = DecoderType_Normal;
    } else if ("Vaapi" == decoderCfg) {
        ret = vaapiInit();
        decoderType = DecoderType_Vaapi;
    }
#else
    ret = normalInit();
    decoderType = DecoderType_Normal;
#endif

    return ret;
}

void MediaStreamProc::deInit()
{
//    boost::unique_lock<boost::mutex> locki(mtxStreamInputReady);
//    boost::unique_lock<boost::mutex> lockd(mtxStreamDecoderReady);
    //先关闭解码和取流线程，再释放资源
    readFrameThread.interrupt();
    readFrameThread.join();
    playThread.interrupt();
//    playThread.join(); //不使用jion，可能产生死锁
    playThread.detach(); //使用detach释放线程资源
    decodeFrameThread.interrupt();
    decodeFrameThread.join();

    avcodec_close(decoder_ctx);
    avcodec_free_context(&decoder_ctx);
    LOG(INFO) << "MediaStreamProc::deInit end input_ctx:" << input_ctx;
    avformat_close_input(&input_ctx);
    av_buffer_unref(&hw_device_ctx);

    input_ctx = nullptr;
    video = nullptr;
    decoder_ctx = nullptr;
    decoder = nullptr;
    hw_device_ctx = nullptr;
    video_stream = -1;
#ifdef linux
    type = AV_HWDEVICE_TYPE_NONE;
#endif
    streamInputReady = false;
    streamDecoderReady = false;
}

void MediaStreamProc::readFrame()
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

bool MediaStreamProc::readStream()
{
    QTimer::singleShot(0, this, SLOT(_readStream()));
    return true;
}

void MediaStreamProc::_readStream()
{
    int i = 50; //如果无视频流，重复连接50次
    {
//        boost::unique_lock<boost::mutex> locki(mtxStreamInputReady);
//        boost::unique_lock<boost::mutex> lockd(mtxStreamDecoderReady);
        do {
            LOG(INFO) << "0.deInit #########################";
            deInit();
            if (!init()) {
                break;
            }
            --i;
            boost::this_thread::sleep_for(boost::chrono::milliseconds(10));
        } while (i);

//        deInit();
//        while (i && init()) {
//            LOG(INFO) << "0. deInit #########################";
//            deInit();
//            --i;
//            boost::this_thread::sleep_for(boost::chrono::milliseconds(10));
//        }
//        cvStreamInputReady.notify_one();
//        cvStreamDecoderReady.notify_one();
    }

    //一个对象只初始化线程一次，线程资源在析构函数里释放
//    static bool firstRun = true;
//    if (i && firstRun) {
//        //初始化成功，运行线程
//        LOG(INFO) << "10.MediaStreamProc::play thread start###########################";
//        readFrameThread = boost::thread(&MediaStreamProc::readFrame, this);
//        decodeFrameThread = boost::thread(&MediaStreamProc::decodeFrame, this);
//        playThread = boost::thread(&MediaStreamProc::play, this);
//    }
//    firstRun = false;

    emit readStreamDone(i);

    if (i) {
        LOG(INFO) << "10.MediaStreamProc::play thread start###########################";
        readFrameThread = boost::thread(&MediaStreamProc::readFrame, this);
        decodeFrameThread = boost::thread(&MediaStreamProc::decodeFrame, this);
        playThread = boost::thread(&MediaStreamProc::play, this);
    }
}

void MediaStreamProc::pushFrame(const MediaFrame &frame) {
    boost::unique_lock<boost::mutex> lock(mtxFrameQueue);

    if (frameQueue.size() >= frameQueueSize) {
        boost::unique_lock<boost::mutex> lockFull(mtxFrameQueueFull);
        while (frameQueue.size() >= frameQueueSize) {
            lock.unlock();
            cvFrameQueue.notify_one();
            cvFrameQueueFull.wait(lockFull);
        }
        frameQueue.push_back(frame);
    } else {
        frameQueue.push_back(frame);
        cvFrameQueue.notify_one();
    }
}

void MediaStreamProc::popFrame(MediaFrame &frame) {
    boost::unique_lock<boost::mutex> lock(mtxFrameQueue);
    while (frameQueue.empty()) {
        cvFrameQueue.wait(lock);
    }

    frame = frameQueue.front();
    frameQueue.pop_front();

    cvFrameQueueFull.notify_one();
}

void MediaStreamProc::play() {
    while (true) {
        //使用异常捕获interruption_point，立即停止播放线程，并释放资源
        //因为emit imageGot();使用BlockingQueuedConnection，
        // 为了避免和析构函数里面到join产生死锁，也必须马上跳出
        try {
            boost::this_thread::interruption_point();
            if (curFrame.image.format != AV_PIX_FMT_NONE) {
                av_freep(&curFrame.image.data[0]);
            }
            popFrame(curFrame);
            emit imageGot(); //直到图片显示完成后再返回
        } catch (boost::thread_interrupted &e) {
            LOG(INFO) << "MediaStreamProc::play interrupt";
            break;
        }
    }
}

MediaFrame *MediaStreamProc::getCurFrame() {
    return &curFrame;
}

void MediaStreamProc::decodeFrame() {

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
            if (video_stream == packet.stream_index) {
#ifdef linux
                if (decoderType == DecoderType_Normal) {
                    decode_write_normal(decoder_ctx, &packet);
                } else if (decoderType == DecoderType_Vaapi) {
                    ret = decode_write_vaapi(decoder_ctx, &packet);
                }
#else
                decode_write_normal(decoder_ctx, &packet);
#endif
            }
        }
        av_packet_unref(&packet);

        //休眠让其他线程能拿到cpu时间片
//        lock.unlock();
//        boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
//        lock.lock();
    }
}

void MediaStreamProc::pushPacket(const AVPacket &packet) {
    boost::unique_lock<boost::mutex> lock(mtxPacketsQuue);
    packetsQuue.push_back(packet);
    cvPacketsQuue.notify_one();
}

void MediaStreamProc::popPacket(AVPacket &packet) {
    boost::unique_lock<boost::mutex> lock(mtxPacketsQuue);
    while (packetsQuue.empty()) {
        cvPacketsQuue.wait(lock);
    }
    packet = packetsQuue.front();
    packetsQuue.pop_front();
}

bool MediaStreamProc::syncReadStream() {
    _readStream();
    return streamInputReady && streamDecoderReady;
}

int MediaStreamProc::normalInit() {
    int ret;
    char errorStr[1024] = {0};

    if ((ret = avformat_open_input(&input_ctx, url.c_str(), nullptr, nullptr)) < 0) {
        goto error;
    }
    if ((ret = avformat_find_stream_info(input_ctx, nullptr)) < 0) {
        goto error;
    }
    /* select the video stream */
    ret = av_find_best_stream(input_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, &decoder, 0);
    if (ret < 0) {
        goto error;
    }
    video_stream = ret;
    /* create decoding context */
    decoder_ctx = avcodec_alloc_context3(decoder);
    if (!decoder_ctx) {
        LOG(INFO) << "MediaStreamProc::normalInit avcodec_alloc_context3 error";
        ret = AVERROR(ENOMEM);
        return ret;
    }

    avcodec_parameters_to_context(decoder_ctx, input_ctx->streams[video_stream]->codecpar);
    av_opt_set_int(decoder_ctx, "refcounted_frames", 1, 0);
    /* init the video decoder */
    if ((ret = avcodec_open2(decoder_ctx, decoder, nullptr)) < 0) {
        goto error;
    }

    streamInputReady = true;
    streamDecoderReady = true;
    LOG(INFO) << "MediaStreamProc::normalInit init video stream success!!!!";
    LOG(INFO) << "MediaStreamProc::normalInit decoder name:" << decoder_ctx->codec->name;
    return 0;

error:
    streamInputReady = false;
    streamDecoderReady = false;
    av_strerror(ret, errorStr, 1024);
    LOG(INFO) << "MediaStreamProc::normalInit got an error:" << ret << " :" << errorStr;
    return ret;
}

int MediaStreamProc::decode_write_normal(AVCodecContext *avctx, AVPacket *packet) {
    int ret = 0;
    char errorStr[1024] = {0};

    ret = avcodec_send_packet(avctx, packet);
    if (ret < 0) {
        av_strerror(ret, errorStr, 1024);
        LOG(INFO) << "MediaStreamProc::decode_write_normal avcodec_send_packet error:" << errorStr;
        return ret;
    }

    MediaFrame mediaFrame;
    memset(&mediaFrame, 0, sizeof(MediaFrame_AI_Info));
    mediaFrame.hasAiInfo = decodeAiInfoFrame(*packet, mediaFrame.ai_info);

    while (ret >= 0) {
        AVFrame *frame = av_frame_alloc();
        ret = avcodec_receive_frame(avctx, frame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            av_frame_free(&frame);
            break;
        } else if (ret < 0) {
            av_frame_free(&frame);
            av_strerror(ret, errorStr, 1024);
            LOG(INFO) << "MediaStreamProc::decode_write_normal avcodec_receive_frame error:" << errorStr;
            return ret;
        }
        if (ret >= 0) {
            frame->pts = frame->best_effort_timestamp;

            AVPixelFormat format = (AVPixelFormat)frame->format;
            if (format == AV_PIX_FMT_YUVJ420P) {
                format = AV_PIX_FMT_YUV420P;
            }
            struct SwsContext *img_convert_ctx =
                    sws_getContext(frame->width, frame->height, format,
                                   frame->width, frame->height, AV_PIX_FMT_RGB32, SWS_BICUBIC, nullptr, nullptr, nullptr);
            AVFrame decoded_frame; // scaled_frame
            frame_width = decoder_ctx->width; // H264, maybe frame->width is better
            frame_height = decoder_ctx->height;
            decoded_frame.format = AV_PIX_FMT_RGB32;
            decoded_frame.width = frame_width;
            decoded_frame.height = frame_height;
            if ((ret = av_image_alloc(decoded_frame.data, decoded_frame.linesize,
                                      frame_width, frame_height, AV_PIX_FMT_RGB32, 32)) < 0) {
                LOG(INFO) << "MediaStreamProc::decode_write_normal alloc decoded frame failed";
                ret = AVERROR(ENOMEM);
                av_frame_free(&frame);
                return ret;
            }
            sws_scale(img_convert_ctx, (const uint8_t * const *)frame->data, frame->linesize, 0, frame->height,
                      decoded_frame.data, decoded_frame.linesize);
            mediaFrame.image = decoded_frame;
            pushFrame(mediaFrame);

            av_frame_free(&frame);
            sws_freeContext(img_convert_ctx);
        }
        av_frame_free(&frame);
    }

    return 0;
}

bool MediaStreamProc::decodeAiInfoFrame(const AVPacket &packet, MediaFrame_AI_Info &aiInfo) {
    if (packet.stream_index != video_stream) {
        return false;
    }

//    LOG(INFO) << "MediaStreamProc::decodeAiInfoFrame packet data:";
//    CHAR_BUFF_TO_LOG(std::vector<char>(packet.data, packet.data + packet.size));
//    for (int i = 0; i < packet.size - 4; ++i) {
//        uint64_t nal_header = 0x01000000; //本地是小端，将字节反序
//        uint64_t data;
//        memset(&data, 0, 8);
//        memcpy(&data, packet.data + i, 4);
//        if (data == nal_header) {
//            LOG(INFO) << "nal type:" << ((packet.data[i + 4]) & 0x1f);
//        }
//    }

//    LOG(INFO) << "package: done##################";

    //查找0x00 0x00 0x00 0x01 0x0d，算法的NAL头
    //算法的NAL头是每一帧的最后一个
    for (int i = packet.size - 5; i >= 0; --i) {
        uint64_t nal_header = 0x0d01000000; //本地是小端，将字节反序
        uint64_t data;
        memset(&data, 0, 8);
        memcpy(&data, packet.data + i, 5);

        if (data == nal_header) {
            int size = sizeof(MediaFrame_AI_Info);
            memcpy(&aiInfo, packet.data + i + 5, sizeof(MediaFrame_AI_Info));
//            LOG(INFO) << "MediaStreamProc::decodeAiInfoFrame got AI info";
            return true;
        }
    }
    return false;
}
