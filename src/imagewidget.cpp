#include "imagewidget.h"
#include "cmddef.h"
#include "protocoldataInterfaceimpl.h"
#include "camerafielddef.h"
#include <QPainter>
#include <QPaintEvent>
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <glog/logging.h>

ImageWidget::ImageWidget(QWidget *parent) :
    QWidget(parent),
    focusStatus(false),
    valid(false),
    recPos(-1000, -1000),
    recSize(100, 100),
    decoder(boost::shared_ptr<VaapiDecoder>(new VaapiDecoder))
{
    valid = !decoder->isValid();
    connect(decoder.get(), SIGNAL(imageGot()), this, SLOT(drawImage()), Qt::BlockingQueuedConnection);
}

void ImageWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if (!image.isNull()) {
        image = image.scaledToWidth(width());
        painter.drawImage(rect(), image);
    }

    if (!focusStatus) return;
    painter.setPen(QPen(Qt::red, Qt::SolidLine));
    painter.setBrush(QBrush(Qt::red, Qt::NoBrush));
    painter.drawRect(QRect(recPos - QPoint(recSize.width() / 2, recSize.height() / 2), recSize));
}

void ImageWidget::drawImage()
{
    AVFrame *frame = decoder->getCurFrame();
    if (NULL != frame) {
        this->image =
                QImage(frame->data[0], frame->width, frame->height, QImage::Format_RGB32);//和解码线程共享frame数据，数据由解码线程管理
        repaint();
    }
}

void ImageWidget::mouseReleaseEvent(QMouseEvent *ev)
{
    qDebug() << ev->pos();
    qDebug() << size();
    if (!focusStatus) return;

    recPos = ev->pos();
    Remo_Camera_PosPoint_s pointNormal{static_cast<float>(recPos.x()) / size().width(), static_cast<float>(recPos.y()) / size().height()};
    ProtocolDataInterfaceImpl::sendCmdCamera(Remo_CmdId_Camera_Set_PosArea,
                                             std::vector<uint8_t>(reinterpret_cast<uint8_t*>(&pointNormal), reinterpret_cast<uint8_t*>(&pointNormal) + sizeof(pointNormal)));
}

ImageWidget::~ImageWidget() {
}
