#ifndef CAMERAWORKMODE_H
#define CAMERAWORKMODE_H
#include "protocaldatainterface.h"
#include "camerafielddef.h"

class ProtocolDataInterfaceImpl : public ProtocolDataInterface
{
public:
    ProtocolDataInterfaceImpl(DispatcheType type = DispatcheType_WorkMode);

    void async_setWorkMode(const Remo_Camera_WorkMode_s & workmode);

    virtual void handle() override;

protected:
    virtual void workModeGot(const Remo_Camera_WorkMode_s & workmode) {}
//    virtual void capDelayTimeGot(const int & value){}
//    virtual void capDelayTimeListGot(const Range_Data & range){}
    virtual void surportRangeGot(std::set<SubItemData> rangeSet, Remo_CmdId_Camera_e cmdId){}
    virtual void settingGot(const std::vector<uint8_t> & data, Remo_CmdId_Camera_e cmdId) {}
    virtual void controlGot() {}

private:
};

#endif // CAMERAWORKMODE_H
