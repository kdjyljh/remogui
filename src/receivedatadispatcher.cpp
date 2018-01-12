#include "receivedatadispatcher.h"

ReceiveDataDispatcher::ReceiveDataDispatcher()
{
    connect(this, SIGNAL(dataGot(QVariant)), this, SLOT(dataDispatcher(QVariant)));
}

DispatcheType ReceiveDataDispatcher::mapToDispatcher(Remo_CmdSet_e cmdSet, int cmdId)
{
    if (Remo_CmdSet_Camera == cmdSet) {
        if ((cmdId & 0x1ff) >= 0x67 && (cmdId & 0x1ff) < 0x78) {
            return DispatcheType_AeMode;
        }
        else if (((cmdId & 0x1ff) >= 0x7b && (cmdId & 0x1ff) < 0x85)) {
            return DispatcheType_Focus_Zoom;
        }
        else if (((cmdId & 0x1ff) >= 0x0 && (cmdId & 0x1ff) < 0x60)) {
            return DispatcheType_WorkMode;
        }
        else {
            return DispatcheType_CameraDefault;
        }
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
    DispatcheType type = mapToDispatcher(static_cast<Remo_CmdSet_e>(cc.cmdSet), cc.cmdId);
    for (auto it : handlerPtrList) {
        if (it && it->getType() == type) {
            it->setContent(cc);
            it->handle();
        }
    }
}
