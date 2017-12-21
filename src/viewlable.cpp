#include "viewlable.h"
#include "protocoldataInterfaceimpl.h"
#include "cmddef.h"
#include "camerafielddef.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

ViewLable::ViewLable(QWidget *parent) :
    QLabel(parent),
    focusStatus(false),
    recPos(-1000, -1000),
    recSize(100, 100)
{

}

void ViewLable::mouseReleaseEvent(QMouseEvent *ev)
{
//    qDebug() << ev->pos();
    if (!focusStatus) return;

    recPos = ev->pos();
    Remo_Camera_PosPoint_s pointNormal{static_cast<float>(recPos.x() / size().width()), static_cast<float>((recPos.y()) / size().height())};
    ProtocolDataInterfaceImpl::sendCmdCamera(Remo_CmdId_Camera_Set_PosArea,
        std::vector<uint8_t>{reinterpret_cast<uint8_t*>(&pointNormal), reinterpret_cast<uint8_t*>((&pointNormal) + sizeof(pointNormal))});
}

void ViewLable::paintEvent(QPaintEvent * ev)
{
    QLabel::paintEvent(ev);

    if (!focusStatus) return;

    QPainter * painter = new QPainter(this);
    painter->setPen(QPen(Qt::red, Qt::SolidLine));
    painter->setBrush(QBrush(Qt::red, Qt::NoBrush));
    painter->drawRect(QRect(recPos - QPoint(recSize.width() / 2, recSize.height() / 2), recSize));
    delete painter;
}
