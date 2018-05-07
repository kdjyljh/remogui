#include "CameraImageWidget.h"
#include "cmddef.h"
#include "protocoldataInterfaceimpl.h"
#include "camerafielddef.h"
#include <QPainter>
#include <QPaintEvent>
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <glog/logging.h>

CameraImageWidget::CameraImageWidget(QWidget *parent) :
    QWidget(parent),
    focusStatus(false),
    valid(false),
    recPos(-1000, -1000),
    recSize(100, 100),
    refreshAiInfo(false),
    decoder(boost::shared_ptr<MediaStreamProc>(new MediaStreamProc))
{
    LOG(INFO) << "CameraImageWidget::CameraImageWidget constructor";
    connect(decoder.get(), SIGNAL(imageGot()), this, SLOT(drawImage()), Qt::BlockingQueuedConnection);
//    connect(decoder.get(), SIGNAL(readStreamDone(bool)), this, SLOT());
}

void CameraImageWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if (!image.isNull()) {
        image = image.scaledToWidth(width());
        painter.drawImage(rect(), image);
    }

    QPen pen(Qt::red, Qt::SolidLine);

    if (focusStatus) {
        painter.setPen(pen);
        painter.setBrush(QBrush(Qt::red, Qt::NoBrush));
        painter.drawRect(QRect(recPos - QPoint(recSize.width() / 2, recSize.height() / 2), recSize));
    }

    if (refreshAiInfo) {
        QRect ret;
        pen.setColor(Qt::red); //body使用红色
        pen.setWidth(5);
        painter.setPen(pen);
        for (int i = 0; i < aiInfo.bodyNum; ++i) {
            ret = QRect(QPoint(width() * aiInfo.bodyROIs[i][0], height() * aiInfo.bodyROIs[i][1]),
                        QPoint(width() * aiInfo.bodyROIs[i][2], height() * aiInfo.bodyROIs[i][3]));
            painter.drawRect(ret);
        }

        pen.setColor(Qt::blue); //face使用蓝色
        painter.setPen(pen);
        for (int i = 0; i < aiInfo.faceNum; ++i) {
            ret = QRect(QPoint(width() * aiInfo.faceROIs[i][0], height() * aiInfo.faceROIs[i][1]),
                        QPoint(width() * aiInfo.faceROIs[i][2], height() * aiInfo.faceROIs[i][3]));
            painter.drawRect(ret);
        }

        pen.setColor(Qt::green); //hand使用绿色
        painter.setPen(pen);
        for (int i = 0; i < aiInfo.handNum; ++i) {
            ret = QRect(QPoint(width() * aiInfo.handROIs[i][0], height() * aiInfo.handROIs[i][1]),
                        QPoint(width() * aiInfo.handROIs[i][2], height() * aiInfo.handROIs[i][3]));
            painter.drawRect(ret);
        }

        pen.setColor(Qt::white); //target使用绿色
        painter.setPen(pen);
        for (int i = 0; i < aiInfo.targetNum; ++i) {
            ret = QRect(QPoint(width() * aiInfo.targetROIs[i][0], height() * aiInfo.targetROIs[i][1]),
                        QPoint(width() * aiInfo.targetROIs[i][2], height() * aiInfo.targetROIs[i][3]));
            painter.drawRect(ret);
        }
    }
}

void CameraImageWidget::drawImage()
{
    AVFrame *frame = &decoder->getCurFrame()->image;

    if (nullptr != frame) {
        if (refreshAiInfo = decoder->getCurFrame()->hasAiInfo) {
            aiInfo = decoder->getCurFrame()->ai_info;
        }
        this->image =
                QImage(frame->data[0], frame->width, frame->height, QImage::Format_RGB32);//和解码线程共享frame数据，数据由解码线程管理
        repaint();
    }
}

void CameraImageWidget::mouseReleaseEvent(QMouseEvent *ev)
{
    if (!focusStatus) return;

    recPos = ev->pos();
    Remo_Camera_PosPoint_s pointNormal{static_cast<float>(recPos.x()) / size().width(), static_cast<float>(recPos.y()) / size().height()};
    ProtocolDataInterfaceImpl::sendCmdCamera(Remo_CmdId_Camera_Set_PosArea,
                                             std::vector<uint8_t>(reinterpret_cast<uint8_t*>(&pointNormal), reinterpret_cast<uint8_t*>(&pointNormal) + sizeof(pointNormal)));
}

CameraImageWidget::~CameraImageWidget() {
}
