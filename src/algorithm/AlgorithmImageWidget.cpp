//
// Created by jianghualuo on 18-4-9.
//

#include "AlgorithmImageWidget.h"
#include <QPainter>

AlgorithmImageWidget::AlgorithmImageWidget(QWidget *parent) :
        QWidget(parent) {
//    pix = QPixmap("/home/jianghualuo/work/data/images/cloud.jpg");
}

AlgorithmImageWidget::~AlgorithmImageWidget() {

}

void AlgorithmImageWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    if (!pix.isNull()) {
        painter.drawPixmap(rect(), pix.scaledToWidth(width()));
    }
}

void AlgorithmImageWidget::setPix(QPixmap pix) {
    this->pix = pix;
    update();
}
