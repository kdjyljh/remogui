#include "receivedatahandler.h"
#include "receivedataproc.h"

ReceiveDataHandler::ReceiveDataHandler()
{
//    ReceiveDataProc::getInstance()->registerHandler(shared_from_this());
}

//Remo_CmdType_e ReceiveDataHandler::map2CmdType()
//{
//    Remo_CmdId_e id = data->cmdID;
//    Remo_CmdSet_e set = data->cmdSet;
//    if (set == Remo_CmdSet_Camera && 0xa0 > id && 0x00 <= id) {
//        return Remo_CmdType_Camera_WorkMode;
//    }
//    else if (set == Remo_CmdSet_Camera && 0x200 > id && 0xa0 <= id) {
//        return Remo_CmdType_Camera_AdvancedSetting;
//    }
//    else {
//        return Remo_CmdType_Camera_WorkMode;
//    }
//}

boost::shared_ptr<ReceiveDataHandler> ReceiveDataHandler::getInstance()
{
    //multi thread no safe
    static boost::shared_ptr<ReceiveDataHandler> instance(new ReceiveDataHandler);
    ReceiveDataProc::getInstance()->registerHandler(instance);
    return instance;
}

void ReceiveDataHandler::handle()
{
    Remo_CmdSet_e set = static_cast<Remo_CmdSet_e>(data.cmdSet);
    for (auto it : handlerList) {
        if (it.getCmdSet() == set) {
            it.setData(data);
            it.handle();
            break;
        }
    }
}
