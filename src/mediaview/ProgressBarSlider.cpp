//
// Created by jianghualuo on 18-3-26.
//
#include <QMouseEvent>
#include "ProgressBarSlider.h"

void ProgressBarSlider::mousePressEvent(QMouseEvent *ev) {
    if (ev->button() == Qt::LeftButton)
    {
        progressBarIsDragging = true;
        if (orientation() == Qt::Vertical)
            setValue(minimum() + ((maximum()-minimum()) * (height()-ev->y())) / height() ) ;
        else
            setValue(minimum() + ((maximum()-minimum()) * ev->x()) / width() ) ;

        ev->accept();
    }
    QSlider::mousePressEvent(ev);
}

ProgressBarSlider::ProgressBarSlider(QWidget *parent) :
        progressBarIsDragging(false),
        QSlider(parent) {

}

void ProgressBarSlider::mouseReleaseEvent(QMouseEvent *ev) {
    if (ev->button() == Qt::LeftButton) {
        progressBarIsDragging = false;
        ev->accept();
    }

    QSlider::mouseReleaseEvent(ev);
}

void ProgressBarSlider::setValueWithDragging(int value) {
    if (progressBarIsDragging) return;
    setValue(value);
}
