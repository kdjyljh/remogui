#ifndef WATERFALLWIDGET_H
#define WATERFALLWIDGET_H

#include <QWidget>
#include <list>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "waterfallitem.h"

#define kItemColumnSize 4

class WaterFallWidget : public QWidget {
Q_OBJECT
public:
    explicit WaterFallWidget(QWidget *parent = nullptr);

signals:
    void allImageGot();

public slots:
    void getImages(int num);
    bool reInit();

protected:
    virtual void paintEvent(QPaintEvent *event) override;

    virtual void resizeEvent(QResizeEvent *event) override;

private:
    int getItemWidth();
    void setFixedHeightSafely();
    void getThmUrls();
    int calculateMaxHeight();

private:
    const int kHSpace;
    const int kVSpace;
    const int kLeftSpace;
    const int kRightSpace;
    const int kTopSpace;
    const int kBottomSpace;
    const int kItemInterHSpace;
    const int kItemInterVSpace;
    int thumbnailUrlIndex;
    bool valid;
    std::vector<boost::shared_ptr<WaterFallItem>> itemListColumn[kItemColumnSize];
    std::vector<std::string> thumbnailUrlVec;
    int curColumnHeights[kItemColumnSize];
};

#endif // WATERFALLWIDGET_H
