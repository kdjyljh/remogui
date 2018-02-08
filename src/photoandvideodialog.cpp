#include "photoandvideodialog.h"
#include "itemdatadef.h"

#include <QDebug>
#include <QButtonGroup>
#include <QtConcurrent>

static const unsigned DEFAULT_DIALOG_WITH = 600;
static const unsigned DEFAULT_DIALOG_HEIGHT = 550;

PhotoAndVideoDialog::PhotoAndVideoDialog(QWidget *parent) :
    QDialog(parent),
    ProtocolDataInterfaceImpl(),
    ui(new Ui::PhotoAndVideoDialog),
    photoMsgDialog(new QMessageBox()),
    recordeMsgDialog(new QMessageBox()),
    button_stop_recod(nullptr),
    button_stop_photo(nullptr),
    isRecording(false),
    isPhotoing(false),
    initCompleted(false),
    stopCapRec_Reverse(true),
    waitMsgBox(new QMessageBox())
{
    LOG(INFO) << "PhotoAndVideoDialog::PhotoAndVideoDialog construct";
    ui->setupUi(this);
    recordeMsgDialog->setStandardButtons(0);
    photoMsgDialog->setStandardButtons(0);
    ui->pushButton_Photo->setEnabled(false);
    ui->pushButton_Record->setEnabled(false);
    waitMsgBox->setWindowTitle(QString::fromLocal8Bit("等待"));
    waitMsgBox->setText(QString::fromLocal8Bit("请等待"));

    photoMsgDialog->setWindowTitle(QString::fromLocal8Bit("提示"));

    setFixedSize(DEFAULT_DIALOG_WITH, DEFAULT_DIALOG_HEIGHT);
    QButtonGroup * buttGroup = new QButtonGroup(this);
    buttGroup->addButton(ui->radioButton_SubWcorkMode_Recode_LapseRec);
//    buttGroup->addButton(ui->radioButton_SubWcorkMode_Recode_Photo);
    buttGroup->addButton(ui->radioButton_SubWcorkMode_Recode_SlowMotion);
    buttGroup->addButton(ui->radioButton_SubWorkMode_Recode_Loop);
    buttGroup->addButton(ui->radioButton_SubWorkMode_Recode_Normal);
    buttGroup->addButton(ui->radioButton_SubWorkMode_MultiPhoto_Continue);
    buttGroup->addButton(ui->radioButton_SubWorkMode_MultiPhoto_Lapse);
    buttGroup->addButton(ui->radioButton_SubWorkMode_MultiPhoto_Panorama);
    buttGroup->addButton(ui->radioButton_SubWorkMode_MultiPhoto_Burst);
//    buttGroup->addButton(ui->radioButton_SubWorkMode_MultiPhoto_AEB);
    buttGroup->addButton(ui->radioButton_SubWorkMode_Photo_Delay);
    buttGroup->addButton(ui->radioButton_SubWorkMode_Photo_Single);

    connect(this, SIGNAL(photoDelayTickTack(QString)), this, SLOT(set_photoMsgDialog_text(QString)));

//    initSurportRange();
    sendCmdCamera(Remo_CmdId_Camera_Get_WorkMode);

    addItem2Map(ui->ComboBox_CapScene, Remo_CmdId_Camera_Get_CapScene);
    addItem2Map(ui->ComboBox_ImageResolution, Remo_CmdId_Camera_Get_ImageResolution);
    addItem2Map(ui->ComboBox_LapseCapInterval, Remo_CmdId_Camera_Get_LapseCap_Interval);
    addItem2Map(ui->ComboBox_LapseCapTotalTime, Remo_CmdId_Camera_Get_LapseCap_TotalTime);
    addItem2Map(ui->ComboBox_MainVideo_Resolution, Remo_CmdId_Camera_Get_MainVideo_Resolution);
    addItem2Map(ui->ComboBox_SlowMotion_Resolution, Remo_CmdId_Camera_Get_SlowMotion_Resolution);
    addItem2Map(ui->ComboBox_SubVideo_Resolution, Remo_CmdId_Camera_Get_SubVideo_Resolution);
    addItem2Map(ui->ComboBox_SubWcorkMode_LapseRec_Interval, Remo_CmdId_Camera_Get_LapseRec_Interval);
    addItem2Map(ui->ComboBox_SubWcorkMode_LapseRec_TotalTime, Remo_CmdId_Camera_Get_LapseRec_TotalTime_Range);
    addItem2Map(ui->ComboBox_SubWorkMode_MultiPhoto_Burst, Remo_CmdId_Camera_Get_BurstCap);
    addItem2Map(ui->ComboBox_SubWorkMode_MultiPhoto_Continue, Remo_CmdId_Camera_Get_ContiCap);
    addItem2Map(ui->ComboBox_SubWorkMode_MultiPhoto_Panorama, Remo_CmdId_Camera_Get_PanoMode);
    addItem2Map(ui->ComboBox_SubWorkMode_Photo_Delay, Remo_CmdId_Camera_Get_CapDelayTime);
    addItem2Map(ui->ComboBox_SubWorkMode_Recode_Loop, Remo_CmdId_Camera_Get_LoopRec_Interval);
    addItem2Map(ui->comboBox_HDRMode, Remo_CmdId_Camera_Get_HDRMode);
    addItem2Map(ui->ComboBox_Thumbnail_Resolution, Remo_CmdId_Camera_Get_Thumbnail_Resolution);
    addItem2Map(ui->ComboBox_AebCap, Remo_CmdId_Camera_Get_AebCap);

    sendCmdCamera(Remo_CmdId_Camera_Get_CapDelayTime_Range);
    sendCmdCamera(Remo_CmdId_Camera_Get_BurstCap_Range);
    sendCmdCamera(Remo_CmdId_Camera_Get_ContiCap_Range);
    sendCmdCamera(Remo_CmdId_Camera_Get_PanoMode_Range);
    sendCmdCamera(Remo_CmdId_Camera_Get_LapseCap_Interval_Range);
    sendCmdCamera(Remo_CmdId_Camera_Get_LapseCap_TotalTime_Range);
    sendCmdCamera(Remo_CmdId_Camera_Get_CapScene_Range);
    sendCmdCamera(Remo_CmdId_Camera_Get_ImageResolution_Range);
    sendCmdCamera(Remo_CmdId_Camera_Get_MainVideo_Resolution_Range);
    sendCmdCamera(Remo_CmdId_Camera_Get_SubVideo_Resolution_Range);
    sendCmdCamera(Remo_CmdId_Camera_Get_Thumbnail_Resolution_Range);
    sendCmdCamera(Remo_CmdId_Camera_Get_LoopRec_Interval_Range);
    sendCmdCamera(Remo_CmdId_Camera_Get_LapseRec_Interval_Range);
    sendCmdCamera(Remo_CmdId_Camera_Get_LapseRec_TotalTime_Range);
    sendCmdCamera(Remo_CmdId_Camera_Get_SlowMotion_Resolution_Range);
    sendCmdCamera(Remo_CmdId_Camera_Get_AebCap_Range);
    ItemData itemData;
    if (findItemByUiPtr(ui->comboBox_HDRMode, itemData))
        surportRangeGot(itemData.subItemData, Remo_CmdId_Camera_Get_HDRMode); //HDR

    sendCmdCamera(Remo_CmdId_Camera_Set_CapOperation, std::vector<uint8_t>{CapOperation_Stop});
    sendCmdCamera(Remo_CmdId_Camera_Set_RecOperation, std::vector<uint8_t>{RecOperation_Stop});
}

