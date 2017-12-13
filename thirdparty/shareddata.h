#ifndef SHAREDDATA_H
#define SHAREDDATA_H

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <deque>
#include "Protocol.hpp"

class SharedData : public boost::noncopyable
{
public:
    static boost::shared_ptr<SharedData> Get();

    void pushData(const ProtocolStruct & data);
    void popData(ProtocolStruct &receivedData);

private:
    static std::deque<ProtocolStruct> queue;
private:
    SharedData();
};

#endif // SHAREDDATA_H
