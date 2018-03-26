#include "waterfallscrollarea.h"
#include "../mainwindow.h"
#include <QScrollBar>

WaterFallScrollArea::WaterFallScrollArea(QWidget *parent) :
    QScrollArea(parent),
    contentWidget(new WaterFallWidget)
{
    setGeometry(QRect(MainWindow::centerPoint, QSize(800, 600)));
    setWidget(contentWidget);
    contentWidget->resize(size());

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    connect(verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(onVerticalScrollBarChanged(int)));
    connect(this, SIGNAL(reloadImages(int)), contentWidget, SLOT(getImages(int)));
    connect(contentWidget, SIGNAL(allImageGot()), this, SLOT(setBottomReach()));
}

void WaterFallScrollArea::resizeEvent(QResizeEvent *event)
{
    contentWidget->resize(size());
}

void WaterFallScrollArea::onVerticalScrollBarChanged(int value)
{
    if (!bottomReached) {
        if (value >= verticalScrollBar()->maximum() - 5) {
            int scaledValue = verticalScrollBar()->maximum() / 5.0;
            verticalScrollBar()->setValue(value - scaledValue);
            emit reloadImages(10);
        }
    }
}

WaterFallScrollArea::~WaterFallScrollArea() {
    delete contentWidget;
}

bool WaterFallScrollArea::reloadImages() {
    bottomReached = false;
    return contentWidget->reInit();
}