PhotoAndVideoDialog::~PhotoAndVideoDialog()
{
    qDebug() << "PhotoAndVideoDialog has been deleted!!";
    delete ui;
}

void PhotoAndVideoDialog::closeEvent(QCloseEvent *event)
{
    LOG(INFO) << "PhotoAndVideoDialog::closeEvent";
    Q_UNUSED(event);
}

void PhotoAndVideoDialog::readVideoStreamDoneSlot(bool gotStream)
{
    QString showStr =  QString::fromLocal8Bit("模式切换成功");
    if (!gotStream) {
        showStr =  QString::fromLocal8Bit("获取视频流失败");
    }
    waitMsgBox->setText(showStr);
    waitMsgBox->setStandardButtons(QMessageBox::Ok);
//    waitMsgBox->exec();
//    waitMsgBox->close();
}

#ifndef IF_COND_SET_ENUMITEM_ACTION_BY_TYPE
#define IF_COND_SET_ENUMITEM_ACTION_BY_TYPE(var, enumItem, type, action) \
do {\
    if (enumItem == var) {\
    ui->type##_##enumItem->action(true);\
    }\
} while(0);
#endif
void PhotoAndVideoDialog::workModeGot(const Remo_Camera_WorkMode_s &workmode)
{
    LOG(INFO) << "PhotoAndVideoDialog::workModeGot!!!!";
    Remo_Camera_MainWorkMode_e mainWorkMode = static_cast<Remo_Camera_MainWorkMode_e>(workmode.MainWorkMode);
    setRecVideoByWorkMode(workmode);
    switch (mainWorkMode) {
    case MainWorkMode_Photo:
        IF_COND_SET_ENUMITEM_ACTION_BY_TYPE(workmode.SubWorkMode, SubWorkMode_Photo_Single, radioButton, setChecked);
        IF_COND_SET_ENUMITEM_ACTION_BY_TYPE(workmode.SubWorkMode, SubWorkMode_Photo_Delay, radioButton, setChecked);
        break;
    case MainWorKMode_MultiPhoto:
        IF_COND_SET_ENUMITEM_ACTION_BY_TYPE(workmode.SubWorkMode, SubWorkMode_MultiPhoto_Burst, radioButton, setChecked);
        IF_COND_SET_ENUMITEM_ACTION_BY_TYPE(workmode.SubWorkMode, SubWorkMode_MultiPhoto_Lapse, radioButton, setChecked);
        IF_COND_SET_ENUMITEM_ACTION_BY_TYPE(workmode.SubWorkMode, SubWorkMode_MultiPhoto_Continue, radioButton, setChecked);
        IF_COND_SET_ENUMITEM_ACTION_BY_TYPE(workmode.SubWorkMode, SubWorkMode_MultiPhoto_Panorama, radioButton, setChecked);
//        IF_COND_SET_ENUMITEM_ACTION_BY_TYPE(workmode.SubWorkMode, SubWorkMode_MultiPhoto_AEB, radioButton, setChecked);
        break;
    case MainWorKMode_Record:
        IF_COND_SET_ENUMITEM_ACTION_BY_TYPE(workmode.SubWorkMode, SubWorkMode_Recode_Normal, radioButton, setChecked);
        IF_COND_SET_ENUMITEM_ACTION_BY_TYPE(workmode.SubWorkMode, SubWorkMode_Recode_Loop, radioButton, setChecked);
        IF_COND_SET_ENUMITEM_ACTION_BY_TYPE(workmode.SubWorkMode, SubWcorkMode_Recode_LapseRec, radioButton, setChecked);
//        IF_COND_SET_ENUMITEM_ACTION_BY_TYPE(workmode.SubWorkMode, SubWcorkMode_Recode_Photo, radioButton, setChecked);
        IF_COND_SET_ENUMITEM_ACTION_BY_TYPE(workmode.SubWorkMode, SubWcorkMode_Recode_SlowMotion, radioButton, setChecked);
        break;
    case MainWorKMode_PlayBack:

        break;
    case MainWorKMode_Bott:

        break;
    default:
        break;
    }

//    if (Remo_CmdId_Camera_Set_WorkMode == ProtocolDataInterfaceImpl::content.cmdId)
//        emit getVideoStreamAgain();
}

