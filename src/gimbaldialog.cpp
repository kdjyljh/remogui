#include "gimbaldialog.h"
#include <QDebug>

GimbalDialog::GimbalDialog(QWidget *parent) :
    QDialog(parent),
    ProtocolDataInterfaceImpl(Remo_CmdSet_Gimbal),
    ui(new Ui::Gimbal)
{
    ui->setupUi(this);

    ui->ComboBox_Calibrate->insertItem(0, QString::fromUtf8("Roll轴电角度对齐"), 0);
    ui->ComboBox_Calibrate->insertItem(1, QString::fromUtf8("Pitch轴电角度对齐"), 1);
    ui->ComboBox_Calibrate->insertItem(2, QString::fromUtf8("Yaw轴电角度对齐"), 2);
    ui->ComboBox_Calibrate->insertItem(3, QString::fromUtf8("加速度计校正"), 3);
    ui->ComboBox_Calibrate->insertItem(4, QString::fromUtf8("角速度计校正"), 4);
    ui->ComboBox_Calibrate->insertItem(5, QString::fromUtf8("云台标定"), 5);

    ui->ComboBox_InterPolation->insertItem(0, QString::fromUtf8("使用阶跃方式插值"), 0);
    ui->ComboBox_InterPolation->insertItem(1, QString::fromUtf8("使用线性插值"), 1);
    ui->ComboBox_InterPolation->insertItem(2, QString::fromUtf8("使用自然插值"), 2);

    ui->ComboBox_LockAxis->insertItem(0, QString::fromUtf8("Roll轴锁定"), 0);
    ui->ComboBox_LockAxis->insertItem(1, QString::fromUtf8("Pitch轴锁定"), 1);
    ui->ComboBox_LockAxis->insertItem(2, QString::fromUtf8("Yaw轴锁定"), 2);

    ui->ComboBox_Roll_WorkMode->insertItem(0, QString::fromUtf8("横拍模式"), 0);
    ui->ComboBox_Roll_WorkMode->insertItem(1, QString::fromUtf8("竖拍模式"), 1);

    connect(ui->LineEdit_AbsAttiAngle_Set, SIGNAL(returnPressed()), this, SLOT(lineEdit_returnPressed()));
    connect(ui->LineEdit_AnguVelo_Set, SIGNAL(returnPressed()), this, SLOT(lineEdit_returnPressed()));
    connect(ui->LineEdit_AnguVeloInter_Set, SIGNAL(returnPressed()), this, SLOT(lineEdit_returnPressed()));
    connect(ui->LineEdit_RelaAttiAngle_Set, SIGNAL(returnPressed()), this, SLOT(lineEdit_returnPressed()));
    connect(ui->LineEdit_RollFineTuning, SIGNAL(returnPressed()), this, SLOT(lineEdit_returnPressed()));
    connect(ui->LineEdit_VeloSlope, SIGNAL(returnPressed()), this, SLOT(lineEdit_returnPressed()));

    getDeviceInfoAndCurrentValue();
}

void GimbalDialog::handle()
{
    Remo_CmdId_Gimbal_e cmdId;
    std::vector<uint8_t> valueData = ProtocolDataInterface::data.data;
    if (Remo_CmdId_Gimbal_Get_AttiAngle == cmdId) {
        if (valueData.size() != 6) {
            LOG(INFO) << "data字段数据错误" << "cmdId = " << cmdId;
            return;
        }
        uint16_t XAxisAngle, YAxisAngle, ZAxisAngle;
        memcpy(&XAxisAngle, valueData.data(), 2);
        memcpy(&YAxisAngle, valueData.data() + 2, 2);
        memcpy(&ZAxisAngle, valueData.data() + 4, 2);
        float XAxisAngleF = 180 / 32767  * XAxisAngle;
        float YAxisAngleF = 180 / 32767  * YAxisAngle;
        float ZAxisAngleF = 180 / 32767  * ZAxisAngle;
        QString showStr = QString::number(XAxisAngleF, 'f', 2) + ",";
        showStr += QString::number(YAxisAngleF, 'f', 2) + ",";
        showStr += QString::number(ZAxisAngleF, 'f', 2);
        ui->LineEdit_AttiAngle_Get->setText(showStr);
    }
    else if (Remo_CmdId_Gimbal_Get_AnguVelo == cmdId) {
        if (valueData.size() != 6) {
            LOG(INFO) << "data字段数据错误" << "cmdId = " << cmdId;
            return;
        }
        uint16_t XaxisAnguVelo, YaxisAnguVelo, ZaxisAnguVelo;
        memcpy(&XaxisAnguVelo, valueData.data(), 2);
        memcpy(&YaxisAnguVelo, valueData.data() + 2, 2);
        memcpy(&ZaxisAnguVelo, valueData.data() + 4, 2);
        float XaxisAnguVeloF = 360 / 32767  * XaxisAnguVelo;
        float YaxisAnguVeloF = 360 / 32767  * YaxisAnguVelo;
        float ZaxisAnguVeloF = 360 / 32767  * ZaxisAnguVelo;
        QString showStr = QString::number(XaxisAnguVeloF, 'f', 2) + ",";
        showStr += QString::number(YaxisAnguVeloF, 'f', 2) + ",";
        showStr += QString::number(ZaxisAnguVeloF, 'f', 2);
        ui->LineEdit_AnguVelo_Get->setText(showStr);
    }
    else if (Remo_CmdId_Gimbal_Get_DeviceInfo == cmdId) {
        if (8 != valueData.size()) {
            LOG(INFO) << "data字段数据错误" << "cmdId = " << cmdId;
            return;
        }
        QString showStr;
        uint8_t * valuePtr = valueData.data();
        showStr += "主控硬件版本号:" + QString::number(*valuePtr++) + "\n";
        showStr += "主控软件版本号:" + QString::number(*valuePtr++) + "\n";
        showStr += "电调硬件版本号:" + QString::number(*valuePtr++) + "\n";
        showStr += "电调软件版本号:" + QString::number(*valuePtr++) + "\n";
        uint32_t u32 = 0;
        memcpy(&u32, valuePtr, 4);
        showStr += "出厂序列号:" + QString::number(u32) + "\n";
        ui->textEdit_DeviceInfo->setText(showStr.toLocal8Bit());
    }
    else {
    }
}

