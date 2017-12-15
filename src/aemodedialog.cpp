#include "aemodedialog.h"
#include <QDebug>


AeModeDialog::AeModeDialog(QWidget * parent) :
    QDialog(parent),
    ProtocolDataInterfaceImpl(),
    ui(new Ui::AeMOde)
{
    ui->setupUi(this);
    setFixedSize(300, 200);

    addItem2Map(ui->ComboBox_AeMode, Remo_CmdId_Camera_Get_AeMode);


}

void AeModeDialog::cameraSettingGot(const std::vector<uint8_t> &data, Remo_CmdId_e cmdId)
{
    int valueIndex = data.at(0);
    if (Remo_CmdId_Camera_Get_AeMode == cmdId) {
        syncUiModeByAeMode(static_cast<Remo_Camera_AeMode_e>(valueIndex));
    }

    QComboBox * ptr = static_cast<QComboBox*>(findUiPtrById(cmdId));
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

void AeModeDialog::surportRangeGot(std::set<SubItemData> rangeSet, Remo_CmdId_e cmdId)
{
    QComboBox * ptr = static_cast<QComboBox*>(findUiPtrById(cmdId));
    if (nullptr != ptr) {
        ItemData itemData;
        if (findItemByUiPtr(ptr, itemData)) {
            for (auto it : itemData.subItemData) {
                ptr->insertItem(it.Index, QString::fromUtf8(it.ShowStr.data()), it.Index);
                connect(ptr, SIGNAL(activated(int)), this, SLOT(combox_activated(int)));
            }
        }
    }
}

void AeModeDialog::syncUiModeByAeMode(Remo_Camera_AeMode_e aeMode)
{
    switch (aeMode) {
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
}

void AeModeDialog::combox_activated(int index)
{
    QComboBox * comboBox = dynamic_cast<QComboBox*>(sender());
    if (nullptr != comboBox) {
        int itemIndex = comboBox->itemData(index).toInt();
        ItemData itemdata;
        if (findItemByUiPtr(comboBox, itemdata)) {
            sendCmdCamera(static_cast<Remo_CmdId_e>(itemdata.CmdId_SetData), std::vector<uint8_t>(itemIndex));
            qDebug() << "AeModeDialog::combox_activated send cmd" \
                     << index << "cmdid is " << itemdata.CmdId_SetData;
        }
    }
}
