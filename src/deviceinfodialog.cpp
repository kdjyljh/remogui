#include "deviceinfodialog.h"

DeviceInfoDialog::DeviceInfoDialog(QWidget *parent) :
    QDialog(parent),
    ProtocolDataInterfaceImpl(DispatcheType_DeviceInfo),
    ui(new Ui::DeviceInfo)
{
    ui->setupUi(this);
}

void DeviceInfoDialog::batteryInfoHandle(std::vector<uint8_t> data)
{

}

void DeviceInfoDialog::chargerInfoHandle(std::vector<uint8_t> data)
{

}
