#include "waterfallitem.h"
#include <QPainter>

WaterFallItem::WaterFallItem(QString url, QWidget *parent) :
    QWidget(parent),
    imageUrl(url)
{
    loadImage();
}

void WaterFallItem::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), *pix);
}

void WaterFallItem::loadImage()
{
    pix = new QPixmap(imageUrl);
}
