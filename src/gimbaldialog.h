#ifndef GIMBALDIALOG_H
#define GIMBALDIALOG_H
#include "protocoldataInterfaceimpl.h"
#include "ui_gimbal.h"

#include <QDialog>
#include <QMessageBox>

class GimbalDialog : public QDialog, public ProtocolDataInterfaceImpl
{
    Q_OBJECT
public:
    GimbalDialog(QWidget *parent = nullptr);

    virtual void handle() override;

private:
    Ui::Gimbal *ui;
    bool relaAttiAngleFineTuneStop;

    void getDeviceInfoAndCurrentValue();
    template<class DataSizeType>
    bool LineEdit_SendCmd(QLineEdit *lineEdite, int dataSize, int sendDataRangeSize, int showDataRangeSize, int sendNotSetValue, int showNotSetValue, int cmdId);


private slots:
    void lineEdit_returnPressed();

    void on_ComboBox_Calibrate_activated(int index);
    void on_ComboBox_InterPolation_activated(int index);
    void on_ComboBox_LockAxis_activated(int index);
    void on_ComboBox_Roll_WorkMode_activated(int index);
    void on_pushButton_Reset_Gimbal_clicked();
    void on_pushButton_FactoryReset_clicked();
    void on_pushButton_getGimbalInfo_clicked();
    void pushButton_RelaAttiAngle_FineTune_pressed();
    void pushButton_RelaAttiAngle_FineTune_released();
};


template<class DataSizeType>
bool GimbalDialog::LineEdit_SendCmd(QLineEdit *lineEdite, int dataSize, int sendDataRangeSize, int showDataRangeSize, int sendNotSetValue, int showNotSetValue, int cmdId)
{
    LOG(INFO) << "GimbalDialog::LineEdit_SendCmd";
    if (nullptr == lineEdite) return false;
    QStringList strList = lineEdite->text().split(',', QString::SkipEmptyParts);
    for (auto it : strList) {
        it = it.trimmed();
    }
    bool * ok = new bool(false);
    if (strList.size() == dataSize) {
        DataSizeType data[dataSize]{0};
        int i = 0;
        for (auto it : strList) {
            uint16_t value = sendDataRangeSize / showDataRangeSize * it.toInt(ok);
            if (!*ok) goto error;
            //设置云台角速度-时间参考值特殊处理
            if (ui->LineEdit_AnguVeloInter_Set == lineEdite && i >= 3) {
                value = 1000 * it.toInt(ok);
            }
            if (showNotSetValue == it.toInt()) value = sendNotSetValue; //0表示对应的值保持不变
            data[i++] = value;
        }
        GimbalDialog::sendCmdGimbal(static_cast<Remo_CmdId_Gimbal_e>(cmdId),
                      std::vector<uint8_t>(reinterpret_cast<uint8_t*>(&data), reinterpret_cast<uint8_t*>(&data) + sizeof(data)));
        delete ok;
        QMessageBox::information(nullptr, QString::fromUtf8("消息"), QString::fromUtf8("命令发送成功"),QMessageBox::Ok);
        return true;
    }
    error:
    delete ok;
    QMessageBox::warning(nullptr, QString::fromUtf8("警告"), QString::fromUtf8("数据格式错误"), QMessageBox::Ok);
    return true;
}

#endif // GIMBALDIALOG_H
