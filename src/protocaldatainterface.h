#ifndef PROTOCALDATAINTERFACE_H
#define PROTOCALDATAINTERFACE_H

#include "cmddef.h"
#include "Protocol.hpp"
#include "itemdatadef.h"

#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <stdint.h>
#include <vector>

class ReceiveDataHandler;

class ProtocolDataInterface : public boost::enable_shared_from_this<ProtocolDataInterface> {
public:
    ProtocolDataInterface(DispatcheType type = DispatcheType_WorkMode);

    void setContent(CmdContent content) { this->content = content; }

    virtual void handle() = 0;

    void registerSelf2Handler();

    DispatcheType getType() { return dispatcheType; }

    static void sendCmd(CommDeviceEnum device, Remo_CmdSet_e cmdSet, int cmdId, bool needAckApp, bool needAckProto,
                        CommProtoVariables::RequestRespond reqres, std::vector<uint8_t> data, int maxRetry = -1,
                        long intervalUS = -1);

    static void sendCmdCamera(Remo_CmdId_Camera_e cmdId,
                              std::vector<uint8_t> data = std::vector<uint8_t>(),
                              CommProtoVariables::RequestRespond reqres = CommProtoVariables::REQUEST);

    static void sendCmdCamera(Remo_CmdId_Camera_e cmdId, int maxRety, long intervalUS,
                              std::vector<uint8_t> data = std::vector<uint8_t>(),
                              CommProtoVariables::RequestRespond reqres = CommProtoVariables::REQUEST);

    static void sendCmdGimbal(Remo_CmdId_Gimbal_e cmdId,
                              std::vector<uint8_t> data = std::vector<uint8_t>(),
                              CommProtoVariables::RequestRespond reqres = CommProtoVariables::REQUEST);

    static void sendCmdBattery(Remo_CmdId_Battery_e cmdId,
                               std::vector<uint8_t> data = std::vector<uint8_t>(),
                               CommProtoVariables::RequestRespond reqres = CommProtoVariables::REQUEST);

    static void sendCmdUniversal(int cmdId,
                                 std::vector<uint8_t> data = std::vector<uint8_t>(),
                                 CommProtoVariables::RequestRespond reqres = CommProtoVariables::REQUEST);

    void sendCmdCamera() {}

    static bool syncSendCmd(CommDeviceEnum device, Remo_CmdSet_e cmdSet, int cmdId, bool needAckApp, bool needAckProto,
                            CommProtoVariables::RequestRespond reqres, std::vector<uint8_t> data, int timeout = 10000);

    static bool syncSendCamera(Remo_CmdId_Camera_e cmdId, std::vector<uint8_t> data = std::vector<uint8_t>(),
                               int timeout = 10000, CommProtoVariables::RequestRespond reqres = CommProtoVariables::REQUEST);

    static bool syncSendUniversal(Remo_CmdId_Camera_e cmdId, vector<uint8_t> data = std::vector<uint8_t>(),
                                  int timeout = 1000,
                                  CommProtoVariables::RequestRespond reqres = CommProtoVariables::REQUEST);

    static bool isBigEndian();

    static void endianTurn(std::vector<uint8_t> &data);

    bool transmitLocaleIp();
    bool isValid() { return valid; }

protected:
    CmdContent content;
    static int reqMaxRetry;
    static long reqIntervalUS;

    virtual void retProcess(CmdContent cc) {}

    bool rangePayloadParer(uint8_t *srcData, int srcLength, Range_Data **destData, int *destLength);

    bool mergeRange(Range_Data *srcRange, int srcLength, std::set<SubItemData> &destRange);

    bool getSurportRange(std::set<SubItemData> &range);

private:
    DispatcheType dispatcheType;
    bool valid;
};

#endif // PROTOCALDATAINTERFACE_H
