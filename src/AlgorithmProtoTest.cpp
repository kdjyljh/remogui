//
// Created by jianghualuo on 6/6/18.
//

#include "AlgorithmProtoTest.h"
#include <QDesktopWidget>

std::map<std::string, std::string> cmd2Str {
        {"ZoomModeAllowedRange" + APP_FUNC_SET_ZOOMMODE_Mode_NONE, "NONE"},
        {"ZoomModeAllowedRange" + APP_FUNC_SET_ZOOMMODE_Mode_AUTO, "AUTO"},
        {"ZoomModeAllowedRange" + APP_FUNC_SET_ZOOMMODE_Mode_NORMAL, "NORMAL"},
        {"ZoomModeAllowedRange" + APP_FUNC_SET_ZOOMMODE_Mode_RANDOM, "RANDOM"},
        {"AutoScaleAllowedRange" + APP_FUNC_SET_ZOOMMODE_Scale_3, "3"},
        {"AutoScaleAllowedRange" + APP_FUNC_SET_ZOOMMODE_Scale_5, "5"},
        {"AutoScaleAllowedRange" + APP_FUNC_SET_ZOOMMODE_Scale_7, "7"},
        {"AutoScaleAllowedRange" + APP_FUNC_SET_ZOOMMODE_Scale_10, "10"},
        {"AutoScaleAllowedRange" + APP_FUNC_SET_ZOOMMODE_Scale_12, "12"},
        {"AutoScaleAllowedRange" + APP_FUNC_SET_ZOOMMODE_Scale_15, "15"}
};

AlgorithmProtoTest::AlgorithmProtoTest(QWidget *parent) :
        QWidget(parent),
        ProtocolDataInterfaceImpl(DispatcheType_Algorithm),
        ui(new Ui::AlgorithmProtoTest) {
    ui->setupUi(this);
    setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, size(),
                                    qApp->desktop()->availableGeometry()));
    aiStatus = {0};

    ui->comboBox_SDKFun_XOffset->addItem("1/3", QVariant(APP_FUNC_SET_XOFFSET_Offset_1Divided3));
    ui->comboBox_SDKFun_XOffset->addItem("5/12", QVariant(APP_FUNC_SET_XOFFSET_Offset_5Divided12));
    ui->comboBox_SDKFun_XOffset->addItem("1/2", QVariant(APP_FUNC_SET_XOFFSET_Offset_1Divided2));
    ui->comboBox_SDKFun_XOffset->addItem("7/12", QVariant(APP_FUNC_SET_XOFFSET_Offset_7Divided12));
    ui->comboBox_SDKFun_XOffset->addItem("2/3", QVariant(APP_FUNC_SET_XOFFSET_Offset_2Divided3));

    ui->comboBox_Selection_Default->addItem("Default", QVariant(APP_SELTARGET_SETBY_DEFAULT_Default));
    ui->comboBox_Selection_Default->addItem("Master", QVariant(APP_SELTARGET_SETBY_DEFAULT_Master));
    ui->comboBox_Selection_Default->addItem("Multiple", QVariant(APP_SELTARGET_SETBY_DEFAULT_Multiple));

    ui->comboBox_SDKFun_SpeedMode->addItem("SLOW", APP_FUNC_SET_SPEEDMODE_Mode_SLOW);
    ui->comboBox_SDKFun_SpeedMode->addItem("NORMAL", APP_FUNC_SET_SPEEDMODE_Mode_NORMAL);
    ui->comboBox_SDKFun_SpeedMode->addItem("FAST", APP_FUNC_SET_SPEEDMODE_Mode_FAST);

