#include "receivedatahandler.h"
#include "receivedataproc.h"

boost::unordered_map<uint32_t, std::vector<uint8_t>> ReceiveDataHandler::deviceStatus;
ReceiveDataHandler::ReceiveDataHandler()
{
}

boost::shared_ptr<ReceiveDataHandler> ReceiveDataHandler::getInstance()
{
    //multi thread not safe
    static boost::shared_ptr<ReceiveDataHandler> instance(new ReceiveDataHandler);
    ReceiveDataProc::getInstance()->registerHandler(instance);
    return instance;
}

void ReceiveDataHandler::handle()
{
    Remo_CmdId_Type_e idType = static_cast<Remo_CmdId_Type_e>(data.cmdID >> 9);
    uint16_t idValue = data.cmdID & 0x1ff;
    if (CmdId_Type_Get == idType || CmdId_Type_Set == idType) {
        uint32_t key = data.cmdSet;
        key = key << 24 | idValue;
        deviceStatus[key] = data.data;
    }

    //解析第一个字节:返回值
    Remo_CmdId_SetCmd_ReturnValue_e ret = static_cast<Remo_CmdId_SetCmd_ReturnValue_e>(data.data.at(0));
    data.data.assign(data.data.begin() + 1, data.data.end());

    if (ret != Return_OK && CmdId_Type_Set == idType) {
        LOG(INFO) << "set device failed, restore device status!!!!!!!!!!!!!!!!!!!!";
        //设置设备失败，修改设备cmdid重新设置界面
        data.cmdID = CmdId_Type_Get << 9 | idValue & 0x1ff;
    }

    Remo_CmdSet_e set = static_cast<Remo_CmdSet_e>(data.cmdSet);
    for (auto it : handlerPtrList) {
        if (it->getCmdSet() == set) {
            it->setData(data);
            it->handle();
            break;
        }
    }
}
