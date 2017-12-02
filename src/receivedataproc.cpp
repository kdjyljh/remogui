#include "receivedataproc.h"
#include "../thirdparty/shareddata.h"
#include "../thirdparty/Protocol.hpp"

#include <boost/make_shared.hpp>
#include <QDebug>

void fun(){}
ReceiveDataProc::ReceiveDataProc(QObject *parent) :
    QObject(parent),
    dataHandler(nullptr)
{
}

void ReceiveDataProc::start()
{
    if (nullptr == dataHandler) {
        qDebug() << "No data handler registered!!!!";
        return;
    }

    thread_ = boost::make_shared<boost::thread>(&ReceiveDataProc::run, this);
}

void ReceiveDataProc::run()
{
    while (true) {
//        ReceivedDataType d;
//        d.push_back(100);
//        dataHandler(d);
        ProtocolStruct protoData;
        SharedData::Get()->popData(protoData);
        if (protoData.data.size() == 0) {
            continue;
        }

        dataHandler(protoData.data);
    }
}
