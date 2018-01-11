#ifndef DEVICEINFO_H
#define DEVICEINFO_H
#include <vector>
#include <stdint.h>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "batteryinfo.h"

class BatteryInfo;

struct DeviceParsedData
{
    std::set<SubItemData> range;
    std::vector<uint8_t> custom;
    int cmdSet;
    int cmdId;
};

class DeviceInfoInterface : public boost::noncopyable
{
public:
    DeviceInfoInterface();
    void init();
    virtual void batteryInfoHandle(std::vector<uint8_t> data) {}
    virtual void chargerInfoHandle(std::vector<uint8_t> data) {}

private:
    boost::weak_ptr<BatteryInfo> batteryInfo;
};

#endif // DEVICEINFO_H
