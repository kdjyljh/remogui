//
// Created by jianghualuo on 18-4-16.
//

#include "AlgorithmGimbalDialog.h"
#include <glog/logging.h>

AlgorithmGimbalDialog::AlgorithmGimbalDialog(QWidget *parent) :
        ui(new Ui::AlgorithmGimbal),
        QWidget(parent) {
    ui->setupUi(this);
    ui->dial_roll->setRange(0, 1000);
    ui->dial_roll->setWrapping(true);
}

AlgorithmGimbalDialog::~AlgorithmGimbalDialog() {

}

void AlgorithmGimbalDialog::on_dial_roll_sliderReleased() {
    LOG(INFO) << ui->dial_roll->value();
}
