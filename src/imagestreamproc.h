#ifndef IMAGESTREAMPROC_H
#define IMAGESTREAMPROC_H

#include <QObject>
#include <QThread>

////必须加以下内容,否则编译不能通过,为了兼容C和C99标准
//#ifndef INT64_C
//#define INT64_C
//#define UINT64_C
//#endif

//引入ffmpeg头文件
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/avfilter.h>
#include <libswscale/swscale.h>
#include <libavutil/frame.h>
}
#include <string>

#include <QObject>
#include <QImage>

class ImageStreamProc : public QObject
{
    Q_OBJECT
public:
    explicit ImageStreamProc(QObject *parent = nullptr); // 完成一些注册和内存资源分配的工作
    virtual ~ImageStreamProc(); // 回收资源

public slots:
    void play();

signals:
    void imageGot(const QImage &image);
    bool readStreamDone(bool gotStream);

public slots:
    bool readStream();
    void _readStream();

private:
    AVPicture  pAVPicture;
    AVFormatContext *pAVFormatContext;
    AVCodecContext *pAVCodecContext;
    AVFrame *pAVFrame;
    SwsContext * pSwsContext;
    AVInputFormat * avInputFormat;
    AVPacket pAVPacket;
    std::string url;
    QThread *readStreamThread;


    int videoWidth;
    int videoHeight;
    int videoStreamIndex;
    static bool streamReady; //多线程共享变量，需要加锁

    bool init();

private slots:
};

#endif // IMAGESTREAMPROC_H
