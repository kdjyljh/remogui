#include "cameraworkmode.h"

CameraWorkMode::CameraWorkMode() :
    ProtocolDataInterface(Remo_CmdSet_Camera)
{
}

void CameraWorkMode::handle()
{
    Remo_CmdId_e id = static_cast<Remo_CmdId_e>(data.cmdID);
    if (Remo_CmdId_Camera_Get_WorkMode == id) {
        workModeGot(*reinterpret_cast<Remo_Camera_WorkMode_S*>(data.data.data()));
    }
    else if (0x01 == id) {

    }
    else if (Remo_CmdId_Camera_Get_CapDelayTime == id) {
//        capDelayTimeGot(static_cast<int>(*data.data.data()));
        cameraSettingGot(data.data, id);
    }
    else if (Remo_CmdId_Camera_Get_CapDelayTime_Range == id) {
        std::set<SubItemData> range;
        getSurportRange(range);
        surportRangeGot(range, id);
    }
    else if (0x35 == id) {

    }

    if (0xa0 > id && 0x10 <= id) {

    }
}

void CameraWorkMode::sendCmd(Remo_CmdId_e cmdId, std::vector<uint8_t> data, CommProtoVariables::RequestRespond reqres)
{
    ProtocolDataInterface::sendCmd(COMMDEVICE_CAMERA, Remo_CmdSet_Camera, cmdId, true, false, reqres, data);
}

void CameraWorkMode::async_setWorkMode(const Remo_Camera_WorkMode_s & workmode)
{
    std::vector<uint8_t> data;
    data.push_back(static_cast<uint8_t>(workmode.MainWorkMode));
    data.push_back(static_cast<uint8_t>(workmode.SubWorkMode));
    sendCmd(Remo_CmdId_Camera_Set_WorkMode, data);
}

//void CameraWorkMode::async_setCapDelayTime()
//{
//    std::vector<uint8_t> data;
//    data.push_back(static_cast<uint8_t>(workmode.MainWorkMode));
//    sendCmd(Remo_CmdId_Camera_Set_CapDelayTime, data);
//}
