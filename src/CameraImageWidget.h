#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>
#include <QImage>
#include "MediaStreamProc.h"
#include "MainImageWidget.h"

class CameraImageWidget : public MainImageWidget // 没有线程
{
    Q_OBJECT
public:
    explicit CameraImageWidget(QWidget *parent = nullptr);
    ~CameraImageWidget();
    bool isValid() {return valid;}

    boost::shared_ptr<MediaStreamProc> getDecoder() {return decoder;}

signals:

//public slots:
//    void setFocusStatus(bool focus) {focusStatus = focus;}

protected:
    virtual void paintEvent(QPaintEvent *event) override;
//    virtual void mouseReleaseEvent(QMouseEvent *ev) override;

private slots:
    void drawImage();

private:
    QImage image;
    MediaFrame_AI_Info aiInfo;
    bool refreshAiInfo;
    boost::shared_ptr<MediaStreamProc> decoder;
    bool valid;
};

#endif // IMAGEWIDGET_H
