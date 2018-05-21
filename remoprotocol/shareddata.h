#ifndef SHAREDDATA_H
#define SHAREDDATA_H

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <deque>
#include "Protocol.hpp"
#include "TimedTask.hpp"

class SharedData : public boost::noncopyable
{
public:
    static boost::shared_ptr<SharedData> Get();

    void pushReceiveData(const ProtocolStruct & data);
    bool popReceiveData(ProtocolStruct &receivedData);

    void pushSendData(const ProtocolStruct & data);
    bool popSendDataBySeqId(ProtocolStruct & data, uint16_t seqId);
    bool popSendDataByTimedTaskId(TimedTaskID timedTaskID, ProtocolStruct & data);
    bool popSendDataByTimedTaskId(TimedTaskID timedTaskID) {
        ProtocolStruct data;
        return popSendDataByTimedTaskId(timedTaskID, data);
    }

private:
    static std::deque<ProtocolStruct> receiveQueue;
    static std::deque<ProtocolStruct> sendQueue;
private:
    SharedData();
};

#endif // SHAREDDATA_H
