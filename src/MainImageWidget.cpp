//
// Created by jianghualuo on 5/18/18.
//

#include "MainImageWidget.h"
#include "AlgorithmDialog.h"
#include "protocoldataInterfaceimpl.h"
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
        qDebug() << ev->pos();
        qDebug() << size();
        QPoint recPos = ev->pos();
        double posX = static_cast<double>(recPos.x()) / size().width();
        double posY = static_cast<double>(recPos.y()) / size().height();

        AlgoParamMsg msg = AlgorithmManager::generateMsgByType(AlgoParam::MsgUnity::SelectionSet);
        msg.mutable_selection_set()->set_select_mode(AlgoParam::SelectionSet::SELECTION_USERPICK);
        msg.mutable_selection_set()->add_userpick_point(posX);
        msg.mutable_selection_set()->add_userpick_point(posY);
        msg.mutable_selection_set()->set_is_selecting(true);
        AlgorithmDialog::getInstance()->asyncSendMsg(msg);
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
    isManuallyChosingPerson = status;
    if (isManuallyChosingPerson) {
        setCursor(Qt::CrossCursor);
    } else {
        setCursor(Qt::ArrowCursor);
    }
}

void MainImageWidget::paintEvent(QPaintEvent *event) {

}
