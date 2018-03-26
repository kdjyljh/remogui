//
// Created by jianghualuo on 18-3-17.
//

#ifndef REMO_GUI_VIDEOSTREAMCONTROL_H
#define REMO_GUI_VIDEOSTREAMCONTROL_H

#include <QObject>
#include <QThread>

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/avfilter.h>
#include <libswscale/swscale.h>
#include <libavutil/frame.h>
#include <libavutil/time.h>
}
#include <string>

#include <QObject>
#include <QImage>

class VideoStreamControl  : public QObject {
Q_OBJECT
public:
    explicit VideoStreamControl(QObject *parent = nullptr);
    virtual ~VideoStreamControl();

    void setUrl(std::string url) {this->url = url;}
    void seek(int seconds);
    void closeReadStreamThread();

public slots:
    void play();

signals:
    void imageGot(const QImage &image);
    bool readStreamDone(bool gotStream);
    void videoFinished();
    void videoTimestampChanged(double time);

public slots:
    bool readStream();
    void _readStream();

private:
    static int readFrameTimeout(void *ctx);

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
    bool streamReady; //多线程共享变量，需要加锁

    bool init();

private slots:
};


#endif //REMO_GUI_VIDEOSTREAMCONTROL_H
