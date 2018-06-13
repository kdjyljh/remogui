//
// Created by jianghualuo on 18-4-21.
//

#include "MainWorkSpaceWidget.h"
#include <QMessageBox>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <glog/logging.h>
#include "CameraImageWidget.h"

MainWorkSpaceWidget::MainWorkSpaceWidget(QWidget *parent) :
        QWidget(parent) {
    setLayout(new QVBoxLayout(this));
    layout()->setContentsMargins(0, 0, 0, 0);
}

MainWorkSpaceWidget::~MainWorkSpaceWidget() {

}

bool MainWorkSpaceWidget::init() {
    algorithmWidget = AlgorithmDialog::getInstance();
    layout()->addWidget(algorithmWidget.get());
    algorithmWidget->resize(size());
    algorithmWidget->init();

    try {
        boost::property_tree::ptree root;
        boost::property_tree::read_json("remo_gui.json", root);
        imageSource = root.get<std::string>("VideoSource");
    } catch (boost::property_tree::ptree_error &e) {
        LOG(INFO) << "MainImageWidget::MainImageWidget json parse error:" << e.what();
        return false;
    }

    if (imageSource == "Algorithm") {
        imageWidget = boost::make_shared<WorkSpaceWidget>();
    } else if (imageSource == "Camera"){
        imageWidget = boost::make_shared<CameraImageWidget>();
    }

    algorithmWidget->changeWorkSpace(imageWidget);

    return true;
}