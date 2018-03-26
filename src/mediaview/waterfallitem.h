#ifndef WATERFALLITEM_H
#define WATERFALLITEM_H

#include <QWidget>
#include <QImage>
#include <string>
class WaterFallItem : public QWidget
{
    Q_OBJECT
public:
    explicit WaterFallItem(std::string url, QWidget *parent = nullptr);
    ~WaterFallItem();
    QSize getPixSize();
    bool loadImage();
    std::string getImageUrl() { return imageUrl;}

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void enterEvent(QEvent *event) override ;
    virtual void leaveEvent(QEvent *event) override ;
    virtual void mousePressEvent(QMouseEvent *event) override ;

signals:

public slots:

private slots:

private:
    std::string imageUrl;
    QImage *image;
    QImage *videoIcon;
    int kDefaultWidth;
    bool isHovering;
    int hoveringRectLength;
};

#endif // WATERFALLITEM_H
