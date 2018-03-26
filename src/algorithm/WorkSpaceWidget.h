//
// Created by jianghualuo on 18-3-22.
//

#ifndef REMO_GUI_WORKSPACEWIDGET_H
#define REMO_GUI_WORKSPACEWIDGET_H

#include <QWidget>
#include <QPixmap>

class WorkSpaceWidget : public QWidget {
    Q_OBJECT
public:
    WorkSpaceWidget(QWidget *parent = nullptr);
    ~WorkSpaceWidget();

protected:
    virtual void paintEvent(QPaintEvent *event) override;
private:
    QPixmap pix;
};


#endif //REMO_GUI_WORKSPACEWIDGET_H
