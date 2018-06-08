//
// Created by jianghualuo on 18-4-21.
//

#ifndef REMO_GUI_MainWorkSpaceWidget_H
#define REMO_GUI_MainWorkSpaceWidget_H

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include "MainImageWidget.h"
#include "protocoldataInterfaceimpl.h"
#include "ui_algorithm.h"

class MainWorkSpaceWidget : public QWidget, public ProtocolDataInterfaceImpl {
    Q_OBJECT
public:
    MainWorkSpaceWidget(QWidget *parent = nullptr);
    virtual ~MainWorkSpaceWidget();

    bool init();
    boost::shared_ptr<MainImageWidget> getImageWidget() {return imageWidget;}

    virtual void handle() override ;

private slots:
    void on_groupBox_person_chose_clicked(bool checked);

private:
    Ui::Algorithm *ui;
    boost::shared_ptr<MainImageWidget> imageWidget;
    std::string imageSource;
};


#endif //REMO_GUI_MAINWORKSPACE_H
