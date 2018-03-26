#ifndef PROTOCALDATAINTERFACE_H
#define PROTOCALDATAINTERFACE_H
#include "cmddef.h"
#include "../thirdparty/Protocol.hpp"
#include "itemdatadef.h"

#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <stdint.h>
#include <vector>

class ReceiveDataHandler;

class ProtocolDataInterface : public boost::enable_shared_from_this<ProtocolDataInterface>
{
public:
    ProtocolDataInterface(DispatcheType type = DispatcheType_WorkMode);

    void setContent(CmdContent content) {this->content = content;}

    virtual void handle() = 0;

    void registerSelf2Handler();
    DispatcheType getType() {return dispatcheType;}

    static void sendCmd(CommDeviceEnum device, Remo_CmdSet_e cmdSet, int cmdId, bool needAckApp, bool needAckProto,
                     CommProtoVariables::RequestRespond reqres, std::vector<uint8_t> data, int maxRetry = -1, long intervalUS = -1);

    static bool isBigEndian();
    static bool getConfiguredEndpoint(uint32_t &ip, uint16_t &port);
    static void endianTurn(std::vector<uint8_t> & data);

protected:
    CmdContent content;
    static int reqMaxRetry;
    static long reqIntervalUS;

    virtual void retProcess(CmdContent cc){}

    bool rangePayloadParer(uint8_t *srcData, int srcLength, Range_Data **destData, int *destLength);
    bool mergeRange(Range_Data *srcRange, int srcLength, std::set<SubItemData> & destRange);
    bool getSurportRange(std::set<SubItemData> & range);

private:
    DispatcheType dispatcheType;
};
#endif // PROTOCALDATAINTERFACE_H
