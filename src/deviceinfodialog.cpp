#include "deviceinfodialog.h"

DeviceInfoDialog::DeviceInfoDialog(QWidget *parent) :
    ProtocolDataInterfaceImpl(DispatcheType_DeviceInfo),
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
    Remo_Battery_BatteryInfo_s bi;
    if (data.size() != sizeof(bi)) return;

    memcpy(&bi, data.data(), sizeof(bi));
    QString healthStr;
    if (BatteryHealth_OverHeat == bi.Health) {
        healthStr = QString::fromUtf8("过热");
    }
    else if (BatteryHealth_OverVoltage == bi.Health) {
        healthStr = QString::fromUtf8("过压");
    }
    else if (BatteryHealth_OverCurrent == bi.Health) {
        healthStr = QString::fromUtf8("过流");
    }
    else if (BatteryHealth_Cold == bi.Health) {
        healthStr = QString::fromUtf8("过冷");
    }
    QString statusStr;
    if (Power_Supply_Status_Charging == bi.Status) {
        statusStr = QString::fromUtf8("正在充电");
    }
    else if (Power_Supply_Status_Discharging == bi.Status) {
        statusStr = QString::fromUtf8("未充电");
    }
    QString cellVoltage(QString::number(bi.Cell_Voltage[0]) + "," +
                        QString::number(bi.Cell_Voltage[1]) + "," +
                        QString::number(bi.Cell_Voltage[2]) + "," +
                        QString::number(bi.Cell_Voltage[3]));

    ui->LineEdit_BatteryCurrent->setText(QString::number(bi.Current));
    ui->LineEdit_BatteryTemperature->setText(QString::number(bi.Temperature));
    ui->LineEdit_BatteryChargeFull->setText(QString::number(bi.Charge_Full));
    ui->LineEdit_BatteryVoltage->setText(QString::number(bi.Voltage));
    ui->LineEdit_BatteryHealth->setText(healthStr);
    ui->LineEdit_BatteryStatus->setText(statusStr);
    ui->LineEdit_BatteryCellVoltage->setText(cellVoltage);
}

void DeviceInfoDialog::chargerInfoHandle(std::vector<uint8_t> data)
{
    Remo_Battery_ChargerInfo_s ci;
    if (data.size() != sizeof(ci)) return;

    memcpy(&ci, data.data(), sizeof(ci));
    QString chargeType;
    if (Power_Supply_Charge_Type_None == ci.Charge_Type) {
        chargeType = QString::fromUtf8("未充电");
    }
    if (Power_Supply_Charge_Type_Fast == ci.Charge_Type) {
        chargeType = QString::fromUtf8("快充电");
    }
    if (Power_Supply_Charge_Type_Pchg == ci.Charge_Type) {
        chargeType = QString::fromUtf8("预充电");
    }
    QString batteryPresent;
    if (Power_Supply_Battery_Present_None == ci.Battery_Present) {
        chargeType = QString::fromUtf8("不在位");
    }
    else if (Power_Supply_Battery_Present == ci.Battery_Present) {
        chargeType = QString::fromUtf8("在位");
    }
    QString adapterPresent;
    if (Power_Supply_Adapter_Present_None == ci.Adapter_Present) {
        adapterPresent = QString::fromUtf8("不在位");
    }
    if (Power_Supply_Adapter_Present == ci.Adapter_Present) {
        adapterPresent = QString::fromUtf8("在位");
    }
    ui->LineEdit_Input_Current->setText(QString::number(ci.Input_Current));
    ui->LineEdit_Input_Voltage->setText(QString::number(ci.Input_Voltage));
    ui->LineEdit_Charge_Voltage->setText(QString::number(ci.Charge_Voltage));
    ui->LineEdit_Charge_Current->setText(QString::number(ci.Charge_Current));
    ui->LineEdit_Charge_Type->setText(chargeType);
    ui->LineEdit_Battery_Present->setText(batteryPresent);
    ui->LineEdit_Adapter_Present->setText(adapterPresent);
}

void DeviceInfoDialog::handle()
{
    if (content.cmdId == Remo_CmdId_Battery_Get_Battery_Info) {
        batteryInfoHandle(content.custom);
    }
    else if (content.cmdId == Remo_CmdId_Battery_Get_Charger_Info) {
        chargerInfoHandle(content.custom);
    }
    else {

    }
}
