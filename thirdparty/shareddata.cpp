#include "shareddata.h"

//boost::mutex SharedData::mtx = boost::mutex();
//boost::unique_lock<boost::mutex> SharedData::lk = boost::unique_lock<boost::mutex>(SharedData::mtx);
//boost::condition_variable SharedData::cv = boost::condition_variable();
static boost::mutex mtx;
//static boost::unique_lock<boost::mutex> lk(mtx);
static boost::condition_variable cv;

std::deque<ProtocolStruct> SharedData::queue = std::deque<ProtocolStruct>();
SharedData::SharedData()
{

}

boost::shared_ptr<SharedData> SharedData::Get()
{
    static boost::shared_ptr<SharedData> instance(new SharedData());
    return instance;
}

void SharedData::pushData(const ProtocolStruct & data)
{
    {
        boost::unique_lock<boost::mutex> lock(mtx);
        queue.push_back(data);
        cv.notify_all();
    }
}

void SharedData::popData(ProtocolStruct &receivedData)
{
    {
        boost::unique_lock<boost::mutex> lock(mtx);
        while (queue.empty())
            cv.wait(lock);
        receivedData = queue.front();
        queue.pop_front();
    }
}


