//
// Created by jianghualuo on 18-3-22.
//

#include "WorkSpaceWidget.h"
#include <QPainter>
#include <QMouseEvent>
#include "AlgorithmManager.h"
#include "AlgorithmDialog.h"

WorkSpaceWidget::~WorkSpaceWidget() {

}

WorkSpaceWidget::WorkSpaceWidget(QWidget *parent) :
        netImage(boost::make_shared<NetImage>()),
        isManuallyChosingPerson(false),
        QWidget(parent) {
    pix = QPixmap("/home/jianghualuo/work/data/images/cloud.jpg");
    connect(netImage.get(), SIGNAL(sender_image(QPixmap)), this, SLOT(updatePix(QPixmap)));
}

void WorkSpaceWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.drawPixmap(rect(), pix.scaledToWidth(width()));
}

void WorkSpaceWidget::updatePix(QPixmap pix) {
    this->pix = pix;
    update();
}

void WorkSpaceWidget::mouseReleaseEvent(QMouseEvent *ev) {
    if (!isManuallyChosingPerson) return;
    qDebug() << ev->pos();
    qDebug() << size();
    QPoint recPos = ev->pos();
    double posX = static_cast<double>(recPos.x()) / size().width();
    double posY = static_cast<double>(recPos.y()) / size().height();

    AlgoParamMsg msg =  AlgorithmManager::generateMsgByType(AlgoParam::MsgUnity::SelectionSet);
    msg.mutable_selection_set()->set_select_mode(AlgoParam::SelectionSet::SELECTION_USERPICK);
    msg.mutable_selection_set()->add_userpick_point(posX);
    msg.mutable_selection_set()->add_userpick_point(posY);
    msg.mutable_selection_set()->set_is_selecting(true);
    AlgorithmDialog::getInstance()->asyncSendMsg(msg);
//    chosingPersonMouseReleased(posX, posY);
}

void WorkSpaceWidget::setManuallyChosingPerson(bool status) {
    isManuallyChosingPerson = status;
    if (isManuallyChosingPerson) {
        setCursor(Qt::CrossCursor);
    } else {
        setCursor(Qt::ArrowCursor);
    }
}
