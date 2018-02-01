#include "focusdialog.h"
#include <QDialog>
#include <QDebug>

FocusDialog::FocusDialog(QWidget * parent) :
    QDialog(parent),
    ProtocolDataInterfaceImpl(DispatcheType_Focus_Zoom),
    ui(new Ui::Focus)
{
    ui->setupUi(this);
    setFixedSize(350, 300);

    addItem2Map(ui->ComboBox_AFMode, Remo_CmdId_Camera_Get_AFMode);
    addItem2Map(ui->ComboBox_AFResult, Remo_CmdId_Camera_Get_AFResult);
    addItem2Map(ui->ComboBox_AFStatus, Remo_CmdId_Camera_Get_AFStatus);

    ItemData itemData;
    if (findItemByUiPtr(ui->ComboBox_AFResult, itemData))
        surportRangeGot(itemData.subItemData, Remo_CmdId_Camera_Get_AFResult);
    if (findItemByUiPtr(ui->ComboBox_AFStatus, itemData))
        surportRangeGot(itemData.subItemData, Remo_CmdId_Camera_Get_AFStatus);
    ui->ComboBox_ZoomSpeed->addItem(QString::fromUtf8("最慢"), 0);
    ui->ComboBox_ZoomSpeed->addItem(QString::fromUtf8("较慢"), 1);
    ui->ComboBox_ZoomSpeed->addItem(QString::fromUtf8("中等"), 2);
    ui->ComboBox_ZoomSpeed->addItem(QString::fromUtf8("较快"), 3);
    ui->ComboBox_ZoomSpeed->addItem(QString::fromUtf8("最快"), 4);

    ui->horizontalSlider_FocalLengthPosNo->setRange(0, 255);

    sendCmdCamera(Remo_CmdId_Camera_Get_AFMode_Range);
    sendCmdCamera(Remo_CmdId_Camera_Get_FocalLengthInfo);
}

void FocusDialog::closeEvent(QCloseEvent *event)
{
    emit focusStatusChange(false);
}

void FocusDialog::surportRangeGot(std::set<SubItemData> rangeSet, Remo_CmdId_Camera_e cmdId)
{
    LOG(INFO) << "FocusDialog::surportRangeGot cmdId = " << std::hex << cmdId;
    if (!((cmdId & 0x1ff) >= 0x7b && (cmdId & 0x1ff) < 0x85)) return;

    QComboBox * ptr = static_cast<QComboBox*>(findUiPtrById(cmdId));
    if (nullptr != ptr) {
        for (int i = 0; i < ptr->count(); ++i) ptr->removeItem(i);
        for (auto it : rangeSet) {
            ptr->insertItem(it.Index, QString::fromUtf8(it.ShowStr.data()), it.Index);
        }
        if (Remo_CmdId_Camera_Get_AFMode_Range == cmdId)
            sendCmdCamera(Remo_CmdId_Camera_Get_AFMode);
    }
}

void FocusDialog::settingGot(const std::vector<uint8_t> &data, Remo_CmdId_Camera_e cmdId)
{
    LOG(INFO) << "FocusDialog::settingGot cmdId = " << std::hex << cmdId;
    if (!((cmdId & 0x1ff) >= 0x7b && (cmdId & 0x1ff) < 0x85)) return;

    if (Remo_CmdId_Camera_Get_FocalLengthInfo == cmdId) {
        int focalLengthDvideNum = 0;
        memcpy(&focalLengthDvideNum, data.data() + 6, 2);
        ui->horizontalSlider_FocalLengthPosNo->setRange(0, focalLengthDvideNum);
        return;
    }

    QComboBox * ptr = static_cast<QComboBox*>(findUiPtrById(cmdId));
    int indexValue = data.at(0);
//    memcpy(&indexValue, data.data(), 1);
    if (nullptr != ptr) {
        ItemData itemData;
        if (findItemByUiPtr(ptr, itemData)) {
//            QString str = ptr->objectName();
//            int coun = ptr->count();
            for (int i = 0; i < ptr->count(); ++i) {
                if (ptr->itemData(i).toInt() == indexValue) {
                    ptr->setCurrentIndex(i);
                }
            }
        }
    }
}

void FocusDialog::on_ComboBox_AFMode_activated(int index)
{
    QComboBox * ptr = dynamic_cast<QComboBox*>(sender());
    if (nullptr != ptr) {
        sendCmdCamera(Remo_CmdId_Camera_Set_AFMode, std::vector<uint8_t>{ptr->itemData(index).toInt()});
    }
}

void FocusDialog::on_ComboBox_ZoomSpeed_activated(int index)
{
    QComboBox * ptr = dynamic_cast<QComboBox*>(sender());
    if (nullptr != ptr) {
        sendCmdCamera(Remo_CmdId_Camera_Set_AFMode, std::vector<uint8_t>{ptr->itemData(index).toInt()});
    }
}

void FocusDialog::on_horizontalSlider_FocalLengthPosNo_sliderReleased()
{
    qDebug() << "releaseed value " << ui->horizontalSlider_FocalLengthPosNo->value();
    Remo_Camera_ZoomControlParam_s data ;
    data.ZoomControlType = 0;
    data.Speed = ui->ComboBox_ZoomSpeed->itemData(ui->ComboBox_ZoomSpeed->currentIndex()).toInt();
    data.TargetPosNo = ui->horizontalSlider_FocalLengthPosNo->value();
    sendCmdCamera(Remo_CmdId_Camera_Set_ZoomControlParam,
                  std::vector<uint8_t>(reinterpret_cast<uint8_t*>(&data), reinterpret_cast<uint8_t*>(&data) + 5));
}

void FocusDialog::on_pushButton_StopZoom_clicked()
{
    sendCmdCamera(Remo_CmdId_Camera_Set_Stop_AF);
}

void FocusDialog::on_pushButton_StartFocus_clicked()
{
    static int clickeCount = 0;
    clickeCount++;
    bool isFocusing = clickeCount % 2;
    QString showStr;
    if (isFocusing) {
        showStr = QString::fromUtf8("结束对焦");
    } else {
        showStr = QString::fromUtf8("启动对焦");
    }
    ui->pushButton_StartFocus->setText(showStr);
    emit focusStatusChange(isFocusing);
}
