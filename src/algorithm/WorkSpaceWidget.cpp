//
// Created by jianghualuo on 18-3-22.
//

#include "WorkSpaceWidget.h"
#include <QPainter>
#include <QMouseEvent>
#include "AlgorithmManager.h"

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
    qDebug() << ev->pos();
    qDebug() << size();
    if (!isManuallyChosingPerson) return;

    QPoint recPos = ev->pos();
    double posX = static_cast<double>(recPos.x()) / size().width();
    double posY = static_cast<double>(recPos.y()) / size().height();

    chosingPersonMouseReleased(posX, posY);
}
