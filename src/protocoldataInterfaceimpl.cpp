#include "protocoldataInterfaceimpl.h"

ProtocolDataInterfaceImpl::ProtocolDataInterfaceImpl() :
    ProtocolDataInterface()
{
}

void ProtocolDataInterfaceImpl::handle()
{
    Remo_CmdId_e idValue = static_cast<Remo_CmdId_e>(data.cmdID);
    Remo_CmdId_e cmdId = static_cast<Remo_CmdId_e>(idValue & 0x1ff);
    Remo_CmdId_Type_e idType = static_cast<Remo_CmdId_Type_e>(idValue >> 9);

    std::cout << "ProtocolDataInterfaceImpl::handle idValue = " << idValue
              << " cmdId = " << cmdId << " idType = " << idType << std::endl;

//    std::set<SubItemData> range;
//    getSurportRange(range);
//    surportRangeGot(range, idValue);

    if (CmdId_Type_Get == idType) {
        if (Remo_CmdId_Camera_Get_WorkMode == idValue) {
            workModeGot(*reinterpret_cast<Remo_Camera_WorkMode_S*>(data.data.data()));
            return;
        }
        settingGot(data.data, idValue);
    }
    else if (CmdId_Type_Set == idType) {
        Remo_CmdId_SetCmd_ReturnValue_e ret = static_cast<Remo_CmdId_SetCmd_ReturnValue_e>(data.data.at(0));

        if (ret != Return_OK) {
            std::cerr << "set camera failed, exit!!!!!!!!!!!!!!!!!!!!!!!!\n";
            exit(-1);//设置相机失败，直接退出程序
        }
    }
    else if (CmdId_Type_GetRange == idType) {
        std::set<SubItemData> range;
        getSurportRange(range);
        surportRangeGot(range, idValue);
    }
    else if (CmdId_Type_Control == idType) {
        controlGot();
    }
    else {
        workModeGot(*reinterpret_cast<Remo_Camera_WorkMode_S*>(data.data.data()));
    }
//    if (Remo_CmdId_Camera_Get_WorkMode == id) {
//        workModeGot(*reinterpret_cast<Remo_Camera_WorkMode_S*>(data.data.data()));
//    }
//    else if (0x01 == id) {

//    }
//    else if (Remo_CmdId_Camera_Get_CapDelayTime == id ||
//             Remo_CmdId_Camera_Get_WhiteBalance == id) {
////        capDelayTimeGot(static_cast<int>(*data.data.data()));
//        cameraSettingGot(data.data, id);
//    }
//    else if (Remo_CmdId_Camera_Get_CapDelayTime_Range == id ||
//             Remo_CmdId_Camera_Get_WhiteBalance_Range == id) {
//        std::set<SubItemData> range;
//        getSurportRange(range);
//        surportRangeGot(range, id);
//    }
//    else if (Remo_CmdId_Camera_Set_CapDelayTime == id ||
//             Remo_CmdId_Camera_Set_WhiteBalance == id) {
//        Remo_CmdId_SetCmd_ReturnValue_e ret = static_cast<Remo_CmdId_SetCmd_ReturnValue_e>(data.data.at(0));

//        if (ret != Return_OK) {
//            exit(-1);//设置相机失败，直接退出程序
//        }
//    }

//    if (0xa0 > id && 0x10 <= id) {

//    }
}

void ProtocolDataInterfaceImpl::sendCmdCamera(Remo_CmdId_e cmdId, std::vector<uint8_t> data, CommProtoVariables::RequestRespond reqres)
{
    ProtocolDataInterface::sendCmdCamera(COMMDEVICE_CAMERA, Remo_CmdSet_Camera, cmdId, true, false, reqres, data);
}

void ProtocolDataInterfaceImpl::async_setWorkMode(const Remo_Camera_WorkMode_s & workmode)
{
    std::vector<uint8_t> data;
    data.push_back(static_cast<uint8_t>(workmode.MainWorkMode));
    data.push_back(static_cast<uint8_t>(workmode.SubWorkMode));
    sendCmdCamera(Remo_CmdId_Camera_Set_WorkMode, data);
}

//void ProtocolDataInterfaceImpl::async_setCapDelayTime()
//{
//    std::vector<uint8_t> data;
//    data.push_back(static_cast<uint8_t>(workmode.MainWorkMode));
//    sendCmdCamera(Remo_CmdId_Camera_Set_CapDelayTime, data);
//}
