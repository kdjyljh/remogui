#ifndef VIEWLABLE_H
#define VIEWLABLE_H
#include <QLabel>
class ViewLable : public QLabel
{
Q_OBJECT
public:
    ViewLable(QWidget *parent = 0);
    virtual ~ViewLable(){}

public slots:
    void setFocusStatus(bool focus) {focusStatus = focus;}

protected:
    virtual void mouseReleaseEvent(QMouseEvent *ev) override;
    virtual void paintEvent(QPaintEvent * ev) override;

private:
    bool focusStatus;
    QPoint recPos;
    const QSize recSize;
};

#endif // VIEWLABLE_H
