#ifndef WATERFALLSCROLLAREA_H
#define WATERFALLSCROLLAREA_H

#include <QScrollArea>
#include "waterfallwidget.h"

class WaterFallScrollArea : public QScrollArea {
Q_OBJECT
public:
    WaterFallScrollArea(QWidget *parent = nullptr);

    ~WaterFallScrollArea();
    bool reloadImages();

signals:
    void reloadImages(int num);

protected:
    //重载size事件，设置添加到widget和scrollarea一样大，否则widget太小看不见
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    WaterFallWidget *contentWidget;
    bool bottomReached;

private slots:
    void setBottomReach() {bottomReached = true;};
    void onVerticalScrollBarChanged(int value);
};

#endif // WATERFALLSCROLLAREA_H
