#include "deviceinfo.h"

boost::shared_ptr<BatteryInfo> bi(new BatteryInfo);
DeviceInfoInterface::DeviceInfoInterface()
{
}

void DeviceInfoInterface::init()
{
    boost::shared_ptr<DeviceInfoInterface> instance(new DeviceInfoInterface());
    if (bi) {
        instance->batteryInfo = bi;
        instance->batteryInfo.lock()->setHandler(instance);
        instance->batteryInfo.lock()->registerSelf2Handler();
    }
}
