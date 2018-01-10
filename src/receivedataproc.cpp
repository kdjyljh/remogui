#include "receivedataproc.h"
#include "../thirdparty/shareddata.h"
#include "../thirdparty/commlog.h"
#include "camerafielddef.h"
#include "cmddef.h"

#include <boost/make_shared.hpp>
#include <stdlib.h>
#include <QDebug>

ReceiveDataProc::ReceiveDataProc()
{
    CommAsyncUDP::Get()->register_recvhandler(do_recved_protocol);
}

boost::shared_ptr<ReceiveDataProc> ReceiveDataProc::getInstance()
{
    static boost::shared_ptr<ReceiveDataProc> instance(new ReceiveDataProc);
    return instance;
}

void ReceiveDataProc::start()
{
    if (nullptr == handler_) {
        registerHandler(ReceiveDataHandler::getInstance());
        if (nullptr == handler_) {
            qDebug() << "No data handler registered!!!!";
            return;
        }
    }

    thread_ = boost::make_shared<boost::thread>(&ReceiveDataProc::run, this);
}

void ReceiveDataProc::run()
{
    while (true) {
        ProtocolStruct protoData;
        //popData maybe blocked
        if (SharedData::Get()->popReceiveData(protoData))
            protocolStructProc(protoData);
    }
}

void ReceiveDataProc::protocolStructProc(const ProtocolStruct &ps)
{
    int cmdId = ps.cmdID;
    Remo_CmdId_Camera_e idValue = static_cast<Remo_CmdId_Camera_e>(cmdId & 0x1ff);
    Remo_CmdId_Type_e idType = static_cast<Remo_CmdId_Type_e>(cmdId >> 9);
    LOG(INFO) << "#########################ReceiveDataProc::protocolStructProc" << " cmdSet = " << std::hex << ps.cmdSet << " cmdId = " << cmdId
              << " idValue = " << idValue << " idType = " << idType << std::endl;
    LOG(INFO) << "data is ";
    CHAR_BUFF_TO_LOG_STDERROR(ps.data);

    if (nullptr != handler_) {
        handler_->setData(ps);
        handler_->handle();
    }
}
