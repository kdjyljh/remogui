#include "waterfallitem.h"
#include "HttpProxy.h"
#include "WaterFallImageItem.h"
#include "WaterFallVideoItem.h"
#include <QPainter>
#include <QMessageBox>
#include <glog/logging.h>

WaterFallItem::WaterFallItem(std::string url, QWidget *parent) :
        QWidget(parent),
        imageUrl(url),
        kDefaultWidth(100),
        isHovering(false),
        hoveringRectLength(3),
        videoIcon(new QImage(":/icon/videoplayer.png")),
        image(nullptr) {
    setToolTip(QString::fromLocal8Bit(url.data()));
    setCursor(Qt::PointingHandCursor);
}

void WaterFallItem::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QRect r = isHovering ? rect() :
              rect().adjusted(hoveringRectLength, hoveringRectLength,
                              -hoveringRectLength, -hoveringRectLength);
    painter.drawImage(r, *image);
    if (imageUrl.find(".MP4") != std::string::npos) {
        painter.drawImage(r, *videoIcon);
    }

    setVisible(true);
}

bool WaterFallItem::loadImage() {
    std::vector<uint8_t> buff;
    std::string url = "http://192.168.0.1//app/";
    int pos = imageUrl.find('.');
    std::string path = imageUrl.substr(0, imageUrl.find('.')) + ".THM";
    url += path;
    if (!HttpProxy::getRequest(url, buff, "keep-alive")) {
        return false;
    }
    image = new QImage();
    image->loadFromData(buff.data(), buff.size());
//    image->scaledToWidth(kDefaultWidth, Qt::FastTransformation);
    return true;
}

QSize WaterFallItem::getPixSize() {
    return image->size();
}

void WaterFallItem::enterEvent(QEvent *event) {
    isHovering = true;
    update();
}

void WaterFallItem::leaveEvent(QEvent *event) {
    isHovering = false;
    update();
}

WaterFallItem::~WaterFallItem() {
    delete image;
    image = nullptr;
}

void WaterFallItem::mousePressEvent(QMouseEvent *event) {
    int pos = imageUrl.find('.') + 1;
    std::string type = imageUrl.substr(pos, imageUrl.size() - pos);
    if (type == "JPG") {
        WaterFallImageItem item(imageUrl);
        if (item.isValid()) {
            item.exec();
        } else {
            QMessageBox::warning(nullptr, "网络错误", "无视频流", QMessageBox::Ok);
        }
    } else {
        WaterFallVideoItem item(imageUrl);
        if (item.isValid()) {
            if (item.getDuration() > 0 && item.getDuration() <= 1000) {
                QMessageBox::warning(nullptr, "警告", "文件太短，无法播放", QMessageBox::Ok);
                return;
            }
            item.exec();
        } else {
            QMessageBox::warning(nullptr, "网络错误", "无视频流", QMessageBox::Ok);
        }
    }
}


