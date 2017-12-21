#include "receivedataproc.h"
#include "../thirdparty/shareddata.h"
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
    if (nullptr != handler_) {
        handler_->setData(ps);
        handler_->handle();
    }
}
