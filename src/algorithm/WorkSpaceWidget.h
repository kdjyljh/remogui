//
// Created by jianghualuo on 18-3-22.
//

#ifndef REMO_GUI_WORKSPACEWIDGET_H
#define REMO_GUI_WORKSPACEWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include "NetImage.hpp"

class WorkSpaceWidget : public QWidget {
    Q_OBJECT
public:
    explicit WorkSpaceWidget(QWidget *parent = nullptr);
    ~WorkSpaceWidget();
    void setManuallyChosingPerson(bool status);

signals:
    void chosingPersonMouseReleased(double x, double y);

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *ev) override;

private slots:
    void updatePix(QPixmap pix);

private:
    QPixmap pix;
    boost::shared_ptr<NetImage> netImage;
    bool isManuallyChosingPerson;
};


#endif //REMO_GUI_WORKSPACEWIDGET_H
