#ifndef DEVICEINFODIALOG_H
#define DEVICEINFODIALOG_H
#include "ui_deviceinfo.h"
#include "batteryinfo.h"
#include <QDialog>

class DeviceInfoDialog : public QDialog
{
public:
    DeviceInfoDialog(QWidget *parent = nullptr);

private:
    Ui::DeviceInfo *ui;
    boost::shared_ptr<BatteryInfo> batteryInfo;
};

#endif // DEVICEINFODIALOG_H
