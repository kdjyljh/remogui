#ifndef WATERFALLWIDGET_H
#define WATERFALLWIDGET_H

#include <QWidget>
#include <QList>
#include "waterfallitem.h"

#define kItemColumnSize 4
class WaterFallWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WaterFallWidget(QWidget *parent = nullptr);

signals:

public slots:
    void getImages();

protected:
    virtual void paintEvent(QPaintEvent *event) override;

private:
    const int kHSpace;
    const int kVSpace;
    const int kLeftSpace;
    const int kRightSpace;
    QList<WaterFallItem*> listItemColumn[kItemColumnSize];
};

#endif // WATERFALLWIDGET_H
