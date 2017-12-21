#ifndef RECEIVEDATAHANDLER_H
#define RECEIVEDATAHANDLER_H
#include "../thirdparty/Protocol.hpp"
#include "cmddef.h"
#include "protocaldatainterface.h"
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/unordered_map.hpp>

class ProtocolDataInterface;

class ReceiveDataHandler : public boost::noncopyable
{
public:
    static boost::shared_ptr<ReceiveDataHandler> getInstance();

    void registerDataHandler(boost::shared_ptr<ProtocolDataInterface> handler) {
        for (auto it : handlerPtrList) {
            if (it == handler) return;
        }
        handlerPtrList.push_back(handler);
    }

    virtual void handle();

    void setData(const ProtocolStruct & data) {this->data = data;}
    const ProtocolStruct getData() {return data;}

private:
    ReceiveDataHandler();
//    Remo_CmdType_e map2CmdType();

private:
    //must be ptr, using virtual funciton handle
    std::vector<boost::shared_ptr<ProtocolDataInterface>> handlerPtrList;
    ProtocolStruct data;
    static boost::unordered_map<uint32_t, std::vector<uint8_t>> deviceStatus;
};

#endif // RECEIVEDATAHANDLER_H
