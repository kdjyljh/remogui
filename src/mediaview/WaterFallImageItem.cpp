//
// Created by jianghualuo on 18-3-16.
//

#include "WaterFallImageItem.h"
#include "HttpProxy.h"
#include <QPainter>

const int DEFAULT_WIDTH = 1000;
WaterFallImageItem::WaterFallImageItem(std::string url, QWidget *parent) :
        QDialog(parent),
        imageValid(false),
        imageUrl(url) {
    std::string baseUrl = "http://192.168.0.1//app/";
    std::vector<uint8_t> buff;
    if (HttpProxy::getRequest(baseUrl + imageUrl, buff, "keep-alive")) {
        image = QImage();
        if (image.loadFromData(buff.data(), buff.size())) {
            image = image.scaledToWidth(DEFAULT_WIDTH);
            resize(image.size());
            imageValid = true;
        }
    }
    setWindowTitle(QString::fromStdString(url));
}

WaterFallImageItem::~WaterFallImageItem() {
}

void WaterFallImageItem::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.drawImage(rect(), image);
    setVisible(true);
}
