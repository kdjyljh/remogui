#include "aemodedialog.h"
#include <QDebug>
#include <cstring>


struct CmdGetSetRang
{
    int Get;
    int Set;
    int Range;
};

const std::vector<CmdGetSetRang> cmdV =
{
    {Remo_CmdId_Camera_Get_Current_Evbias, Remo_CmdId_Camera_PlaceHolder, Remo_CmdId_Camera_Get_Current_Evbias_Range},
    {Remo_CmdId_Camera_Get_Current_Aperture, Remo_CmdId_Camera_PlaceHolder, Remo_CmdId_Camera_Get_Current_Aperture_Range},
    {Remo_CmdId_Camera_Get_Current_Shutter, Remo_CmdId_Camera_PlaceHolder, Remo_CmdId_Camera_Get_Current_Shutter_Range},
    {Remo_CmdId_Camera_Get_Current_ISO, Remo_CmdId_Camera_PlaceHolder, Remo_CmdId_Camera_Get_Current_ISO_Range}
};
//bool findOtherCmd(CmdGetSetRang & ret, int get, int set = Remo_CmdId_Camera_PlaceHolder, int range = Remo_CmdId_Camera_PlaceHolder);
static bool findOtherCmd(CmdGetSetRang & ret, int get, int set = Remo_CmdId_Camera_PlaceHolder, int range = Remo_CmdId_Camera_PlaceHolder)
{
    for (auto it : cmdV) {
        if (get == it.Get || set == it.Set) {
            ret = it;
            return true;
        }
    }
    return false;
}

AeModeDialog::AeModeDialog(QWidget * parent) :
    QDialog(parent),
    ProtocolDataInterfaceImpl(DispatcheType_AeMode),
    currentAeMode(AeMode_Bott),
    ui(new Ui::AeMOde)
{
    ui->setupUi(this);
    setFixedSize(300, 200);

    addItem2Map(ui->ComboBox_AeMode, Remo_CmdId_Camera_Get_AeMode);
    addItem2Map(ui->ComboBox_Aperture, Remo_CmdId_Camera_Get_Current_Aperture,
                Remo_CmdId_Camera_PlaceHolder, Remo_CmdId_Camera_Get_Current_Aperture_Range, true);
    addItem2Map(ui->ComboBox_Evbias, Remo_CmdId_Camera_Get_Current_Evbias,
                Remo_CmdId_Camera_PlaceHolder, Remo_CmdId_Camera_Get_Current_Evbias_Range, true);
    addItem2Map(ui->ComboBox_Shutter, Remo_CmdId_Camera_Get_Current_Shutter,
                Remo_CmdId_Camera_PlaceHolder, Remo_CmdId_Camera_Get_Current_Shutter_Range, true);
    addItem2Map(ui->ComboBox_ISO, Remo_CmdId_Camera_Get_Current_ISO,
                Remo_CmdId_Camera_PlaceHolder, Remo_CmdId_Camera_Get_Current_ISO_Range, true);

    sendCmdCamera(Remo_CmdId_Camera_Get_AeMode_Range);
    sendCmdCamera(Remo_CmdId_Camera_Get_Current_Aperture_Range);
    sendCmdCamera(Remo_CmdId_Camera_Get_Current_Shutter_Range);
    sendCmdCamera(Remo_CmdId_Camera_Get_Current_ISO_Range);
    sendCmdCamera(Remo_CmdId_Camera_Get_Current_Evbias_Range);
}

void AeModeDialog::settingGot(const std::vector<uint8_t> &data, Remo_CmdId_Camera_e cmdId)
{
//    if (!((cmdId & 0x1ff) >= 0x67 && (cmdId & 0x1ff) < 0x78)) return;

    int indexValue = 0;
    memcpy(&indexValue, data.data(), 1);

    QComboBox * ptr = nullptr;
    if (Remo_CmdId_Camera_Get_AeMode == cmdId) {
        currentAeMode = static_cast<Remo_Camera_AeMode_e>(indexValue);
        syncUiModeByAeMode();
        ptr = static_cast<QComboBox*>(findUiPtrById(Remo_CmdId_Camera_Get_AeMode));
    }
    else {
        CmdGetSetRang cmdGSR;
        findOtherCmd(cmdGSR, cmdId);
        ptr = static_cast<QComboBox*>(findUiPtrById(static_cast<Remo_CmdId_Camera_e>(cmdId),
                                                    static_cast<Remo_CmdId_Camera_e>(cmdGSR.Set), static_cast<Remo_CmdId_Camera_e>(cmdGSR.Range)));
    }
    if (nullptr != ptr) {
        ItemData itemData;
        if (findItemByUiPtr(ptr, itemData)) {
            for (auto it : itemData.subItemData) {
                for (int i = 0; i < ptr->count(); ++i) {
                    if (it.Index == ptr->itemData(i).toInt()) {
                        ptr->setCurrentIndex(i);
                    }
                }
            }
        }
    }
}

