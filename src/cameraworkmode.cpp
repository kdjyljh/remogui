#include "cameraworkmode.h"

CameraWorkMode::CameraWorkMode() :
    ProtocalDataInterface(Remo_CmdSet_Camera)
{
}

void CameraWorkMode::handle()
{
    Remo_CmdId_e id = static_cast<Remo_CmdId_e>(data.cmdID);
    if (0x00 == id) {
        workModeGot(*reinterpret_cast<Remo_Camera_WorkMode_S*>(data.data.data()));
    }
}

void CameraWorkMode::async_getWorkMode()
{
    auto sender = CommProtoVariables::Get();
    auto msginfo = sender->gen_request_respond(COMMDEVICE_CAMERA, static_cast<CommCmdSetEnum>(Remo_CmdSet_Camera),
                                               static_cast<CommCmdIDEnum>(Remo_CmdId_Camera_Get_WorkMode), true, false, 0,
                                               CommProtoVariables::REQUEST, nullptr, 0);
    int maxRetry = 5; long intervalUS = 50000;
    sender->do_request(msginfo, maxRetry, intervalUS);
}
