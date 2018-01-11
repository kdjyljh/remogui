#include "deviceinfodialog.h"

DeviceInfoDialog::DeviceInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DeviceInfo)
{
    ui->setupUi(this);
    DeviceInfoInterface::init();
}

boost::shared_ptr<DeviceInfoDialog> DeviceInfoDialog::createInstance(QWidget *parent)
{
    boost::shared_ptr<DeviceInfoDialog> instance(boost::shared_ptr<DeviceInfoDialog>(new DeviceInfoDialog(parent)));
}

void DeviceInfoDialog::batteryInfoHandle(std::vector<uint8_t> data)
{

}

void DeviceInfoDialog::chargerInfoHandle(std::vector<uint8_t> data)
{

}
