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
#ifdef linux
#include <libavutil/hwcontext.h>
#endif
#include <libavutil/opt.h>
#include <libavutil/avassert.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <libavformat/avformat.h>
}

//struct MediaFrame_AI_Info
//{
//    uint64_t frameID;
//    float bodyROIs[15][4];
//    int bodyNum;
//    float faceROIs[15][4];
//    int faceNum;
//    float handROIs[15][4];
//    int handNum;
//    float targetROIs[2][4];
//    int targetNum;
//    int hpResult;
//    int capResult[3];
//};
#pragma pack(1)
struct MediaFrame_AI_Info
{
    uint8_t u8SDKStatus;
    uint64_t u64TimeStamp;
    uint8_t u8HpType;
    uint8_t u8HpCount;
    uint8_t u8CapSF;
    uint8_t u8CapSA;
    uint8_t u8CapSL;
    uint8_t u8CtrlType;
    float fCtrlDx;
    float fCtrlDy;
    float fCtrlScale;
    float fCtrlGap;
    uint8_t u8NumTarget;
    uint8_t u8NumPerson;
    uint8_t u8NumFace;
    uint8_t u8NumHand;
    uint8_t u8TargetRois[2][18];
    uint8_t u8PersonRois[15][16];
    uint8_t u8FaceRois[15][16];
    uint8_t u8Hand[15][16];
};
#pragma pack()

struct MediaFrame {
    MediaFrame_AI_Info ai_info;
    AVFrame image;
    bool hasAiInfo;
};

class MediaStreamProc : public QObject
{
    Q_OBJECT
public:
    MediaStreamProc(QObject *parent = nullptr);
    ~MediaStreamProc();
//    AVFrame *getDecodedFrame() {return &decoded_frame;}
    MediaFrame *getCurFrame();
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
    int decode_write_normal(AVCodecContext *avctx, AVPacket *packet);
    bool decodeAiInfoFrame(const AVPacket &packet, MediaFrame_AI_Info &aiInfo);

#ifdef linux
    int decode_write_vaapi(AVCodecContext *avctx, AVPacket *packet);
    static enum AVPixelFormat get_hw_format(AVCodecContext *ctx,
                                     const enum AVPixelFormat *pix_fmts);
    int hw_decoder_init(AVCodecContext *ctx, const enum AVHWDeviceType type);
    int vaapiInit(); //返回0表示成功
#endif
    int init(); //返回0表示成功
    int normalInit(); //返回0表示成功
    void deInit();
    void pushFrame(const MediaFrame &frame);
    void popFrame(MediaFrame &frame);
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
    std::string url;
    std::string deviceType;
    int frame_width;
    int frame_height;
    MediaFrame curFrame;
    std::string decoderCfg;

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

    std::deque<MediaFrame> frameQueue;
    boost::mutex mtxFrameQueue;
    boost::condition_variable cvFrameQueue;
    boost::mutex mtxFrameQueueFull;
    boost::condition_variable cvFrameQueueFull;
    const int frameQueueSize;
};

#endif // MediaStreamProc_H
