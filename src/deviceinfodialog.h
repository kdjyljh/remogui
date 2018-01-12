#ifndef DEVICEINFODIALOG_H
#define DEVICEINFODIALOG_H
#include "ui_deviceinfo.h"
#include "protocoldataInterfaceimpl.h"
#include <QDialog>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

class DeviceInfoDialog : public QDialog, public ProtocolDataInterfaceImpl
{
public:
    static boost::shared_ptr<DeviceInfoDialog> createInstance(QWidget *parent = nullptr);

    void batteryInfoHandle(std::vector<uint8_t> data);
    void chargerInfoHandle(std::vector<uint8_t> data);
    virtual void handle() override;

private:
    DeviceInfoDialog(QWidget *parent = nullptr);

private:
    Ui::DeviceInfo *ui;
};

#endif // DEVICEINFODIALOG_H