//    connect(ui->comboBox_SDKFun_ZommMode_Scale, SIGNAL(activated(int index)), this, SLOT(comboBox__activated(int index)));
    connect(ui->comboBox_SDKFun_ZommMode_Mode, SIGNAL(activated(int)), this, SLOT(comboBox_activated(int)));
    connect(ui->comboBox_SDKFun_SpeedMode, SIGNAL(activated(int)), this, SLOT(comboBox_activated(int)));
    connect(ui->comboBox_SDKFun_XOffset, SIGNAL(activated(int)), this, SLOT(comboBox_activated(int)));
    connect(ui->comboBox_Selection_Default, SIGNAL(activated(int)), this, SLOT(comboBox_activated(int)));

    connect(ui->checkBox_SDKFun_CapSA, SIGNAL(clicked(bool)), this, SLOT(checkBox_SDKFun_Cfg_clicked(bool)));
    connect(ui->checkBox_SDKFun_CapSF, SIGNAL(clicked(bool)), this, SLOT(checkBox_SDKFun_Cfg_clicked(bool)));
    connect(ui->checkBox_SDKFun_CapSL, SIGNAL(clicked(bool)), this, SLOT(checkBox_SDKFun_Cfg_clicked(bool)));
    connect(ui->checkBox_SDKFun_Capture, SIGNAL(clicked(bool)), this, SLOT(checkBox_SDKFun_Cfg_clicked(bool)));
    connect(ui->checkBox_SDKFun_FindMasterInit, SIGNAL(clicked(bool)), this, SLOT(checkBox_SDKFun_Cfg_clicked(bool)));
    connect(ui->checkBox_SDKFun_GimbalLock, SIGNAL(clicked(bool)), this, SLOT(checkBox_SDKFun_Cfg_clicked(bool)));
    connect(ui->checkBox_SDKFun_Handpose, SIGNAL(clicked(bool)), this, SLOT(checkBox_SDKFun_Cfg_clicked(bool)));
    connect(ui->checkBox_SDKFun_SDKEnable, SIGNAL(clicked(bool)), this, SLOT(checkBox_SDKFun_Cfg_clicked(bool)));
}

AlgorithmProtoTest::~AlgorithmProtoTest() {

}

void AlgorithmProtoTest::handle() {
    if (!content.isSync) {
        QString str = QString::fromLocal8Bit("上一条命令执行结果:") + QString::number(content.ret);
        LOG(INFO) << str.toStdString();
        ui->label_lastCmdError->setText(str);
    }

    if (content.cmdId == SDKSTATUS_PUSH_APP) {
        updateStatus();
    }
}

