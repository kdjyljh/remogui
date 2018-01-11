#include "protocoldataInterfaceimpl.h"
#include "../thirdparty/commlog.h"

ProtocolDataInterfaceImpl::ProtocolDataInterfaceImpl(DispatcheType type) :
    ProtocolDataInterface(type)
{
}

void ProtocolDataInterfaceImpl::handle()
{
    Remo_CmdId_Camera_e cmdId = static_cast<Remo_CmdId_Camera_e>(content.cmdId);
//    Remo_CmdId_Camera_e idValue = static_cast<Remo_CmdId_Camera_e>(cmdId & 0x1ff);
    Remo_CmdId_Type_e idType = static_cast<Remo_CmdId_Type_e>(cmdId >> 9);

//    LOG(INFO) << "#########################ProtocolDataInterfaceImpl::handle idValue = " << idValue
//              << " cmdId = " << cmdId << " idType = " << idType << std::endl;
//    LOG(INFO) << "data is ";
//    CHAR_BUFF_TO_LOG_STDERROR(data.data);

    if (Remo_CmdId_Camera_Get_WorkMode == cmdId || Remo_CmdId_Camera_Set_WorkMode == cmdId) {
        Remo_Camera_WorkMode_s workMode{0};
        memcpy(&workMode, content.custom.data(), 2);
        workModeGot(workMode);
        return;
    }

    if (CmdId_Type_Get == idType) {
        settingGot(content.custom, cmdId);
    }
//    else if (CmdId_Type_Set == idType) {
//        Remo_CmdId_SetCmd_ReturnValue_e ret = static_cast<Remo_CmdId_SetCmd_ReturnValue_e>(data.data.at(0));

//        if (ret != Return_OK) {
//            std::cerr << "set camera failed, exit!!!!!!!!!!!!!!!!!!!!!!!!\n";
//            exit(-1);//设置相机失败，直接退出程序
//        }
//    }
    else if (CmdId_Type_GetRange == idType) {
        std::set<SubItemData> range;
        getSurportRange(range);
        surportRangeGot(range, cmdId);
    }
    else if (CmdId_Type_Control == idType) {
        controlGot();
    }
    else {

    }
}

void ProtocolDataInterfaceImpl::sendCmdCamera(Remo_CmdId_Camera_e cmdId, std::vector<uint8_t> data, CommProtoVariables::RequestRespond reqres)
{
    ProtocolDataInterface::sendCmd(COMMDEVICE_CAMERA, Remo_CmdSet_Camera, cmdId, true, false, reqres, data);
}

void ProtocolDataInterfaceImpl::sendCmdCamera(Remo_CmdId_Camera_e cmdId, int maxRety, long intervalUS, std::vector<uint8_t> data, CommProtoVariables::RequestRespond reqres)
{
    ProtocolDataInterface::sendCmd(COMMDEVICE_CAMERA, Remo_CmdSet_Camera, cmdId, true, false, reqres, data, maxRety, intervalUS);
}

void ProtocolDataInterfaceImpl::sendCmdGimbal(Remo_CmdId_Gimbal_e cmdId, std::vector<uint8_t> data, CommProtoVariables::RequestRespond reqres)
{
    ProtocolDataInterface::sendCmd(COMMDEVICE_GIMBAL, Remo_CmdSet_Gimbal, cmdId, true, false, reqres, data);
}

void ProtocolDataInterfaceImpl::sendCmdBattery(Remo_CmdId_Battery_e cmdId, std::vector<uint8_t> data, CommProtoVariables::RequestRespond reqres)
{
    ProtocolDataInterface::sendCmd(COMMDEVICE_BATTERY, Remo_CmdSet_Gimbal, cmdId, true, false, reqres, data);
}

void ProtocolDataInterfaceImpl::async_setWorkMode(const Remo_Camera_WorkMode_s & workmode)
{
    std::vector<uint8_t> data;
    data.push_back(static_cast<uint8_t>(workmode.MainWorkMode));
    data.push_back(static_cast<uint8_t>(workmode.SubWorkMode));
    sendCmdCamera(Remo_CmdId_Camera_Set_WorkMode, data);
}
