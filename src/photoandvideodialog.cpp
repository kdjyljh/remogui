#include "photoandvideodialog.h"
#include "itemdatadef.h"

#include <QDebug>
#include <QButtonGroup>

static const unsigned DEFAULT_DIALOG_WITH = 600;
static const unsigned DEFAULT_DIALOG_HEIGHT = 550;

PhotoAndVideoDialog::PhotoAndVideoDialog(QWidget *parent) :
    QDialog(parent),
    ProtocolDataInterfaceImpl(),
    ui(new Ui::PhotoAndVideoDialog)
{
    ui->setupUi(this);

    setFixedSize(DEFAULT_DIALOG_WITH, DEFAULT_DIALOG_HEIGHT);
    ui->pushButton_Stop->setEnabled(false);
    QButtonGroup * buttGroup = new QButtonGroup(this);
    buttGroup->addButton(ui->radioButton_SubWcorkMode_Recode_Collapses);
    buttGroup->addButton(ui->radioButton_SubWcorkMode_Recode_Photo);
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

    initSurportRange();
    addItem2Map(ui->ComboBox_SubWorkMode_Photo_Delay, Remo_CmdId_Camera_Get_CapDelayTime);

//    ui->ComboBox_SubWorkMode_Photo_Delay->addItem("1");
//    ui->ComboBox_SubWorkMode_Photo_Delay->addItem("2");
//    ui->ComboBox_SubWorkMode_Photo_Delay->addItem("3");
//    ui->ComboBox_SubWorkMode_Photo_Delay->addItem("4");
//    ui->ComboBox_SubWorkMode_Photo_Delay->addItem("5");
//    ui->ComboBox_SubWorkMode_Photo_Delay->addItem("6");
//    ui->ComboBox_SubWorkMode_Photo_Delay->addItem("7");
//    ui->ComboBox_SubWorkMode_Photo_Delay->insertItem(-2, "some", 100);
//    ui->ComboBox_SubWorkMode_Photo_Delay->insertItem(-1, "somesome");

//    ui->formLayout->addRow(QString::fromUtf8("和最大值和step，"), (QWidget*)(new QComboBox()));

//    sendCmdCamera(Remo_CmdId_Camera_Get_WorkMode);
//    sendCmdCamera(Remo_CmdId_Camera_Get_CapDelayTime_Range);
//    sendCmdCamera(Remo_CmdId_Camera_Get_CapDelayTime);
}

PhotoAndVideoDialog::~PhotoAndVideoDialog()
{
    qDebug() << "PhotoAndVideoDialog has been deleted!!";
    delete ui;
}

void PhotoAndVideoDialog::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
}

#ifndef IF_COND_SET_ENUMITEM_ACTION_BY_TYPE
#define IF_COND_SET_ENUMITEM_ACTION_BY_TYPE(var, enumItem, type, action) \
do {\
    if (enumItem == var) {\
    ui->type##_##enumItem->action(true);\
    }\
}while(0);
#endif
void PhotoAndVideoDialog::workModeGot(const Remo_Camera_WorkMode_s &workmode)
{
    qDebug() << "PhotoAndVideoDialog::workModeGot!!!!";
    Remo_Camera_MainWorkMode_e mainWorkMode = static_cast<Remo_Camera_MainWorkMode_e>(workmode.MainWorkMode);
    setRecVideoByMainWorkMode(mainWorkMode);
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
        IF_COND_SET_ENUMITEM_ACTION_BY_TYPE(workmode.SubWorkMode, SubWcorkMode_Recode_Collapses, radioButton, setChecked);
        IF_COND_SET_ENUMITEM_ACTION_BY_TYPE(workmode.SubWorkMode, SubWcorkMode_Recode_Photo, radioButton, setChecked);
        IF_COND_SET_ENUMITEM_ACTION_BY_TYPE(workmode.SubWorkMode, SubWcorkMode_Recode_SlowMotion, radioButton, setChecked);
        break;
    case MainWorKMode_PlayBack:

        break;
    case MainWorKMode_Bott:

        break;
    default:
        break;
    }
}