void AlgorithmProtoTest::updateStatus() {
    aiStatus.SDKSTATUS_PUSH_APP_Enable = content.custom[0];
    aiStatus.SDKSTATUS_PUSH_APP_Selection = content.custom[1];
    aiStatus.SDKSTATUS_PUSH_APP_GimbalLock = content.custom[2];
    aiStatus.SDKSTATUS_PUSH_APP_Capture = content.custom[3];
    aiStatus.SDKSTATUS_PUSH_APP_Capture_sf = content.custom[4];
    aiStatus.SDKSTATUS_PUSH_APP_Capture_sa = content.custom[5];
    aiStatus.SDKSTATUS_PUSH_APP_Capture_sl = content.custom[6];
    aiStatus.SDKSTATUS_PUSH_APP_Handpose = content.custom[7];
    aiStatus.SDKSTATUS_PUSH_APP_FindMasterInit = content.custom[8];
    aiStatus.SDKSTATUS_PUSH_APP_SpeedMode = content.custom[9];
    aiStatus.SDKSTATUS_PUSH_APP_XOffset = content.custom[10];
    aiStatus.SDKSTATUS_PUSH_APP_ZoomMode = content.custom[11];
    aiStatus.SDKSTATUS_PUSH_APP_AutoScale = content.custom[12];
    aiStatus.SDKSTATUS_PUSH_APP_ActionType = content.custom[13];
    aiStatus.SDKSTATUS_PUSH_APP_ZoomCtrlType = content.custom[14];
    aiStatus.SDKSTATUS_PUSH_APP_RollCtrlType = content.custom[15];
    aiStatus.SDKSTATUS_PUSH_APP_OnceZoomScale = content.custom[16];
    aiStatus.SDKSTATUS_PUSH_APP_CbZoomSpeed = content.custom[17];
    aiStatus.SDKSTATUS_PUSH_APP_CbCycleZoomPrd = content.custom[18];
    aiStatus.SDKSTATUS_PUSH_APP_CbShotScale = content.custom[19];
    aiStatus.SDKSTATUS_PUSH_APP_CbBiasRollDegree = content.custom[20];
    aiStatus.SDKSTATUS_PUSH_APP_CbCycleBiasRollPrd = content.custom[21];
    aiStatus.SDKSTATUS_PUSH_APP_CbScanScale = content.custom[22];
    aiStatus.SDKSTATUS_PUSH_APP_CbScanSpeed = content.custom[23];
    aiStatus.SDKSTATUS_PUSH_APP_CbSwitchSpeed = content.custom[24];
    aiStatus.SDKSTATUS_PUSH_APP_XOffsetAllowed = content.custom[25];

    int i = 25;
    for (auto &it : aiStatus.SDKSTATUS_PUSH_APP_ZoomModeAllowedRange) it = content.custom[++i];
    for (auto &it : aiStatus.SDKSTATUS_PUSH_APP_AutoScaleAllowedRange) it = content.custom[++i];
    for (auto &it : aiStatus.SDKSTATUS_PUSH_APP_ActionTypeAllowedRange) it = content.custom[++i];
    for (auto &it : aiStatus.SDKSTATUS_PUSH_APP_ZoomCtrlTypeAllowedRange) it = content.custom[++i];
    for (auto &it : aiStatus.SDKSTATUS_PUSH_APP_RollCtrlTypeAllowedRange) it = content.custom[++i];
    for (auto &it : aiStatus.SDKSTATUS_PUSH_APP_OnceZoomScaleAllowedRange) it = content.custom[++i];

    std::stringstream statusStr;
    statusStr  << std::setfill(' ') << std::left;
    int fillSpaceSize = 20;
    int fillSpaceSize1 = 10;
    statusStr << std::setw(fillSpaceSize) << "SDKEnable:" << std::setw(fillSpaceSize1) << aiStatus.SDKSTATUS_PUSH_APP_Enable;
    statusStr << std::setw(fillSpaceSize) << "Selection:" << std::setw(fillSpaceSize1) << aiStatus.SDKSTATUS_PUSH_APP_Selection;
    statusStr << std::setw(fillSpaceSize) << "gimbalLock:" << std::setw(fillSpaceSize1) << aiStatus.SDKSTATUS_PUSH_APP_GimbalLock;
    statusStr << std::setw(fillSpaceSize) << "capture:" << std::setw(fillSpaceSize1) << aiStatus.SDKSTATUS_PUSH_APP_Capture;
    statusStr << std::setw(fillSpaceSize) << "capture_sf:" << std::setw(fillSpaceSize1) << aiStatus.SDKSTATUS_PUSH_APP_Capture_sf;
    statusStr << std::setw(fillSpaceSize) << "capture_sa:" << std::setw(fillSpaceSize1) << aiStatus.SDKSTATUS_PUSH_APP_Capture_sa;
    statusStr << std::setw(fillSpaceSize) << "capture_sl:" << std::setw(fillSpaceSize1) << aiStatus.SDKSTATUS_PUSH_APP_Capture_sl;
    statusStr << std::setw(fillSpaceSize) << "handpose:" << std::setw(fillSpaceSize1) << aiStatus.SDKSTATUS_PUSH_APP_Handpose;
    statusStr << std::setw(fillSpaceSize) << "findMasterInit:" << std::setw(fillSpaceSize1) << aiStatus.SDKSTATUS_PUSH_APP_FindMasterInit;
    statusStr << std::setw(fillSpaceSize) << "speedMode:" << std::setw(fillSpaceSize1) << aiStatus.SDKSTATUS_PUSH_APP_SpeedMode;
    statusStr << std::setw(fillSpaceSize) << "xOffset:" << std::setw(fillSpaceSize1) << aiStatus.SDKSTATUS_PUSH_APP_XOffset;
    statusStr << std::setw(fillSpaceSize) << "zoomMode:" << std::setw(fillSpaceSize1) << aiStatus.SDKSTATUS_PUSH_APP_ZoomMode;
    statusStr << std::setw(fillSpaceSize) << "autoScale:" << std::setw(fillSpaceSize1) << aiStatus.SDKSTATUS_PUSH_APP_AutoScale;
    statusStr << std::setw(fillSpaceSize) << "actionType:" << std::setw(fillSpaceSize1) << aiStatus.SDKSTATUS_PUSH_APP_ActionType;
    statusStr << std::setw(fillSpaceSize) << "zoomCtrlType:" << std::setw(fillSpaceSize1) << aiStatus.SDKSTATUS_PUSH_APP_ZoomCtrlType;
    statusStr << std::setw(fillSpaceSize) << "rollCtrlType:" << std::setw(fillSpaceSize1) << aiStatus.SDKSTATUS_PUSH_APP_RollCtrlType;
    statusStr << std::setw(fillSpaceSize) << "onceZoomScale:" << std::setw(fillSpaceSize1) << aiStatus.SDKSTATUS_PUSH_APP_OnceZoomScale;
    statusStr << std::setw(fillSpaceSize) << "cbZoomSpeed:" << std::setw(fillSpaceSize1) << aiStatus.SDKSTATUS_PUSH_APP_CbZoomSpeed;
    statusStr << std::setw(fillSpaceSize) << "cbCycleZoomPrd:" << std::setw(fillSpaceSize1) << aiStatus.SDKSTATUS_PUSH_APP_CbCycleZoomPrd;
    statusStr << std::setw(fillSpaceSize) << "cbShotScale:" << std::setw(fillSpaceSize1) << aiStatus.SDKSTATUS_PUSH_APP_CbShotScale;
    statusStr << std::setw(fillSpaceSize) << "cbBiasRollDegree:" << std::setw(fillSpaceSize1) << aiStatus.SDKSTATUS_PUSH_APP_CbBiasRollDegree;
    statusStr << std::setw(fillSpaceSize) << "cbCycleBiasRollPrd:" << std::setw(fillSpaceSize1) << aiStatus.SDKSTATUS_PUSH_APP_CbCycleBiasRollPrd;
    statusStr << std::setw(fillSpaceSize) << "cbScanScale:" << std::setw(fillSpaceSize1) << aiStatus.SDKSTATUS_PUSH_APP_CbScanScale;
    statusStr << std::setw(fillSpaceSize) << "cbScanSpeed:" << std::setw(fillSpaceSize1) << aiStatus.SDKSTATUS_PUSH_APP_CbScanSpeed;
    statusStr << std::setw(fillSpaceSize) << "cbSwitchSpeed:" << std::setw(fillSpaceSize1) << aiStatus.SDKSTATUS_PUSH_APP_CbSwitchSpeed;
    statusStr << std::setw(fillSpaceSize) << "xOffsetAllowed:" << std::setw(fillSpaceSize1) << aiStatus.SDKSTATUS_PUSH_APP_XOffsetAllowed;

    ui->textEdit_SDK_STATUS->setFont(QFont("Monospace")); //等宽字体
    ui->textEdit_SDK_STATUS->setText(QString::fromStdString(statusStr.str()));

    ui->checkBox_Selection_Set->setChecked(aiStatus.SDKSTATUS_PUSH_APP_Selection);

    ui->comboBox_SDKFun_ZommMode_Mode->clear();
    ui->comboBox_SDKFun_ZommMode_Scale->clear();
    for (auto it : aiStatus.SDKSTATUS_PUSH_APP_ZoomModeAllowedRange) {
        if (0xff != it)
            ui->comboBox_SDKFun_ZommMode_Mode->addItem(
                    QString::fromStdString(cmd2Str.find("ZoomModeAllowedRange" + it)->second),
                    QVariant(it));
    }
    for (auto it : aiStatus.SDKSTATUS_PUSH_APP_AutoScaleAllowedRange) {
        if (0xff != it)
            ui->comboBox_SDKFun_ZommMode_Scale->addItem(
                    QString::fromStdString(cmd2Str.find("AutoScaleAllowedRange" + it)->second),
                    QVariant(it));
    }

#define set_cur_combobox_by_itemData(item, field) \
    for (int i = 0; i < ui->item->count(); ++i) { \
        if (aiStatus.field == ui->item->itemData(i).toInt()) {\
            ui->item->setCurrentIndex(i);\
        }\
    }

    set_cur_combobox_by_itemData(comboBox_SDKFun_ZommMode_Scale, SDKSTATUS_PUSH_APP_AutoScale)
    set_cur_combobox_by_itemData(comboBox_SDKFun_ZommMode_Mode, SDKSTATUS_PUSH_APP_ZoomMode)
    set_cur_combobox_by_itemData(comboBox_SDKFun_XOffset, SDKSTATUS_PUSH_APP_XOffset)
    set_cur_combobox_by_itemData(comboBox_SDKFun_SpeedMode, SDKSTATUS_PUSH_APP_SpeedMode)
#undef set_cur_combobox_by_itemData

//    uint8_t SDKEnableData = (uint8_t)aiStatus.SDKSTATUS_PUSH_APP_Enable;
//    ui->checkBox_SDKFun_SDKEnable->setChecked(SDKEnableData&((uint8_t)1<<0));
//    ui->checkBox_SDKFun_Capture->setChecked(SDKEnableData&((uint8_t)1<<1));
//    ui->checkBox_SDKFun_CapSF->setChecked(SDKEnableData&((uint8_t)1<<2));
//    ui->checkBox_SDKFun_CapSA->setChecked(SDKEnableData&((uint8_t)1<<3));
//    ui->checkBox_SDKFun_CapSL->setChecked(SDKEnableData&((uint8_t)1<<4));
//    ui->checkBox_SDKFun_Handpose->setChecked(SDKEnableData&((uint8_t)1<<5));
//    ui->checkBox_SDKFun_FindMasterInit->setChecked(SDKEnableData&((uint8_t)1<<6));
//    ui->checkBox_SDKFun_GimbalLock->setChecked(SDKEnableData&((uint8_t)1<<7));

    uint8_t SDKEnableData = (uint8_t)aiStatus.SDKSTATUS_PUSH_APP_Enable;
    ui->checkBox_SDKFun_SDKEnable->setChecked((bool)aiStatus.SDKSTATUS_PUSH_APP_Enable);
    ui->checkBox_SDKFun_Capture->setChecked((bool)aiStatus.SDKSTATUS_PUSH_APP_Capture);
    ui->checkBox_SDKFun_CapSF->setChecked((bool)aiStatus.SDKSTATUS_PUSH_APP_Capture_sf);
    ui->checkBox_SDKFun_CapSA->setChecked((bool)aiStatus.SDKSTATUS_PUSH_APP_Capture_sa);
    ui->checkBox_SDKFun_CapSL->setChecked((bool)aiStatus.SDKSTATUS_PUSH_APP_Capture_sl);
    ui->checkBox_SDKFun_Handpose->setChecked((bool)aiStatus.SDKSTATUS_PUSH_APP_Handpose);
    ui->checkBox_SDKFun_FindMasterInit->setChecked((bool)aiStatus.SDKSTATUS_PUSH_APP_FindMasterInit);
    ui->checkBox_SDKFun_GimbalLock->setChecked((bool)aiStatus.SDKSTATUS_PUSH_APP_GimbalLock);

    ui->comboBox_SDKFun_XOffset->setEnabled(aiStatus.SDKSTATUS_PUSH_APP_XOffsetAllowed);
}

