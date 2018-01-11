#ifndef RECEIVEDATAHANDLER_H
#define RECEIVEDATAHANDLER_H
#include "../thirdparty/Protocol.hpp"
#include "cmddef.h"
#include "protocaldatainterface.h"
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/unordered_map.hpp>
#include <deque>

class ProtocolDataInterface;

struct ParsedData
{
    std::set<SubItemData> range;
    std::vector<uint8_t> custom;
    int cmdSet;
    int cmdId;
};

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

    bool popData(ParsedData & data);

    virtual void handle();

    void setData(const ProtocolStruct & data) {this->data = data;}
    const ProtocolStruct getData() {return data;}

private:
    ReceiveDataHandler();
    bool dataParser(ParsedData &parsedData);
    void pushData(ParsedData &parsedData);

private:
    //must be ptr, using virtual funciton handle
    std::vector<boost::shared_ptr<ProtocolDataInterface>> handlerPtrList;
    ProtocolStruct data;
    static boost::unordered_map<uint32_t, std::vector<uint8_t>> deviceStatus;
    static std::deque<ParsedData> parsedDataQueue;
};

#endif // RECEIVEDATAHANDLER_H