void PhotoAndVideoDialog::settingGot(const std::vector<uint8_t> & data, Remo_CmdId_Camera_e cmdId)
{
//    LOG(INFO) << "PhotoAndVideoDialog::settingGot cmdId = " << std::hex << cmdId;
    if (!((cmdId & 0x1ff) >= 0x0 && (cmdId & 0x1ff) < 0x60)) return;

    if (data.empty()) {
        return;
    }

    QComboBox *ptr = static_cast<QComboBox*>(findUiPtrById(cmdId));
    if (nullptr != ptr) {
        int value = 0;
        memcpy(&value, data.data(), 1);
        ItemData item;
        if (findItemByUiPtr(ptr, item)) {
            std::set<SubItemData> subItemSet = item.subItemData;
            auto it = subItemSet.find({value, ""});
            if (it == subItemSet.end()) return;
            for (int i = 0; i < ptr->count(); ++i) {
                //查找存在item的QVariant里面的enum值，匹配的话设置为当前活动的item
                if (value == ptr->itemData(i).toInt()) {
                    ptr->setCurrentIndex(i);
                }
            }
        }
    }

    initCompleted = true;
}


void PhotoAndVideoDialog::surportRangeGot(std::set<SubItemData> rangeSet, Remo_CmdId_Camera_e cmdId)
{
//    qDebug() << "PhotoAndVideoDialog::surportRangeGot" << cmdId;
    if (!((cmdId & 0x1ff) >= 0x0 && (cmdId & 0x1ff) < 0x60)) return;
//    qDebug() << "PhotoAndVideoDialog::surportRangeGot through!!!!!!!!!!!!!!" << cmdId;

    QComboBox * ptr = static_cast<QComboBox*>(findUiPtrById(cmdId));

    if (nullptr != ptr) {
        for (int i = 0; i < ptr->count(); ++i) ptr->removeItem(i);
        for (auto it : rangeSet) {
            ptr->insertItem(it.Index, QString::fromUtf8(it.ShowStr.data()), QVariant(it.Index));//插入item并存储enum值在QVariant中
        }

        connect(ptr, SIGNAL(activated(int)), this, SLOT(comboBox_activated(int)));

        ItemData itemData;
        if (findItemByUiPtr(ptr, itemData)) {
            sendCmdCamera(static_cast<Remo_CmdId_Camera_e>(itemData.CmdId_GetData));//获取当前相机设置值
            LOG(INFO) << "send cmd to get current camera setting id is " << std::hex << itemData.CmdId_GetData;
        }
    }
}