void AlgorithmProtoTest::on_checkBox_Selection_Set_clicked(bool checked) {
    sendCmdAlgorithm(APP_SELTARGET_SET_SELECTION, std::vector<uint8_t>{checked});
}

void AlgorithmProtoTest::on_lineEdit_Selection_Point_returnPressed() {
    QStringList strList =  ui->lineEdit_Selection_Point->text().split(',', QString::SkipEmptyParts);

    std::vector<uint8_t> data(8);
    for (auto it : strList) {
        it = it.trimmed();
    }

    if (strList.size() >= 2) {
        bool ok = false;
        float x = strList[0].toFloat(&ok);
        float y = strList[1].toFloat(&ok);
        if (!ok) {
            LOG(INFO) << "AlgorithmProtoTest::on_lineEdit_Selection_Point_returnPressed not a float: ";
            return;
        }
        std::vector<uint8_t> data(8);
        memcpy(data.data(), &x, 4); memcpy(data.data() + 4, &y, 4);
        sendCmdAlgorithm(APP_SELTARGET_SETBY_POINT, data);
    } else {
        LOG(INFO) << "AlgorithmProtoTest::on_lineEdit_Selection_Point_returnPressed strList size:" << strList.size() << " not right";
    }
}

void AlgorithmProtoTest::comboBox_activated(int index) {
    QComboBox *comboBox = dynamic_cast<QComboBox*>(sender());
    std::vector<uint8_t> data(1);
    int indexData = comboBox->itemData(index).toInt();
    memcpy(data.data(), &indexData, 1);

    if (comboBox == ui->comboBox_SDKFun_XOffset) {
        sendCmdAlgorithm(APP_FUNC_SET_XOFFSET, data);
    } else if (comboBox == ui->comboBox_Selection_Default) {
        sendCmdAlgorithm(APP_SELTARGET_SETBY_DEFAULT, data);
    } else if (comboBox == ui->comboBox_SDKFun_SpeedMode) {
        sendCmdAlgorithm(APP_FUNC_SET_SPEEDMODE, data);
    } else if (comboBox == ui->comboBox_SDKFun_ZommMode_Mode) {
        data.resize(2);
        if (indexData == APP_FUNC_SET_ZOOMMODE_Mode_AUTO) {
            int scaleValue = ui->comboBox_SDKFun_ZommMode_Scale->itemData(index).toInt();
            memcpy(data.data() + 1, &scaleValue, 1);
        }
        sendCmdAlgorithm(APP_FUNC_SET_ZOOMMODE, data);
    } else {

    }
}

