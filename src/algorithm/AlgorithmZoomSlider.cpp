//
// Created by jianghualuo on 18-4-16.
//

#include "AlgorithmZoomSlider.h"
#include <QMouseEvent>
#include <QStyle>
#include <QDesktopWidget>
#include <QApplication>
#include <QDebug>
#include <QLayout>

AlgorithmZoomSlider::AlgorithmZoomSlider(QWidget *parent) :
        QSlider(parent) {
    setRange(10, 350);
    setOrientation(Qt::Horizontal);
    setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, QSize(300, 50),
                                    qApp->desktop()->availableGeometry()));
}

AlgorithmZoomSlider::~AlgorithmZoomSlider() {

}

void AlgorithmZoomSlider::mousePressEvent(QMouseEvent *ev) {
    //注意应先调用父类的鼠标点击处理事件，这样可以不影响拖动的情况
    QSlider::mousePressEvent(ev);
    if (ev->button() == Qt::LeftButton) {
        //获取鼠标的位置，这里并不能直接从ev中取值（因为如果是拖动的话，鼠标开始点击的位置没有意义了）
        double pos = ev->pos().x() / (double)width();
        if (orientation() == Qt::Vertical)
            pos = ev->pos().x() / (double)height();
        setValue(pos * (maximum() - minimum()) + minimum());
    }
}

void AlgorithmZoomSlider::mouseReleaseEvent(QMouseEvent *ev) {
    QSlider::mouseReleaseEvent(ev);
    if (ev->button() == Qt::LeftButton) {
        ev->accept();
    }
    qDebug() << value();
}
