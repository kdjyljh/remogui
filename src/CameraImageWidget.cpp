#include "CameraImageWidget.h"
#include "cmddef.h"
#include "protocoldataInterfaceimpl.h"
#include "camerafielddef.h"
#include "mainwindow.h"
#include <QPainter>
#include <QPaintEvent>
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QDesktopWidget>
#include <glog/logging.h>

CameraImageWidget::CameraImageWidget(QWidget *parent) :
        MainImageWidget(parent),
        valid(false),
        refreshAiInfo(false),
        decoder(boost::shared_ptr<MediaStreamProc>(new MediaStreamProc)) {
    LOG(INFO) << "CameraImageWidget::CameraImageWidget constructor this:" << this;
    connect(decoder.get(), SIGNAL(imageGot()), this, SLOT(drawImage()), Qt::BlockingQueuedConnection);
//    connect(decoder.get(), SIGNAL(readStreamDone(bool)), this, SLOT());
}

void CameraImageWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if (!image.isNull()) {
        image = image.scaledToWidth(width());
//        resize(image.size());
//        setFixedSize(image.size());
//        setFixedHeight(image.height());
//        image = image.scaled(size(), Qt::KeepAspectRatio);
        MainWindow::getWindInstace()->setFixedHeight(MainWindow::getWindInstace()->height() - height() + image.height());
        painter.drawImage(image.rect(), image);
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
        for (int i = 0; i < aiInfo.u8NumPerson; ++i) {
            float x1 = (*((float*) ((char *)aiInfo.u8PersonRois + i * 16)));
            float y1 = (*((float*) ((char *)aiInfo.u8PersonRois + i * 16 + 4)));
            float x2 = (*((float*) ((char *)aiInfo.u8PersonRois + i * 16 + 8)));
            float y2 = (*((float*) ((char *)aiInfo.u8PersonRois + i * 16 + 12)));
//            LOG(INFO) << x1 << ", " << y1 << ", "<< x2 << ", " << y2;
            ret = QRect(QPoint(width() * x1, height() * y1),
                        QPoint(width() * x2, height() * y2));
            painter.drawRect(ret);
        }
        pen.setColor(Qt::green); //face使用绿色
        painter.setPen(pen);
        for (int i = 0; i < aiInfo.u8NumFace; ++i) {
            ret = QRect(QPoint(width() * (*((float*) ((char *)aiInfo.u8FaceRois + i * 16))), height() * (*((float*) ((char *)aiInfo.u8FaceRois + i * 16 + 4)))),
                        QPoint(width() * (*((float*) ((char *)aiInfo.u8FaceRois + i * 16 + 8))), height() * (*((float*) ((char *)aiInfo.u8FaceRois + i * 16 + 12)))));
            painter.drawRect(ret);
        }
        pen.setColor(Qt::white); //target使用白色
        painter.setPen(pen);
        for (int i = 0; i < aiInfo.u8NumTarget; ++i) {
            float x1 = (*((float*) ((char *)aiInfo.u8TargetRois + 2 + i * 18)));
            float y1 = (*((float*) ((char *)aiInfo.u8TargetRois + 2 + i * 18 + 4)));
            float x2 = (*((float*) ((char *)aiInfo.u8TargetRois + 2 + i * 18 + 8)));
            float y2 = (*((float*) ((char *)aiInfo.u8TargetRois + 2 + i * 18 + 12)));
//            LOG(INFO) << x1 << ", " << y1 << ", "<< x2 << ", " << y2;
            ret = QRect(QPoint(width() * x1, height() * y1),
                        QPoint(width() * x2, height() * y2));
            painter.drawRect(ret);
        }
        pen.setColor(Qt::blue); //hand使用蓝色
        painter.setPen(pen);
        for (int i = 0; i < aiInfo.u8NumHand; ++i) {
            ret = QRect(QPoint(width() * (*((float*) ((char *)aiInfo.u8Hand + i * 16))), height() * (*((float*) ((char *)aiInfo.u8Hand + i * 16 + 4)))),
                        QPoint(width() * (*((float*) ((char *)aiInfo.u8Hand + i * 16 + 8))), height() * (*((float*) ((char *)aiInfo.u8Hand + i * 16 + 12)))));
            painter.drawRect(ret);
        }

        painter.setPen(QColor(0, 160, 230, 150)); //当前手势使用透明蓝色
        painter.setPen(pen);
        QFont font; font.setPointSize(16); painter.setFont(font);
        painter.drawText(50, 50, QString("当前手势: (type:%1,count:%2)").arg(aiInfo.u8HpType).arg(aiInfo.u8HpCount));

//        pen.setColor(Qt::blue); //face使用蓝色
//        painter.setPen(pen);
//        for (int i = 0; i < aiInfo.faceNum; ++i) {
//            ret = QRect(QPoint(width() * aiInfo.faceROIs[i][0], height() * aiInfo.faceROIs[i][1]),
//                        QPoint(width() * aiInfo.faceROIs[i][2], height() * aiInfo.faceROIs[i][3]));
//            painter.drawRect(ret);
//        }
//
//        pen.setColor(Qt::green); //hand使用绿色
//        painter.setPen(pen);
//        for (int i = 0; i < aiInfo.handNum; ++i) {
//            ret = QRect(QPoint(width() * aiInfo.handROIs[i][0], height() * aiInfo.handROIs[i][1]),
//                        QPoint(width() * aiInfo.handROIs[i][2], height() * aiInfo.handROIs[i][3]));
//            painter.drawRect(ret);
//        }
//
//        pen.setColor(Qt::white); //target使用白色
//        painter.setPen(pen);
//        for (int i = 0; i < aiInfo.targetNum; ++i) {
//            ret = QRect(QPoint(width() * aiInfo.targetROIs[i][0], height() * aiInfo.targetROIs[i][1]),
//                        QPoint(width() * aiInfo.targetROIs[i][2], height() * aiInfo.targetROIs[i][3]));
//            painter.drawRect(ret);
//        }
    }
}

void CameraImageWidget::drawImage()
{
    AVFrame *frame = &decoder->getCurFrame()->image;

    if (nullptr != frame) {
        if ((refreshAiInfo = decoder->getCurFrame()->hasAiInfo)) {
            aiInfo = decoder->getCurFrame()->ai_info;
        }
//        this->image =
//                QImage(frame->data[0], frame->width, frame->height, QImage::Format_RGB32);//和解码线程共享frame数据，数据由解码线程管理
        image = QImage(frame->data[0], frame->width, frame->height, QImage::Format_RGB32).copy();
        repaint();
    }
}

//void CameraImageWidget::mouseReleaseEvent(QMouseEvent *ev)
//{
//    if (!focusStatus) return;
//
//    recPos = ev->pos();
//    Remo_Camera_PosPoint_s pointNormal{static_cast<float>(recPos.x()) / size().width(), static_cast<float>(recPos.y()) / size().height()};
//    ProtocolDataInterfaceImpl::sendCmdCamera(Remo_CmdId_Camera_Set_PosArea,
//                                             std::vector<uint8_t>(reinterpret_cast<uint8_t*>(&pointNormal), reinterpret_cast<uint8_t*>(&pointNormal) + sizeof(pointNormal)));
//}

CameraImageWidget::~CameraImageWidget() {
}