void AlgorithmProtoTest::checkBox_SDKFun_Cfg_clicked(bool checked) {
//    uint8_t data = 0;
//    data = ui->checkBox_SDKFun_SDKEnable->isChecked() ? data|((uint8_t)1<<0) : data&(~((uint8_t)1<<0));
//    data = ui->checkBox_SDKFun_Capture->isChecked() ? data|((uint8_t)1<<1) : data&(~((uint8_t)1<<1));
//    data = ui->checkBox_SDKFun_CapSF->isChecked() ? data|((uint8_t)1<<2) : data&(~((uint8_t)1<<2));
//    data = ui->checkBox_SDKFun_CapSA->isChecked() ? data|((uint8_t)1<<3) : data&(~((uint8_t)1<<3));
//    data = ui->checkBox_SDKFun_CapSL->isChecked() ? data|((uint8_t)1<<4) : data&(~((uint8_t)1<<4));
//    data = ui->checkBox_SDKFun_Handpose->isChecked() ? data|((uint8_t)5<<0) : data&(~((uint8_t)5<<0));
//    data = ui->checkBox_SDKFun_FindMasterInit->isChecked() ? data|((uint8_t)1<<6) : data&(~((uint8_t)1<<6));
//    data = ui->checkBox_SDKFun_GimbalLock->isChecked() ? data|((uint8_t)1<<7) : data&(~((uint8_t)1<<7));

    uint8_t cmdId = 0;
    uint8_t enable = 0;
    QCheckBox *ptr = dynamic_cast<QCheckBox*>(sender());
    if (ptr == ui->checkBox_SDKFun_GimbalLock) {
        cmdId = APP_FUNC_SET_SDK_CFG_Cfg_GimbalLock;
        enable = ui->checkBox_SDKFun_GimbalLock->isChecked();
    } else if (ptr == ui->checkBox_SDKFun_FindMasterInit) {
        cmdId = APP_FUNC_SET_SDK_CFG_Cfg_FindMasterInit;
        enable = ui->checkBox_SDKFun_FindMasterInit->isChecked();
    } else if (ptr == ui->checkBox_SDKFun_Capture) {
        cmdId = APP_FUNC_SET_SDK_CFG_Cfg_Capture;
        enable = ui->checkBox_SDKFun_Capture->isChecked();
    } else if (ptr == ui->checkBox_SDKFun_SDKEnable) {
        cmdId = APP_FUNC_SET_SDK_CFG_Cfg_SDKEnable;
        enable = ui->checkBox_SDKFun_SDKEnable->isChecked();
    } else if (ptr == ui->checkBox_SDKFun_Handpose) {
        cmdId = APP_FUNC_SET_SDK_CFG_Cfg_Handpose;
        enable = ui->checkBox_SDKFun_Handpose->isChecked();
    } else if (ptr == ui->checkBox_SDKFun_CapSL) {
        cmdId = APP_FUNC_SET_SDK_CFG_Cfg_CapSL;
        enable = ui->checkBox_SDKFun_CapSL->isChecked();
    } else if (ptr == ui->checkBox_SDKFun_CapSA) {
        cmdId = APP_FUNC_SET_SDK_CFG_Cfg_CapSA;
        enable = ui->checkBox_SDKFun_CapSA->isChecked();
    } else if (ptr == ui->checkBox_SDKFun_CapSF) {
        cmdId = APP_FUNC_SET_SDK_CFG_Cfg_CapSF;
        enable = ui->checkBox_SDKFun_CapSF->isChecked();
    }

    std::vector<uint8_t> data(2);
    memcpy(data.data(), &cmdId, 1);
    memcpy(data.data() + 1, &enable, 1);
    sendCmdAlgorithm(APP_FUNC_SET_SDK_CFG, data);
}


