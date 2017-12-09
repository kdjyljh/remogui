#include "protocaldatainterface.h"

ProtocalDataInterface::ProtocalDataInterface(Remo_CmdSet_e set) :
    cmdSet(set)
{
    //can't regist self here
//    ReceiveDataHandler::getInstance()->registerDataHandler(shared_from_this());
}
