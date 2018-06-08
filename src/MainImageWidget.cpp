//
// Created by jianghualuo on 5/18/18.
//

#include "MainImageWidget.h"
#include "protocoldataInterfaceimpl.h"
#include "algorithmprotodef.h"
#include <glog/logging.h>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>

MainImageWidget::MainImageWidget(QWidget *parent) :
        QWidget(parent),
        focusStatus(false),
        recPos(-1000, -1000),
        recSize(100, 100),
        isManuallyChosingPerson(false) {
}

MainImageWidget::~MainImageWidget() {

}

void MainImageWidget::mouseReleaseEvent(QMouseEvent *ev) {
    if (isManuallyChosingPerson) {
//        qDebug() << ev->pos();
//        qDebug() << size();
        QPoint recPos = ev->pos();
        double posX = static_cast<double>(recPos.x()) / size().width();
        double posY = static_cast<double>(recPos.y()) / size().height();
        std::vector<uint8_t> data(8);
        memcpy(data.data(), &posX, 4);
        memcpy(data.data() + 4, &posY, 4);
        ProtocolDataInterface::sendCmdAlgorithm(APP_SELTARGET_SETBY_POINT, data);
    }

    if (focusStatus) {
        recPos = ev->pos();
        Remo_Camera_PosPoint_s pointNormal{static_cast<float>(recPos.x()) / size().width(),
                                           static_cast<float>(recPos.y()) / size().height()};
        ProtocolDataInterfaceImpl::sendCmdCamera(Remo_CmdId_Camera_Set_PosArea,
                                                 std::vector<uint8_t>(reinterpret_cast<uint8_t *>(&pointNormal),
                                                                      reinterpret_cast<uint8_t *>(&pointNormal) + sizeof(pointNormal)));
    }
}

void MainImageWidget::setManuallyChosingPerson(bool status) {
    LOG(INFO) << "MainImageWidget::setManuallyChosingPerson";
    isManuallyChosingPerson = status;
    if (isManuallyChosingPerson) {
        setCursor(Qt::CrossCursor);
    } else {
        setCursor(Qt::ArrowCursor);
    }
}

void MainImageWidget::paintEvent(QPaintEvent *event) {

}
