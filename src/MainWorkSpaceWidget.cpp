//
// Created by jianghualuo on 18-4-21.
//

#include "MainWorkSpaceWidget.h"
#include <QMessageBox>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <glog/logging.h>
#include "CameraImageWidget.h"
#include "WorkSpaceWidget.h"

MainWorkSpaceWidget::MainWorkSpaceWidget(QWidget *parent) :
        QWidget(parent),
        ProtocolDataInterfaceImpl(DispatcheType_Algorithm),
        ui(new Ui::Algorithm){
//    setLayout(new QVBoxLayout(this));
//    layout()->setContentsMargins(0, 0, 0, 0);
    ui->setupUi(this);
    resize(1300, 900);
    ui->tabWidget->tabBar()->setMinimumWidth(800);

}

MainWorkSpaceWidget::~MainWorkSpaceWidget() {

}

bool MainWorkSpaceWidget::init() {
//    algorithmWidget = boost::make_shared<AlgorithmDialog>();
//    layout()->addWidget(algorithmWidget.get());
//    algorithmWidget->resize(size());

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

    delete ui->workSpaceWidget;
    ui->verticalLayout->addWidget(imageWidget.get());

    return true;
}

void MainWorkSpaceWidget::on_groupBox_person_chose_clicked(bool checked) {
    LOG(INFO) << "MainWorkSpaceWidget::on_groupBox_person_chose_clicked ui->workSpaceWidget:" << ui->workSpaceWidget;
    imageWidget->setManuallyChosingPerson(checked);

    std::vector<uint8_t> data(1);
    uint8_t tempData = checked;
    memcpy(data.data(), &tempData, 1);
    ProtocolDataInterface::sendCmdAlgorithm(APP_SELTARGET_SET_SELECTION, data);

//    //当处于正在选人状态时才需要结束选人
//    if (!checked && manager->status.selection_set().is_selecting()) {
//        AlgoParamMsg msg =  AlgorithmManager::generateMsgByType(AlgoParam::MsgUnity::SelectionSet);
//        msg.mutable_selection_set()->set_is_selecting(false);
//        //user_pick需要用户点击确认结束。
//        if (manager->status.selection_set().select_mode() == AlgoParam::SelectionSet::SELECTION_USERPICK) {
//            msg.mutable_selection_set()->set_select_mode(AlgoParam::SelectionSet::SELECTION_USERPICK);
//        } else {
//            //剩下的mutil，master和auto选人会自动结束
//            // 但是用户在还未结束时点击结束选人,则会取消当前选人并且进入auto选人
//            msg.mutable_selection_set()->set_select_mode(AlgoParam::SelectionSet::SELECTION_AUTO);
//            msg.mutable_selection_set()->set_is_selecting(true);
//        }
//        asyncSendMsg(msg);
//    }
}

void MainWorkSpaceWidget::handle() {
}