void PhotoAndVideoDialog::retProcess(CmdContent cc)
{
    if (!(cc.cmdId == Remo_CmdId_Camera_Set_CapOperation ||
        cc.cmdId == Remo_CmdId_Camera_Set_RecOperation)) return;

    if (stopCapRec_Reverse) {
        return;
    }

//    QString type = QString::fromLocal8Bit("拍照");
//    if (cc.cmdId == Remo_CmdId_Camera_Set_RecOperation) {
//        type = QString::fromLocal8Bit("录像");
//        if (isRecording) {
//            type = QString::fromLocal8Bit("关闭录像");
//        }
//    }
    QString ret = QString::fromLocal8Bit("成功");
    if (cc.ret != Return_OK)
        ret = QString::fromLocal8Bit("失败");

    if (cc.cmdId == Remo_CmdId_Camera_Set_RecOperation) {
        QString type;
        if (isRecording) {
            button_stop_recod = recordeMsgDialog->addButton(QString::fromLocal8Bit("停止"), QMessageBox::ActionRole);
            if (cc.ret == Return_OK)
                connect(button_stop_recod, SIGNAL(clicked(bool)), this, SLOT(stop_recorde()));
            type = QString::fromLocal8Bit("开始录像");
        }
        else {
            type = QString::fromLocal8Bit("停止录像");
        }
        recordeMsgDialog->setText(type + ret);
    }
    else {
        if ((currentWorkMode.MainWorkMode == MainWorKMode_MultiPhoto &&
             currentWorkMode.SubWorkMode == SubWorkMode_MultiPhoto_Continue) ||
                (currentWorkMode.MainWorkMode == MainWorkMode_Photo &&
                 currentWorkMode.SubWorkMode == SubWorkMode_Photo_Delay)) {
            QString type = QString::fromLocal8Bit("拍照");
            if (isPhotoing) {
                type = QString::fromLocal8Bit("开始拍照");
                button_stop_photo = photoMsgDialog->addButton(QString::fromLocal8Bit("停止"), QMessageBox::ActionRole);
                if (cc.ret == Return_OK) {
                    if (currentWorkMode.MainWorkMode == MainWorkMode_Photo &&
                            currentWorkMode.SubWorkMode == SubWorkMode_Photo_Delay) {
                        boost::thread([&](){
                            int index = this->ui->ComboBox_SubWorkMode_Photo_Delay->currentIndex();
                            int indexdata = this->ui->ComboBox_SubWorkMode_Photo_Delay->itemData(index).toInt();
                            int timeSeconds = 0;
                            if (indexdata == 0)
                                timeSeconds = 3;
                            else if (indexdata == 1)
                                timeSeconds = 6;
                            else if (indexdata == 2)
                                timeSeconds = 10;
                            else if (indexdata == 3)
                                timeSeconds = 15;
                            else if (indexdata == 4)
                                timeSeconds = 20;
                            else if (indexdata == 5)
                                timeSeconds = 30;
                            QString text = this->photoMsgDialog->text();
                            while (isPhotoing && timeSeconds) {
                                emit photoDelayTickTack(text + "\n" + QString::number(timeSeconds) + "S");
                                --timeSeconds;
                                sleep(1);
                            }
                            emit photoDelayTickTack(QString::fromLocal8Bit("拍照完成"));
                        });
                    }
                    connect(button_stop_photo, SIGNAL(clicked(bool)), this, SLOT(stop_photo()));
                }
            }
            else {
                type = QString::fromLocal8Bit("停止拍照");
                button_stop_photo->setText(QString::fromLocal8Bit("确定"));
                photoMsgDialog->setText(type + ret + "");
                disconnect(button_stop_photo, SIGNAL(clicked(bool)), this, SLOT(stop_photo()));
                photoMsgDialog->exec();
            }
        }
        else {
            photoMsgDialog->setText(QString::fromLocal8Bit("拍照") + ret);
            button_stop_photo = photoMsgDialog->addButton(QString::fromLocal8Bit("确定"), QMessageBox::ActionRole);
        }
    }
}

