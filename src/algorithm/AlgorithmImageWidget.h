//
// Created by jianghualuo on 18-4-9.
//

#ifndef REMO_GUI_IMAGEWIDGET_H
#define REMO_GUI_IMAGEWIDGET_H

#include <QWidget>
#include <QPixmap>

class AlgorithmImageWidget : public QWidget {
    Q_OBJECT
public:
    explicit AlgorithmImageWidget(QWidget *parent = nullptr);
    ~AlgorithmImageWidget();
    void setPix(QPixmap pix);

protected:
    virtual void paintEvent(QPaintEvent *event) override;

private:
    QPixmap pix;
};


#endif //REMO_GUI_IMAGEWIDGET_H
