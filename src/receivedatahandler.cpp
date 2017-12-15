#include "receivedatahandler.h"
#include "receivedataproc.h"

ReceiveDataHandler::ReceiveDataHandler()
{
}

boost::shared_ptr<ReceiveDataHandler> ReceiveDataHandler::getInstance()
{
    //multi thread not safe
    static boost::shared_ptr<ReceiveDataHandler> instance(new ReceiveDataHandler);
    ReceiveDataProc::getInstance()->registerHandler(instance);
    return instance;
}

void ReceiveDataHandler::handle()
{
    Remo_CmdSet_e set = static_cast<Remo_CmdSet_e>(data.cmdSet);
    for (auto it : handlerPtrList) {
        if (it->getCmdSet() == set) {
            it->setData(data);
            it->handle();
            break;
        }
    }
}
