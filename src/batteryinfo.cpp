//#include "batteryinfo.h"

//BatteryInfo::BatteryInfo() :
//    ProtocolDataInterfaceImpl(Remo_CmdSet_Battery)
//{

//}

//void BatteryInfo::setHandler(boost::shared_ptr<DeviceInfoInterface> handle)
//{
//    batteryHandle = handle;
//}

//void BatteryInfo::handle()
//{
//    if (batteryHandle) {
//        if (data.cmdID == Remo_CmdId_Battery_Get_Battery_Info) {
//            batteryHandle->batteryInfoHandle(data.data);
//        }
//        else if (data.cmdID == Remo_CmdId_Battery_Get_Charger_Info) {
//           batteryHandle->chargerInfoHandle(data.data);
//        }
//        else {

//        }
//    }
//}
