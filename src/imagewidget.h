#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>
#include <QImage>
#include "vaapidecoder.h"

class ImageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ImageWidget(QWidget *parent = nullptr);
    ~ImageWidget();
    bool isValid() {return valid;}

    boost::shared_ptr<VaapiDecoder> getDecoder() {return decoder;}

signals:

public slots:
    void setFocusStatus(bool focus) {focusStatus = focus;}

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *ev) override;

private slots:
    void drawImage();

private:
    QImage image;
    boost::shared_ptr<VaapiDecoder> decoder;
    bool focusStatus;
    QPoint recPos;
    const QSize recSize;
    bool valid;
};

#endif // IMAGEWIDGET_H
