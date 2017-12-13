#ifndef PROTOCALDATAINTERFACE_H
#define PROTOCALDATAINTERFACE_H
#include "cmddef.h"
#include "../thirdparty/Protocol.hpp"
#include "receivedatahandler.h"
#include "itemdatadef.h"

#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <stdint.h>
#include <vector>

typedef enum Range_Type_E
{
    SINGLE = 0,//单个离散的数
    RANGE_1,//最小和最大值
    RANGE_2//最小和最大值和step，类似等差数列
}Range_Type;

typedef struct Range_Data_S
{
    Range_Type type;
    int length;
    int * data;
}Range_Data;



class ReceiveDataHandler;

class ProtocolDataInterface : public boost::enable_shared_from_this<ProtocolDataInterface>
{
public:
    ProtocolDataInterface(Remo_CmdSet_e set = Remo_CmdSet_Camera);

    void setData(const ProtocolStruct & data) {this->data = data;}
    const ProtocolStruct & getData() {return data;}

    virtual void handle() = 0;

    void registerSelf2Handler();

    Remo_CmdSet_e getCmdSet(){ return cmdSet;}

    void sendCmd(CommDeviceEnum device, Remo_CmdSet_e cmdSet, Remo_CmdId_e cmdId, bool needAckApp, bool needAckProto,
                     CommProtoVariables::RequestRespond reqres, std::vector<uint8_t> data);

protected:
    ProtocolStruct data;
    int reqMmaxRetry = 5;
    long reqIntervalUS = 50000;

    bool rangePayloadParer(uint8_t *srcData, int srcLength, Range_Data **destData, int *destLength);
    bool mergeRange(Range_Data *srcRange, int srcLength, std::set<SubItemData> & destRange);
    bool getSurportRange(std::set<SubItemData> & range);

private:
    const Remo_CmdSet_e cmdSet;
};
#endif // PROTOCALDATAINTERFACE_H
