#ifndef BATTERYINFO_H
#define BATTERYINFO_H

#include "protocoldataInterfaceimpl.h"
#include "deviceinfo.h"
#include <boost/shared_ptr.hpp>

class DeviceInfoInterface;

class BatteryInfo : public ProtocolDataInterfaceImpl
{
public:
    BatteryInfo();
    void setHandler(boost::shared_ptr<DeviceInfoInterface> handle);

    virtual void handle() override;

private:
    boost::shared_ptr<DeviceInfoInterface> batteryHandle;
};

#endif // BATTERYINFO_H