void PhotoAndVideoDialog::initSurportRange()
{
//    sendCmdCamera(Remo_CmdId_Camera_Get_CapDelayTime_Range);
}

void PhotoAndVideoDialog::setRecVideoByWorkMode(Remo_Camera_WorkMode_s workMode)
{
    Remo_Camera_MainWorkMode_e mainWorkMode = static_cast<Remo_Camera_MainWorkMode_e>(workMode.MainWorkMode);
    currentWorkMode = workMode;
    if (MainWorkMode_Photo == mainWorkMode ||
        MainWorKMode_MultiPhoto == mainWorkMode) {
        recordOrCapture = false;
        ui->pushButton_Photo->setEnabled(true);
        ui->pushButton_Record->setEnabled(false);
    } else {
        recordOrCapture = true;
        ui->pushButton_Photo->setEnabled(false);
        ui->pushButton_Record->setEnabled(true);
    }
}

#ifndef ON_ENUMITEM_ACTION
//__VA_ARGS__ is cmdID
#define ON_ENUMITEM_ACTION(mainEnum, enumItem, type, action, ...) \
    void PhotoAndVideoDialog::on_##type##_##enumItem##_##action()\
    {\
        emit getVideoStreamAgain();\
        Remo_Camera_WorkMode_s workMode;\
        workMode.MainWorkMode = mainEnum;\
        workMode.SubWorkMode = enumItem;\
        async_setWorkMode(workMode);\
        sendCmdCamera(__VA_ARGS__);\
        waitMsgBox->setStandardButtons(0);\
        waitMsgBox->setText(QString::fromLocal8Bit("正在切换模式，请等待"));\
        if (initCompleted) {\
            waitMsgBox->exec();\
        }\
    }

