#ifndef WATERFALLITEM_H
#define WATERFALLITEM_H

#include <QWidget>

class WaterFallItem : public QWidget
{
    Q_OBJECT
public:
    explicit WaterFallItem(QString url, QWidget *parent = nullptr);

protected:
    virtual void paintEvent(QPaintEvent *event) override;

signals:

public slots:

private:
    void loadImage();

private:
    QString imageUrl;
    QPixmap *pix;
};

#endif // WATERFALLITEM_H
