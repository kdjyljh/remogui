#include "waterfallwidget.h"
#include <QPixmapCache>
#include <QPainter>
#include <QLabel>

WaterFallWidget::WaterFallWidget(QWidget *parent) :
    QWidget(parent),
    kHSpace(5),
    kVSpace(5),
    kLeftSpace(10),
    kRightSpace(10)
{
    for (int i = 0; i < 100; ++i) {
        WaterFallItem *item = new WaterFallItem("/home/jianghualuo/work/data/images/cloud.jpg", this);
        item->setGeometry(200, 200, 200, 200);
        listItemColumn[0].append(item);
    }
}

void WaterFallWidget::getImages()
{

}

void WaterFallWidget::paintEvent(QPaintEvent *event)
{
}