ON_ENUMITEM_ACTION(MainWorkMode_Photo, SubWorkMode_Photo_Single, radioButton, clicked)
ON_ENUMITEM_ACTION(MainWorkMode_Photo, SubWorkMode_Photo_Delay, radioButton, clicked, Remo_CmdId_Camera_Get_CapDelayTime)
ON_ENUMITEM_ACTION(MainWorKMode_MultiPhoto, SubWorkMode_MultiPhoto_Burst, radioButton, clicked)
ON_ENUMITEM_ACTION(MainWorKMode_MultiPhoto, SubWorkMode_MultiPhoto_Lapse, radioButton, clicked)
ON_ENUMITEM_ACTION(MainWorKMode_MultiPhoto, SubWorkMode_MultiPhoto_Continue, radioButton, clicked)
ON_ENUMITEM_ACTION(MainWorKMode_MultiPhoto, SubWorkMode_MultiPhoto_Panorama, radioButton, clicked)
//ON_ENUMITEM_ACTION(MainWorKMode_MultiPhoto, SubWorkMode_MultiPhoto_AEB, radioButton, clicked)
ON_ENUMITEM_ACTION(MainWorKMode_Record, SubWorkMode_Recode_Normal, radioButton, clicked)
ON_ENUMITEM_ACTION(MainWorKMode_Record, SubWorkMode_Recode_Loop, radioButton, clicked)
ON_ENUMITEM_ACTION(MainWorKMode_Record, SubWcorkMode_Recode_LapseRec, radioButton, clicked)
//ON_ENUMITEM_ACTION(MainWorKMode_Record, SubWcorkMode_Recode_Photo, radioButton, clicked)
ON_ENUMITEM_ACTION(MainWorKMode_Record, SubWcorkMode_Recode_SlowMotion, radioButton, clicked)
#endif

//void PhotoAndVideoDialog::on_pushButton_Start_clicked()
//{
//    if (recordOrCapture) {
//        sendCmdCamera(Remo_CmdId_Camera_Set_RecOperation);
//    } else {
//        sendCmdCamera(Remo_CmdId_Camera_Set_CapOperation);
//    }
//    ui->pushButton_Stop->setEnabled(true);
//    ui->pushButton_Start->setEnabled(false);
//}

//void PhotoAndVideoDialog::on_pushButton_Stop_clicked()
//{
//    if (recordOrCapture) {
//        sendCmdCamera(Remo_CmdId_Camera_Set_RecOperation);
//    } else {
//        sendCmdCamera(Remo_CmdId_Camera_Set_CapOperation);
//    }
//    ui->pushButton_Start->setEnabled(true);
//    ui->pushButton_Stop->setEnabled(false);
//}

