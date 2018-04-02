//
// Created by jianghualuo on 18-3-26.
//

#ifndef REMO_GUI_PROGRESSBARSLIDER_H
#define REMO_GUI_PROGRESSBARSLIDER_H

#include <QSlider>
#include <QWidget>

class ProgressBarSlider : public QSlider {
    Q_OBJECT
public:
    ProgressBarSlider(QWidget *parent = nullptr);
    void setValueWithDragging(int value);
protected:
    virtual void mousePressEvent(QMouseEvent *ev) override ;
    virtual void mouseReleaseEvent(QMouseEvent *ev) override ;

private:
    bool progressBarIsDragging;
};


#endif //REMO_GUI_PROGRESSBARSLIDER_H
