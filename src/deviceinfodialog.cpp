#include "deviceinfodialog.h"

DeviceInfoDialog::DeviceInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DeviceInfo)
{
    ui->setupUi(this);
}

boost::shared_ptr<DeviceInfoDialog> DeviceInfoDialog::createInstance(QWidget *parent)
{
    boost::shared_ptr<DeviceInfoDialog> instance(boost::shared_ptr<DeviceInfoDialog>(new DeviceInfoDialog(parent)));
    instance->registerSelf2Handler();
}

void DeviceInfoDialog::batteryInfoHandle(std::vector<uint8_t> data)
{

}

void DeviceInfoDialog::chargerInfoHandle(std::vector<uint8_t> data)
{

}
