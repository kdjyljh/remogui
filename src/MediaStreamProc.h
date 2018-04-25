#ifndef MediaStreamProc_H
#define MediaStreamProc_H

#include <QObject>
#include <QImage>
#include <QThread>

#include <stdio.h>
#include <deque>
#include <boost/thread.hpp>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/pixdesc.h>
#include <libavutil/hwcontext.h>
#include <libavutil/opt.h>
#include <libavutil/avassert.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
}

class MediaStreamProc : public QObject
{
    Q_OBJECT
public:
    MediaStreamProc(QObject *parent = nullptr);
    ~MediaStreamProc();
//    AVFrame *getDecodedFrame() {return &decoded_frame;}
    AVFrame *getCurFrame();
    bool isValid() {return streamDecoderReady && streamInputReady;}

signals:
    void imageGot(const QImage &image);
    void imageGot();
    bool readStreamDone(bool gotStream);

public slots:
    bool readStream();
    bool syncReadStream();

private slots:
    void _readStream();

private:
    void readFrame();
    void decodeFrame();
    void play();
    int decode_write_vaapi(AVCodecContext *avctx, AVPacket *packet);
    int decode_write_normal(AVCodecContext *avctx, AVPacket *packet);
    static enum AVPixelFormat get_hw_format(AVCodecContext *ctx,
                                     const enum AVPixelFormat *pix_fmts);
    int hw_decoder_init(AVCodecContext *ctx, const enum AVHWDeviceType type);
    int init(); //返回0表示成功
    int vaapiInit(); //返回0表示成功
    int normalInit(); //返回0表示成功
    void deInit();
    void pushFrame(const AVFrame &frame);
    void popFrame(AVFrame &frame);
    void pushPacket(const AVPacket &packet);
    void popPacket(AVPacket &packet);

private:
    AVFormatContext *input_ctx;
    AVStream *video;
    AVCodecContext *decoder_ctx;
    AVCodec *decoder;
    AVBufferRef *hw_device_ctx;
    AVInputFormat *input_format;
    int video_stream;
//    AVPacket packet;
//    AVFrame decoded_frame;
    enum AVHWDeviceType type;
    std::string url;
    std::string deviceType;
    int frame_width;
    int frame_height;
    AVFrame curFrame;

    enum DecoderType {
        DecoderType_None    = 0,
        DecoderType_Normal    = 1,
        DecoderType_Vaapi   = 2,
    } decoderType;

private:
    boost::mutex mtxStreamInputReady;
    boost::condition_variable cvStreamInputReady;
    bool streamInputReady; //多线程共享变量，需要加锁

    boost::mutex mtxStreamDecoderReady;
    boost::condition_variable cvStreamDecoderReady;
    bool streamDecoderReady; //多线程共享变量，需要加锁

    QThread *readStreamThread;
    boost::thread readFrameThread;
    boost::thread playThread;
    boost::thread decodeFrameThread;

    std::deque<AVPacket> packetsQuue;
    boost::mutex mtxPacketsQuue;
    boost::condition_variable cvPacketsQuue;

    std::deque<AVFrame> frameQueue;
    boost::mutex mtxFrameQueue;
    boost::condition_variable cvFrameQueue;
    boost::mutex mtxFrameQueueFull;
    boost::condition_variable cvFrameQueueFull;
    const int frameQueueSize;
};

#endif // MediaStreamProc_H