#include "receivedatahandler.h"
#include "receivedataproc.h"
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>

boost::mutex mtx_parsedDataQueue;
boost::condition_variable cv_parsedDataQueue;

boost::unordered_map<uint32_t, std::vector<uint8_t>> ReceiveDataHandler::deviceStatus;
std::deque<ParsedData> ReceiveDataHandler::parsedDataQueue();
ReceiveDataHandler::ReceiveDataHandler()
{
}

void ReceiveDataHandler::pushData(ParsedData &parsedData)
{
    boost::unique_lock<boost::mutex> lock;
    parsedDataQueue.push_back(parsedData);
    cv_parsedDataQueue.notify_one();
}

bool ReceiveDataHandler::dataParser(DeviceParsedData &parsedData)
{

}

boost::shared_ptr<ReceiveDataHandler> ReceiveDataHandler::getInstance()
{
    //multi thread not safe
    static boost::shared_ptr<ReceiveDataHandler> instance(new ReceiveDataHandler);
    ReceiveDataProc::getInstance()->registerHandler(instance);
    return instance;
}

bool ReceiveDataHandler::popData(ParsedData &data)
{
    boost::unique_lock<boost::mutex> lock;
    while (parsedDataQueue.empty()) {
        cv_parsedDataQueue.wait(mtx_parsedDataQueue);
    }
    data = parsedDataQueue.front();
    parsedDataQueue.pop_front();
}

void ReceiveDataHandler::handle()
{
    if (data.data.empty()) return;

    Remo_CmdId_Type_e idType = static_cast<Remo_CmdId_Type_e>(data.cmdID >> 9);
    uint16_t idValue = data.cmdID & 0x1ff;

    //解析第一个字节:返回值
    Remo_CmdId_SetCmd_ReturnValue_e ret = static_cast<Remo_CmdId_SetCmd_ReturnValue_e>(data.data.at(0));
    if (CmdId_Type_Get == idType || (CmdId_Type_Set == idType && ret == Return_OK)) {
        uint32_t key = data.cmdSet;
        key = key << 24 | idValue;
        deviceStatus[key] = data.data;
    }

    if (ret != Return_OK) {
        if (CmdId_Type_Set == idType) {
            LOG(INFO) << "set device failed, restore device status!!!!!!!!!!!!!!!!!!!!";
            //设置设备失败，修改设备cmdid重新设置界面
            data.cmdID = CmdId_Type_Get << 9 | idValue;
            uint32_t key = data.cmdSet;
            key = key << 24 | idValue;

            auto it = deviceStatus.find(key);
            if (it != deviceStatus.end()) {
                    data.data = it->second;
            }
            else {
                LOG(INFO) << "restore device status failed, can not find device key !!!!!!!!!!!!!!!!!!!!";
            }
        }
        else {
            return;
        }
    }

    data.data.assign(data.data.begin() + 1, data.data.end());

    ParsedData data;
    if(dataParser(data)) {

    }

//    Remo_CmdSet_e set = static_cast<Remo_CmdSet_e>(data.cmdSet);
//    for (auto it : handlerPtrList) {
//        if (it->getCmdSet() == set) {
//            it->setData(data);
//            it->handle();
////            break;
//        }
//    }
}
