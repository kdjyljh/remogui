#include "protocoldataInterfaceimpl.h"
#include "commlog.h"

ProtocolDataInterfaceImpl::ProtocolDataInterfaceImpl(DispatcheType type) :
    ProtocolDataInterface(type)
{
}

void ProtocolDataInterfaceImpl::handle()
{
    Remo_CmdId_Camera_e cmdId = static_cast<Remo_CmdId_Camera_e>(content.cmdId);
    Remo_CmdId_Type_e idType = static_cast<Remo_CmdId_Type_e>(cmdId >> 9);

    if (content.ret == Return_OK || content.isRestore) {
        if (Remo_CmdId_Camera_Get_WorkMode == cmdId || Remo_CmdId_Camera_Set_WorkMode == cmdId) {
            if (content.custom.size() == 2) {
                Remo_Camera_WorkMode_s workMode{};
                memcpy(&workMode, content.custom.data(), 2);
                workModeGot(workMode);
            }
        }
        else if (CmdId_Type_Get == idType) {
            settingGot(content.custom, cmdId);
        }
        else if (CmdId_Type_GetRange == idType) {
            surportRangeGot(content.range, cmdId);
        }
        else if (CmdId_Type_Control == idType) {
            controlGot();
        }
        else {

        }
    }

    retProcess(content);
}

void ProtocolDataInterfaceImpl::async_setWorkMode(const Remo_Camera_WorkMode_s & workmode)
{
    std::vector<uint8_t> data;
    data.push_back(static_cast<uint8_t>(workmode.MainWorkMode));
    data.push_back(static_cast<uint8_t>(workmode.SubWorkMode));
    sendCmdCamera(Remo_CmdId_Camera_Set_WorkMode, data);
}
