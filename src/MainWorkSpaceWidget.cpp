//
// Created by jianghualuo on 18-4-21.
//

#include "MainWorkSpaceWidget.h"

MainWorkSpaceWidget::MainWorkSpaceWidget(QWidget *parent) :
        algorithmWidget(AlgorithmDialog::getInstance()),
        cameraImageWidget(boost::make_shared<CameraImageWidget>()),
        QWidget(parent) {
    setLayout(new QVBoxLayout(this));
    layout()->addWidget(algorithmWidget.get());
    algorithmWidget->resize(size());
//    algorithmWidget->setStyleSheet("background-color:red;");
    layout()->setContentsMargins(0, 0, 0, 0);
    algorithmWidget->init();
}

MainWorkSpaceWidget::~MainWorkSpaceWidget() {

}