void PhotoAndVideoDialog::on_pushButton_Photo_clicked()
{
    photoMsgDialog->setText(QString::fromLocal8Bit("正在拍照，稍等..."));

    if (currentWorkMode.MainWorkMode == MainWorKMode_MultiPhoto &&
            currentWorkMode.SubWorkMode == SubWorkMode_MultiPhoto_Continue ||
            (currentWorkMode.MainWorkMode == MainWorkMode_Photo &&
             currentWorkMode.SubWorkMode == SubWorkMode_Photo_Delay)) {
        if (nullptr != button_stop_photo) {
            disconnect(button_stop_photo, SIGNAL(clicked(bool)), this, SLOT(stop_photo()));
        }
    }

    if (nullptr != button_stop_photo) {
        photoMsgDialog->removeButton(button_stop_photo);
        delete button_stop_photo;
    }

    sendCmdCamera(Remo_CmdId_Camera_Set_CapOperation, std::vector<uint8_t>{CapOperation_Start});
    isPhotoing = true;
    stopCapRec_Reverse = false;
    photoMsgDialog->exec();
}

void PhotoAndVideoDialog::on_pushButton_Record_clicked()
{
    recordeMsgDialog->setText(QString::fromLocal8Bit("正在录像，稍等..."));

    sendCmdCamera(Remo_CmdId_Camera_Set_RecOperation, std::vector<uint8_t>{RecOperation_Start});

    if (nullptr != button_stop_recod) {
        disconnect(button_stop_recod, SIGNAL(clicked(bool)), this, SLOT(stop_recorde()));
        recordeMsgDialog->removeButton(button_stop_recod);
        delete button_stop_recod;
    }
    isRecording = true;
    stopCapRec_Reverse = false;
    recordeMsgDialog->exec();
}

void PhotoAndVideoDialog::comboBox_activated(int index)
{
//    int itemIndex = ui->ComboBox_SubWorkMode_Photo_Delay->itemData(index).toInt();
//    ItemData itemdata;
//    if (findItemByUiPtr(ui->ComboBox_SubWorkMode_Photo_Delay, itemdata)) {
//        sendCmdCamera(static_cast<Remo_CmdId_Camera_e>(itemdata.CmdId_SetData), std::vector<uint8_t>(itemIndex));
//        qDebug() << "on_ComboBox_SubWorkMode_Photo_Delay_activated send cmd" \
//                 << index << "cmdid is " << itemdata.CmdId_SetData;
//    }

    QComboBox * comboBox = dynamic_cast<QComboBox*>(sender());
    if (nullptr != comboBox) {
        ItemData itemdata;
        if (findItemByUiPtr(comboBox, itemdata)) {
            sendCmdCamera(static_cast<Remo_CmdId_Camera_e>(itemdata.CmdId_SetData),
                          std::vector<uint8_t>{comboBox->itemData(index).toInt()});
            if (comboBox == ui->ComboBox_MainVideo_Resolution) {
                emit getVideoStreamAgain();
            }
        }
    }
}

void PhotoAndVideoDialog::stop_recorde()
{
    LOG(INFO) << "stop_recorde";
    recordeMsgDialog->setText(QString::fromLocal8Bit("停止录像，保存中..."));
//    recordeMsgDialog->setStandardButtons(0);
    sendCmdCamera(Remo_CmdId_Camera_Set_RecOperation, std::vector<uint8_t>{RecOperation_Stop});
    recordeMsgDialog->exec();
    isRecording = false;
}

void PhotoAndVideoDialog::stop_photo()
{
    photoMsgDialog->setText(QString::fromLocal8Bit("停止拍照，保存中..."));
    sendCmdCamera(Remo_CmdId_Camera_Set_CapOperation, std::vector<uint8_t>{CapOperation_Stop});
    isPhotoing = false;
    photoMsgDialog->exec();
}

void PhotoAndVideoDialog::set_photoMsgDialog_text(QString showStr)
{
    photoMsgDialog->setText(showStr);
    if (QString::fromLocal8Bit("拍照完成") == showStr) {
        disconnect(button_stop_photo, SIGNAL(clicked(bool)), this, SLOT(stop_photo()));
        button_stop_photo->setText(QString::fromLocal8Bit("确定"));
    }
}

