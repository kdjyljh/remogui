#include "cmdsender.h"
#include "cmddef.h"
//#include <boost/make_shared.hpp>

boost::shared_ptr<CmdSender> CmdSender::getInstance()
{
    static boost::shared_ptr<CmdSender> instance(new CmdSender);
    return instance;
}

void CmdSender::async_getWorkMode(workModeHandler handler)
{

    auto sender = CommProtoVariables::Get();
    auto msginfo = sender->gen_request_respond(COMMDEVICE_CAMERA, static_cast<CommCmdSetEnum>(Remo_CmdSet_Camera),
                                               static_cast<CommCmdIDEnum>(Remo_CmdId_Camera_Get_WorkMode), true, false, 0,
                                               CommProtoVariables::REQUEST, nullptr, 0);
    int maxRetry = 5; long intervalUS = 50000;
    sender->do_request(msginfo, maxRetry, intervalUS);
}

CmdSender::CmdSender()
{
//    CommProtoVariables::Get()->request_version(COMMDEVICE_CAMERA);
//    CommProtoVariables::Get()->request_status(COMMDEVICE_CAMERA);
    CommAsyncUDP::Get()->register_recvhandler(do_recved_protocol);
}
