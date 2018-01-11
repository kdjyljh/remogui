#include "receivedatadispatcher.h"

ReceiveDataDispatcher::ReceiveDataDispatcher()
{

}

boost::shared_ptr<ReceiveDataDispatcher> ReceiveDataDispatcher::getInstance()
{
    //multi thread not safe
    static boost::shared_ptr<ReceiveDataDispatcher> instance(new ReceiveDataDispatcher);
    return instance;
}

void ReceiveDataDispatcher::run()
{
    while(true) {
        ParsedData data;
        ReceiveDataHandler::getInstance()->popData(data); //may be block
        emit dataGot(data);
    }
}

void ReceiveDataDispatcher::dataDispatcher(ParsedData data)
{
    Remo_CmdSet_e cmdSet = data.cmdSet;
    int cmdId = data.cmdId;
    Remo_CmdId_Camera_e idValue = static_cast<Remo_CmdId_Camera_e>(cmdId & 0x1ff);
    Remo_CmdId_Type_e idType = static_cast<Remo_CmdId_Type_e>(cmdId >> 9);
    for (auto it : handlerPtrList) {
        if (it && it->getCmdSet() == cmdSet) {
            it->setData(data);
            it->handle();
        }
    }
}
