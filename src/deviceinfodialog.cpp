#include "deviceinfodialog.h"

DeviceInfoDialog::DeviceInfoDialog(QWidget *parent) :
    QDialog(parent),
    batteryInfo(boost::shared_ptr<BatteryInfo>(new BatteryInfo)),
    ui(new Ui::DeviceInfo)
{
    ui->setupUi(this);
    batteryInfo->registerSelf2Handler();

    batteryInfo->sendCmdBattery(Remo_CmdId_Battery_Get_Battery_Info);
    batteryInfo->sendCmdBattery(Remo_CmdId_Battery_Get_Charger_Info);
}
