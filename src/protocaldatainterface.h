#ifndef PROTOCALDATAINTERFACE_H
#define PROTOCALDATAINTERFACE_H
#include "cmddef.h"
#include "../thirdparty/Protocol.hpp"
#include "receivedatahandler.h"

#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <stdint.h>
#include <vector>

//enum ProtocalDataType
//{
//    ProtocalData_Normal = 0,
//    ProtocalData_SurportList
//};

class ReceiveDataHandler;

class ProtocalDataInterface : public boost::enable_shared_from_this<ProtocalDataInterface>
{
public:
    ProtocalDataInterface(Remo_CmdSet_e set = Remo_CmdSet_Camera);

    void setData(const ProtocolStruct & data) {this->data = data;}
    const ProtocolStruct & getData() {return data;}

    virtual void handle(){}

    void registerSelf2Handler() const {
//        ReceiveDataHandler::getInstance()->registerDataHandler(*shared_from_this());
    }

    Remo_CmdSet_e getCmdSet(){ return cmdSet;}

protected:
    ProtocolStruct data;

private:
    const Remo_CmdSet_e cmdSet;
};

#endif // PROTOCALDATAINTERFACE_H
