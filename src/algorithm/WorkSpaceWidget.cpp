//
// Created by jianghualuo on 18-3-22.
//

#include "WorkSpaceWidget.h"
#include "NetImage.hpp"
#include <QPainter>

WorkSpaceWidget::~WorkSpaceWidget() {

}

WorkSpaceWidget::WorkSpaceWidget(QWidget *parent) :
        QWidget(parent) {
    pix = QPixmap("/home/jianghualuo/work/data/images/cloud.jpg");
}

void WorkSpaceWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.drawPixmap(rect(), pix.scaledToWidth(width()));
}
