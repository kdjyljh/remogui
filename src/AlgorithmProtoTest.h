//
// Created by jianghualuo on 6/6/18.
//

#ifndef REMO_GUI_ALGORITHMPROTOTEST_H
#define REMO_GUI_ALGORITHMPROTOTEST_H

#include "ui_algorithmprototest.h"
#include "protocoldataInterfaceimpl.h"

class AlgorithmProtoTest :public QWidget, public ProtocolDataInterfaceImpl {
    Q_OBJECT
public:
    AlgorithmProtoTest(QWidget *parent = nullptr);
    virtual ~AlgorithmProtoTest();

    virtual void handle() override ;

private:
    void updateStatus();


private slots:
    void on_checkBox_Selection_Set_clicked(bool checked);
    void on_lineEdit_Selection_Point_returnPressed();

private slots:
    void comboBox_activated(int index);
    void checkBox_SDKFun_Cfg_clicked(bool checked);

private:
    Ui::AlgorithmProtoTest *ui;
    SDKSTATUS_PUSH_APP_S aiStatus;
};


#endif //REMO_GUI_ALGORITHMPROTOTEST_H