void GimbalDialog::getDeviceInfoAndCurrentValue()
{
    sendCmdGimbal(Remo_CmdId_Gimbal_Get_AttiAngle);
    sendCmdGimbal(Remo_CmdId_Gimbal_Get_AnguVelo);
    sendCmdGimbal(Remo_CmdId_Gimbal_Get_DeviceInfo);
}

void GimbalDialog::lineEdit_returnPressed()
{
    QLineEdit * lineEdite = dynamic_cast<QLineEdit*>(sender());
    if (nullptr == lineEdite) return;

    if (ui->LineEdit_AbsAttiAngle_Set == lineEdite) {
        LineEdit_SendCmd<uint16_t>(lineEdite, 3, 32767, 180, -32768, 0, Remo_CmdId_Gimbal_Set_AbsAttiAngle);
    }
    else if (ui->LineEdit_RelaAttiAngle_Set == lineEdite) {
        LineEdit_SendCmd<uint16_t>(lineEdite, 3, 32767, 360, -32768, 0, Remo_CmdId_Gimbal_Set_RelaAttiAngle);
    }
    else if (ui->LineEdit_AnguVelo_Set == lineEdite) {
        LineEdit_SendCmd<uint16_t>(lineEdite, 3, 32767, 360, -32768, 0, Remo_CmdId_Gimbal_Set_AnguVelo);
    }
    else if (ui->LineEdit_AnguVeloInter_Set == lineEdite) {
        LineEdit_SendCmd<uint16_t>(lineEdite, 6, 32767, 360, -32768, 0, Remo_CmdId_Gimbal_Set_AnguVeloInter);
    }
    else if (ui->LineEdit_RollFineTuning == lineEdite) {
        LineEdit_SendCmd<uint8_t>(lineEdite, 1, 127, 10, -128, 0, Remo_CmdId_Gimbal_Set_RollFineTuning);
    }
    else if (ui->LineEdit_VeloSlope == lineEdite) {
        LineEdit_SendCmd<uint8_t>(lineEdite, 3, 20, 2, 100, 0, Remo_CmdId_Gimbal_Set_VeloSlope);
    }
    else {

    }
}

void GimbalDialog::on_ComboBox_Calibrate_activated(int index)
{
    int value = ui->ComboBox_Calibrate->itemData(index).toInt();
    std::vector<uint8_t> data(6, 0);
    data[value] = 1;
    sendCmdGimbal(Remo_CmdId_Gimbal_Start_Calibrate, data);
}

void GimbalDialog::on_ComboBox_InterPolation_activated(int index)
{
    int value = ui->ComboBox_InterPolation->itemData(index).toInt();
    sendCmdGimbal(Remo_CmdId_Gimbal_Set_InterPolation, std::vector<uint8_t>{value});
}

void GimbalDialog::on_ComboBox_Roll_WorkMode_activated(int index)
{
    int value = ui->ComboBox_LockAxis->itemData(index).toInt();
    sendCmdGimbal(Remo_CmdId_Gimbal_Set_Roll_WorkMode, std::vector<uint8_t>{value});
}

void GimbalDialog::on_pushButton_Reset_clicked()
{
    sendCmdGimbal(Remo_CmdId_Gimbal_Reset, std::vector<uint8_t>{1});
}

void GimbalDialog::on_pushButton_FactoryReset_clicked()
{
    sendCmdGimbal(Remo_CmdId_Gimbal_FactoryReset, std::vector<uint8_t>{1});
}

void GimbalDialog::on_pushButton_getGimbalInfo_clicked()
{
    getDeviceInfoAndCurrentValue();
}

void GimbalDialog::on_ComboBox_LockAxis_activated(int index)
{
    int value = ui->ComboBox_LockAxis->itemData(index).toInt();
    std::vector<uint8_t> data(3, 0);
    data[value] = 1;
    sendCmdGimbal(Remo_CmdId_Gimbal_Set_LockAxis, data);
}
