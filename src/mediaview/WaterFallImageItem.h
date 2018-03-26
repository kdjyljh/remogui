//
// Created by jianghualuo on 18-3-16.
//

#ifndef REMO_GUI_WATERFALLIMAGEITEM_H
#define REMO_GUI_WATERFALLIMAGEITEM_H

#include <QWidget>
#include <QDialog>
#include <QImage>
#include <string>
class WaterFallImageItem : public QDialog {
    Q_OBJECT
public:
    explicit WaterFallImageItem(std::string url, QWidget *parent = nullptr);
    ~WaterFallImageItem();

    bool isValid() {return imageValid;}

protected:
    virtual void paintEvent(QPaintEvent *event) override;

private:
    std::string imageUrl;
    bool imageValid;
    QImage image;
};


#endif //REMO_GUI_WATERFALLIMAGEITEM_H