//void setCurrentComboxActivated(int activatedData);
//void setCurrentComboxRange(std::set<SubItemData> rangeSet);

void AeModeDialog::surportRangeGot(std::set<SubItemData> rangeSet, Remo_CmdId_Camera_e cmdId)
{
//    if (!((cmdId & 0x1ff) >= 0x67 && (cmdId & 0x1ff) < 0x78)) return;

//    int indexValue = 0;
//    memcpy(&indexValue, data.data(), 1);

    QComboBox * ptr = nullptr;
    if (Remo_CmdId_Camera_Get_AeMode_Range == cmdId) {
        ptr = static_cast<QComboBox*>(findUiPtrById(Remo_CmdId_Camera_Get_AeMode));
    }
    else {
        CmdGetSetRang cmdGSR;
        findOtherCmd(cmdGSR, cmdId);
        ptr = static_cast<QComboBox*>(findUiPtrById(static_cast<Remo_CmdId_Camera_e>(cmdId),
                                                    static_cast<Remo_CmdId_Camera_e>(cmdGSR.Set), static_cast<Remo_CmdId_Camera_e>(cmdGSR.Range)));
    }
    if (nullptr != ptr) {
        connect(ptr, SIGNAL(activated(int)), this, SLOT(combox_activated(int)));

        for (int i = 0; i < ptr->count(); ++i) {
            ptr->removeItem(i);
        }
        for (auto it : rangeSet) {
            ptr->insertItem(it.Index, QString::fromUtf8(it.ShowStr.data()), it.Index);
        }

        ItemData itemData;
        if (findItemByUiPtr(ptr, itemData)) {
            sendCmdCamera(static_cast<Remo_CmdId_Camera_e>(itemData.CmdId_GetData));
        }
    }
}

void AeModeDialog::syncUiModeByAeMode()
{
    switch (currentAeMode) {
    case AeMode_Auto:
        ui->ComboBox_Aperture->setEnabled(false);
        ui->ComboBox_ISO->setEnabled(false);
        ui->ComboBox_Shutter->setEnabled(false);
        ui->ComboBox_Evbias->setEnabled(true);
        break;
    case AeMode_S:
        ui->ComboBox_Aperture->setEnabled(false);
        ui->ComboBox_ISO->setEnabled(false);
        ui->ComboBox_Shutter->setEnabled(true);
        ui->ComboBox_Evbias->setEnabled(true);
        break;
    case AeMode_A:
        ui->ComboBox_Aperture->setEnabled(true);
        ui->ComboBox_ISO->setEnabled(false);
        ui->ComboBox_Shutter->setEnabled(false);
        ui->ComboBox_Evbias->setEnabled(true);
        break;
    case AeMode_M:
        ui->ComboBox_Aperture->setEnabled(true);
        ui->ComboBox_ISO->setEnabled(true);
        ui->ComboBox_Shutter->setEnabled(true);
        ui->ComboBox_Evbias->setEnabled(false);
        break;
    default:
        break;
    }

//    sendCmdCamera(Remo_CmdId_Camera_Get_AeMode);
//    sendCmdCamera(Remo_CmdId_Camera_Get_Current_Aperture);
//    sendCmdCamera(Remo_CmdId_Camera_Get_Current_Shutter);
//    sendCmdCamera(Remo_CmdId_Camera_Get_Current_ISO);
//    sendCmdCamera(Remo_CmdId_Camera_Get_Current_Evbias);
}

void AeModeDialog::combox_activated(int index)
{
    QComboBox * comboBox = dynamic_cast<QComboBox*>(sender());
    if (nullptr != comboBox) {
        ItemData itemdata;
        if (findItemByUiPtr(comboBox, itemdata)) {
            sendCmdCamera(static_cast<Remo_CmdId_Camera_e>(itemdata.CmdId_SetData), std::vector<uint8_t>{comboBox->itemData(index).toInt()});
            qDebug() << "AeModeDialog::combox_activated send cmd" \
                     << index << "cmdid is " << itemdata.CmdId_SetData;
        }
    }
}
