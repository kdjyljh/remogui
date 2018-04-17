//
// Created by jianghualuo on 18-4-16.
//

#ifndef REMO_GUI_ALGORITHMGIMBALDIALOG_H
#define REMO_GUI_ALGORITHMGIMBALDIALOG_H

#include <QWidget>
#include <remo_gui_autogen/include/ui_algorithm.h>
#include "ui_algorithmgimbal.h"

class AlgorithmGimbalDialog : public QWidget {
Q_OBJECT
public:
    AlgorithmGimbalDialog(QWidget *parent = nullptr);
    ~AlgorithmGimbalDialog();

private slots:
    void on_dial_roll_sliderReleased();

private:
    Ui::AlgorithmGimbal *ui;
};


#endif //REMO_GUI_ALGORITHMGIMBALDIALOG_H