void PhotoAndVideoDialog::cameraSettingGot(const std::vector<uint8_t> & data, Remo_CmdId_e cmdId)
{
    if (data.empty()) {
        return;
    }

    QComboBox *ptr = static_cast<QComboBox*>(findUiPtrById(cmdId));
    if (nullptr != ptr) {
        int value = data.at(0);
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
}


void PhotoAndVideoDialog::surportRangeGot(std::set<SubItemData> rangeSet, Remo_CmdId_e cmdId)
{
    QComboBox * ptr = static_cast<QComboBox*>(findUiPtrById(cmdId));
    if (nullptr != ptr) {
        for (auto it : rangeSet) {
            ptr->insertItem(it.Index, QString::fromUtf8(it.ShowStr.data()), QVariant(it.Index));//插入item并存储enum值在QVariant中
        }
        ItemData itemData;
        if (findItemByUiPtr(ptr, itemData)) {
            sendCmdCamera(static_cast<Remo_CmdId_e>(itemData.CmdId_GetData));//获取当前相机设置值
            qDebug() << "send cmd to get current camera setting id is " << itemData.CmdId_GetData;
        }
    }
}

void PhotoAndVideoDialog::initSurportRange()
{
    sendCmdCamera(Remo_CmdId_Camera_Get_CapDelayTime_Range);
}

void PhotoAndVideoDialog::setRecVideoByMainWorkMode(Remo_Camera_MainWorkMode_e mainWorkMode)
{
    if (MainWorkMode_Photo == mainWorkMode ||
        MainWorKMode_MultiPhoto == mainWorkMode) {
        recordOrCapture = false;
    } else {
        recordOrCapture = true;
    }
}

#ifndef ON_ENUMITEM_ACTION
//__VA_ARGS__ is cmdID
#define ON_ENUMITEM_ACTION(mainEnum, enumItem, type, action, ...) \
    void PhotoAndVideoDialog::on_##type##_##enumItem##_##action()\
    {\
        setRecVideoByMainWorkMode(mainEnum);\
        Remo_Camera_WorkMode_s workMode;\
        workMode.MainWorkMode = mainEnum;\
        workMode.SubWorkMode = enumItem;\
        async_setWorkMode(workMode);\
        sendCmdCamera(__VA_ARGS__);\
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
ON_ENUMITEM_ACTION(MainWorKMode_Record, SubWcorkMode_Recode_Collapses, radioButton, clicked)
ON_ENUMITEM_ACTION(MainWorKMode_Record, SubWcorkMode_Recode_Photo, radioButton, clicked)
ON_ENUMITEM_ACTION(MainWorKMode_Record, SubWcorkMode_Recode_SlowMotion, radioButton, clicked)
#endif

void PhotoAndVideoDialog::on_pushButton_Start_clicked()
{
    if (recordOrCapture) {
        sendCmdCamera(Remo_CmdId_Camera_Set_CapOperation);
    } else {
        sendCmdCamera(Remo_CmdId_Camera_Set_RecOperation);
    }
    ui->pushButton_Stop->setEnabled(true);
    ui->pushButton_Start->setEnabled(false);
}

void PhotoAndVideoDialog::on_pushButton_Stop_clicked()
{
    if (recordOrCapture) {
        sendCmdCamera(Remo_CmdId_Camera_Set_CapOperation);
    } else {
        sendCmdCamera(Remo_CmdId_Camera_Set_RecOperation);
    }
    ui->pushButton_Start->setEnabled(true);
    ui->pushButton_Stop->setEnabled(false);
}

void PhotoAndVideoDialog::on_ComboBox_SubWorkMode_Photo_Delay_activated(int index)
{
//    int itemIndex = ui->ComboBox_SubWorkMode_Photo_Delay->itemData(index).toInt();
//    ItemData itemdata;
//    if (findItemByUiPtr(ui->ComboBox_SubWorkMode_Photo_Delay, itemdata)) {
//        sendCmdCamera(static_cast<Remo_CmdId_e>(itemdata.CmdId_SetData), std::vector<uint8_t>(itemIndex));
//        qDebug() << "on_ComboBox_SubWorkMode_Photo_Delay_activated send cmd" \
//                 << index << "cmdid is " << itemdata.CmdId_SetData;
//    }

    QComboBox * comboBox = dynamic_cast<QComboBox*>(sender());
    if (nullptr != comboBox) {
        int itemIndex = comboBox->itemData(index).toInt();
        ItemData itemdata;
        if (findItemByUiPtr(comboBox, itemdata)) {
            sendCmdCamera(static_cast<Remo_CmdId_e>(itemdata.CmdId_SetData), std::vector<uint8_t>(itemIndex));
            qDebug() << "on_ComboBox_SubWorkMode_Photo_Delay_activated send cmd" \
                     << index << "cmdid is " << itemdata.CmdId_SetData;
        }
    }
}

