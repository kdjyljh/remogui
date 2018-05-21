#ifndef RECEIVEDATAHANDLER_H
#define RECEIVEDATAHANDLER_H
#include "Protocol.hpp"
#include "cmddef.h"
#include "itemdatadef.h"
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/unordered_map.hpp>
#include <deque>

class ProtocolDataInterface;



class ReceiveDataHandler : public boost::noncopyable
{
public:
    static boost::shared_ptr<ReceiveDataHandler> getInstance();

    void popData(CmdContent & data);

    virtual void handle();

    void setData(const ProtocolStruct & data) {this->data = data;}
    const ProtocolStruct getData() {return data;}

private:
    ReceiveDataHandler();
    bool dataParser(CmdContent &parsedData);
    void pushData(CmdContent &parsedData);
    bool mergeRange(Range_Data *srcRange, int srcLength, std::set<SubItemData> & destRange);
    bool rangePayloadParer(uint8_t *srcData, int srcLength, Range_Data **destData, int *destLength);
    bool getSurportRange(std::set<SubItemData> & range);

private:
    ProtocolStruct data;
    static boost::unordered_map<uint32_t, std::vector<uint8_t>> deviceStatus;
    static std::deque<CmdContent> parsedDataQueue;
};

#endif // RECEIVEDATAHANDLER_H
