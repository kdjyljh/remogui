#include "shareddata.h"

//boost::mutex SharedData::mtx = boost::mutex();
//boost::unique_lock<boost::mutex> SharedData::lk = boost::unique_lock<boost::mutex>(SharedData::mtx);
//boost::condition_variable SharedData::cv = boost::condition_variable();
static boost::mutex mtxReceive;
static boost::condition_variable cvReceive;

static boost::mutex mtxSend;

std::deque<ProtocolStruct> SharedData::receiveQueue = std::deque<ProtocolStruct>();
std::deque<ProtocolStruct> SharedData::sendQueue;
SharedData::SharedData()
{

}

boost::shared_ptr<SharedData> SharedData::Get()
{
    static boost::shared_ptr<SharedData> instance(new SharedData());
    return instance;
}

void SharedData::pushReceiveData(const ProtocolStruct & data)
{
    {
        boost::unique_lock<boost::mutex> lock(mtxReceive);
        receiveQueue.push_back(data);
        cvReceive.notify_all();
    }

    LOG(INFO) << "SharedData::pushReceiveData success seqId = " << std::hex << data.packSeq << " cmdId = " << data.cmdID << " cmdSet = " << data.cmdSet;
}

bool SharedData::popReceiveData(ProtocolStruct &data)
{
    {
        boost::unique_lock<boost::mutex> lock(mtxReceive);
        while (receiveQueue.empty())
            cvReceive.wait(lock);
        data = receiveQueue.front();
        receiveQueue.pop_front();
    }

    //如果是回应包，根据包序号找到对应的请求包，并添加请求包数据字段到响应包
    if (data.packFlags.bits.ReqResp = CommProtoVariables::RESPOND) {
        ProtocolStruct sendData;
        if (popSendDataBySeqId(sendData, data.packSeq) /*&& !sendData.data.empty()*/) {
            data.data.insert(data.data.end(), sendData.data.begin(), sendData.data.end());
        }
        else {
            LOG(INFO) << "Can not find resqest packeg for response seqId = " << std::hex << data.packSeq << " cmdId = " << data.cmdID << " cmdSet = " << data.cmdSet;
            return false; //没有找到对应的请求包，返回false，提示应该将包丢弃
        }
    }
    return true;
}

void SharedData::pushSendData(const ProtocolStruct &data)
{
//    std::cout << FLAGS_logtostderr << std::endl;

    {
        boost::unique_lock<boost::mutex> lock(mtxSend);
        LOG(INFO) << "SharedData::pushSendData seqId = " << std::hex << data.packSeq  << " cmdId = " << data.cmdID << " cmdSet = " << data.cmdSet;
        sendQueue.push_back(data);
    }
}

bool SharedData::popSendDataBySeqId(ProtocolStruct &data, uint16_t seqId)
{
    {
        boost::unique_lock<boost::mutex> lock(mtxSend);
        bool foundData = false;
        for (auto it = sendQueue.begin(); it != sendQueue.end();) {
            if (seqId == it->packSeq) {
                data = *it;
                it = sendQueue.erase(it);
                LOG(INFO) << "SharedData::popSendDataBySeqId find resqest packeg for seqId = " << std::hex << seqId << " cmdId = " << data.cmdID << " cmdSet = " << data.cmdSet;
                foundData = true;
            }
            else {
                ++it;
            }
        }
        return foundData;
    }
}

bool SharedData::popSendDataByTimedTaskId(TimedTaskID timedTaskID, ProtocolStruct &data)
{
    {
        boost::unique_lock<boost::mutex> lock(mtxSend);
        bool foundData = false;
        for (auto it = sendQueue.begin(); it != sendQueue.end();) {
            if (timedTaskID == it->idForward()) {
                data = *it;
                it = sendQueue.erase(it);
                LOG(INFO) << "SharedData::popSendDataByTimedTaskId find resqest packeg for TimedTaskId = " << std::hex << timedTaskID << " cmdId = " << data.cmdID << " cmdSet = " << data.cmdSet;
                foundData = true;
            }
            else {
                ++it;
            }
        }
        return foundData;
    }
}


