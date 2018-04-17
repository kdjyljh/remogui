//
// Created by jianghualuo on 18-4-16.
//

#ifndef REMO_GUI_ALGORITHMSLIDER_H
#define REMO_GUI_ALGORITHMSLIDER_H

#include <QSlider>

class AlgorithmZoomSlider : public QSlider {
    Q_OBJECT
public:
    explicit AlgorithmZoomSlider(QWidget *parent = nullptr);
    ~AlgorithmZoomSlider();

protected:
    virtual void mousePressEvent(QMouseEvent *ev) override ;
    virtual void mouseReleaseEvent(QMouseEvent *ev) override ;
};


#endif //REMO_GUI_ALGORITHMSLIDER_H
