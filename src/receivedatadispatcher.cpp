#include "receivedatadispatcher.h"

ReceiveDataDispatcher::ReceiveDataDispatcher()
{
    connect(this, SIGNAL(dataGot(QVariant)), this, SLOT(dataDispatcher(QVariant)));
}

DispatcheType ReceiveDataDispatcher::mapToDispatcher(Remo_CmdSet_e cmdSet, int cmdId)
{
    if (Remo_CmdSet_Camera == cmdSet) {
        if (cmd)
    }
    else if (Remo_CmdSet_Gimbal == cmdSet) {
        return DispatcheType_Gimbal;
    }
    else if (Remo_CmdSet_Battery) {
        return DispatcheType_DeviceInfo;
    }
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
        CmdContent data;
        ReceiveDataHandler::getInstance()->popData(data); //may be block
        QVariant v;
        v.setValue(data);
        emit dataGot(v);
    }
}

void ReceiveDataDispatcher::dataDispatcher(QVariant content)
{
    CmdContent cc = content.value<CmdContent>();
//    Remo_CmdSet_e cmdSet = static_cast<Remo_CmdSet_e>(cc.cmdSet);
//    int cmdId = cc.cmdId;
////    Remo_CmdId_Camera_e idValue = static_cast<Remo_CmdId_Camera_e>(cmdId & 0x1ff);
////    Remo_CmdId_Type_e idType = static_cast<Remo_CmdId_Type_e>(cmdId >> 9);
    DispatcheType type = mapToDispatcher(static_cast<Remo_CmdSet_e>(cc.cmdSet), cc.cmdId);
    for (auto it : handlerPtrList) {
        if (it && it->getType() == type) {
            it->setContent(content);
            it->handle();
        }
    }
}
