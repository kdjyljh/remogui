//
// Created by jianghualuo on 5/18/18.
//

#ifndef REMO_GUI_MAINIMAGEWIDGET_H
#define REMO_GUI_MAINIMAGEWIDGET_H

#include <QWidget>

class MainImageWidget : public QWidget {
    Q_OBJECT
public:
    MainImageWidget(QWidget *parent = nullptr);
    ~MainImageWidget();
    void setManuallyChosingPerson(bool status);

//signals:
//    void chosingPersonMouseReleased(double x, double y);

public slots:
    void setFocusStatus(bool focus) {focusStatus = focus;}

protected:
    virtual void mouseReleaseEvent(QMouseEvent *ev) override;
    virtual void paintEvent(QPaintEvent *event) override;

protected:
    bool focusStatus;
    QPoint recPos;
    const QSize recSize;

private:
    bool isManuallyChosingPerson;
};


#endif //REMO_GUI_MAINIMAGEWIDGET_H
