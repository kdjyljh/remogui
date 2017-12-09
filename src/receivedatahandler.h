#ifndef RECEIVEDATAHANDLER_H
#define RECEIVEDATAHANDLER_H
#include "../thirdparty/Protocol.hpp"
#include "cmddef.h"
#include "protocaldatainterface.h"
#include <boost/shared_ptr.hpp>

class ProtocalDataInterface;

class ReceiveDataHandler : public boost::noncopyable
{
public:
    static boost::shared_ptr<ReceiveDataHandler> getInstance();

    void registerDataHandler(const ProtocalDataInterface & handler) {
        handlerList.push_back(handler);
    }

    virtual void handle();

    void setData(const ProtocolStruct & data) {this->data = data;}
    const ProtocolStruct getData() {return data;}

private:
    ReceiveDataHandler();
//    Remo_CmdType_e map2CmdType();

private:
    std::vector<ProtocalDataInterface> handlerList;
    ProtocolStruct data;
};

#endif // RECEIVEDATAHANDLER_H
