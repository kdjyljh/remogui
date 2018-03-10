#include "receivedatahandler.h"
#include "receivedataproc.h"
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <cstring>

boost::mutex mtx_parsedDataQueue;
boost::condition_variable cv_parsedDataQueue;

boost::unordered_map<uint32_t, std::vector<uint8_t>> ReceiveDataHandler::deviceStatus;
std::deque<CmdContent> ReceiveDataHandler::parsedDataQueue{};
ReceiveDataHandler::ReceiveDataHandler()
{
}

void ReceiveDataHandler::pushData(CmdContent &parsedData)
{
    boost::unique_lock<boost::mutex> lock(mtx_parsedDataQueue);
    parsedDataQueue.push_back(parsedData);
    cv_parsedDataQueue.notify_one();
}

bool ReceiveDataHandler::rangePayloadParer(uint8_t *srcData, int srcLength, Range_Data **destData, int *destLength)
{
    if (NULL == srcData || srcLength <= 0 || NULL == destData) {
        return false;
    }

    uint8_t *ptr = srcData;
    int payloadBytes = srcData[0];//每个元组中数据字节数
    int elemNum = srcData[1];//元组数量
    ptr += 2;
    int remainBytes = srcLength - 2;//剩余字节数，用于校验数据是否有错误

    Range_Data *rangeData = (Range_Data *) malloc(sizeof(Range_Data) * elemNum);
    memset(rangeData, 0, sizeof(Range_Data) * elemNum);
    int rangeDataNum = 0;
    for (; rangeDataNum < elemNum; ++rangeDataNum) {
        Range_Type rangType = (Range_Type) ptr[0];
        ++ptr;
        --remainBytes;

        if (remainBytes <= 0) goto error; //剩余数据小于0,出错

        if (SINGLE == rangType) {
            rangeData[rangeDataNum].type = SINGLE;
            rangeData[rangeDataNum].length = 1;//单个值，取一个payload长度

            rangeData[rangeDataNum].data = (int *) malloc(sizeof(int));//目前数据值都是int型变量
            memset(rangeData[rangeDataNum].data, 0, sizeof(int));

            memcpy(rangeData[rangeDataNum].data, ptr, payloadBytes);
            ptr += payloadBytes;

            remainBytes -= payloadBytes;
        } else if (RANGE_1 == rangType) {
            int max = 0;
            int min = 0;
            //取最大最小两个个值
            memcpy(&max, ptr, payloadBytes);
            ptr += payloadBytes;
            memcpy(&min, ptr, payloadBytes);
            ptr += payloadBytes;

            remainBytes -= payloadBytes * 2;

            if (max > min) {
                rangeData[rangeDataNum].type = RANGE_1;
                rangeData[rangeDataNum].length = 2;//取最大最小值，两个长度
                rangeData[rangeDataNum].data = (int *) malloc(2);
                memset(rangeData[rangeDataNum].data, 0, 2);
                memcpy(rangeData[rangeDataNum].data, &min, payloadBytes);
                memcpy(rangeData[rangeDataNum].data + 1, &max, payloadBytes);
            } else if (max == min) {
                rangeData[rangeDataNum].type = RANGE_1;
                rangeData[rangeDataNum].length = 1;//最大值与最小值相等，取一个长度
                rangeData[rangeDataNum].data = (int *) malloc(1);
                memset(rangeData[rangeDataNum].data, 0, 1);
                memcpy(rangeData[rangeDataNum].data, &max, payloadBytes);
            } else {
                //数据出错，丢弃当前值，不做任何操作
            }
        } else if (RANGE_2 == rangType) {
            int min = 0;
            int max = 0;
            int step = 0;
            //取最大最小和step三个值
            memcpy(&max, ptr, payloadBytes);
            ptr += payloadBytes;
            memcpy(&step, ptr, payloadBytes);
            ptr += payloadBytes;
            memcpy(&min, ptr, payloadBytes);
            ptr += payloadBytes;

            remainBytes -= 3 * payloadBytes;

            //将最大最小和step转化成等差数列，类型设置为SINGLE
            rangeData[rangeDataNum].type = RANGE_2;
            int length = 0;
            bool dataInvalid = true;
            if (0 < max - min) {
                if (step <= 0 || step >= (max - min)) {
                    length = 2;//step值不合理，数列只有最小和最大值，长度为2
                } else {
                    //数据正确，数列长度为：如果（max-min)/step能整除则长度为(max-min)/step+1，否则为(max-min)/step+2
                    length = ((max - min) % step) ? ((max - min) / step + 2) : ((max - min) / step + 1);
                    dataInvalid = false;
                }
            } else if (0 > max - min) {
                length = 0;//max比min小，数列0个值
            } else {
                length = 1;//max和min相等，数列只有一个值，长度为1
            }

            rangeData[rangeDataNum].length = length;
            rangeData[rangeDataNum].data = (int *) malloc(length * sizeof(int));
            memset(rangeData[rangeDataNum].data, 0, length);

            if (dataInvalid) {
                if (0 == length) {
                    continue;
                } else if (1 == length) {
                    memcpy(rangeData[rangeDataNum].data, &max, payloadBytes);
                } else if (2 == length) {
                    memcpy(rangeData[rangeDataNum].data, &min, payloadBytes);
                    memcpy(rangeData[rangeDataNum].data + 1, &max, payloadBytes);
                }
            } else {
                for (int i = 0; i < rangeData[rangeDataNum].length; ++i) {
                    if (min + i * step < max) {
                        rangeData[rangeDataNum].data[i] = min + i * step;//每次将数据放入数列中
                    } else {
                        rangeData[rangeDataNum].data[i] = max;//放入最后一个数max
                    }
                }
            }
        } else {
            goto error;
        }
    }

    if (remainBytes > 0) goto error;//解析完成，数据还有剩余，出错

    *destData = rangeData;
    *destLength = elemNum;
    return true;

    error:
    //如果出错很可能会在这里挂掉，最好注释，但是注释会造成内存泄露
    for (int i = 0; i < rangeDataNum; ++i) {
        if (rangeData->data) free(rangeData->data);
    }
    free(rangeData);
    return false;
}

bool ReceiveDataHandler::mergeRange(Range_Data *srcRange, int srcLength, std::set<SubItemData> & destRange)
{
    if (nullptr == srcRange || 0 >= srcLength) {
        return false;
    }

    destRange.clear();
    for (auto it : itemData) {
        if (it.CmdId_GetRange == data.cmdID || it.CmdId_GetData == data.cmdID || it.CmdId_SetData == data.cmdID) {
            std::set<SubItemData> subData = it.subItemData;
            for (auto subIt : subData) {
                for (int i = 0; i < srcLength; ++i) {
                    if (RANGE_2 == srcRange[i].type) {
                        std::vector<int> v(srcRange[i].data, srcRange[i].data + srcRange[i].length);
                        for (int j = 0; j < srcRange[i].length; ++j) {
                            std::string showStr = boost::lexical_cast<std::string>(srcRange[i].data[j]);
                            destRange.insert({srcRange[i].data[j], showStr.data()});
                        }
                        return true;
                    }
                    else if (RANGE_1 == srcRange[i].type) {
                        if (srcRange[i].data[0] <= subIt.Index && subIt.Index <= srcRange[i].data[1]) {
                            destRange.insert(subIt);
                            break;
                        }
                    }
                    else if (SINGLE == srcRange[i].type) {
                        if (srcRange[i].data[0] == subIt.Index) {
                            destRange.insert(subIt);
                            break;
                        }
                    }
                }
            }
            break;
        }
    }
    return true;
}

bool ReceiveDataHandler::getSurportRange(std::set<SubItemData> & range)
{
    Range_Data *srcRange;
    int length;
    if (rangePayloadParer(data.data.data(), data.data.size(), &srcRange, &length)) {
        if (mergeRange(srcRange, length, range)) {
            return true;
        }
    }
    return false;
}

bool ReceiveDataHandler::dataParser(CmdContent &parsedData)
{
    bool ret = true;
    Remo_CmdId_Camera_e cmdId = static_cast<Remo_CmdId_Camera_e>(data.cmdID);
    Remo_CmdId_Type_e idType = static_cast<Remo_CmdId_Type_e>(cmdId >> 9);
//    memset(&parsedData, 0, sizeof(parsedData));
    parsedData.cmdId = data.cmdID;
    parsedData.cmdSet = data.cmdSet;

    if (data.cmdSet == Remo_CmdSet_Camera) {
        if (CmdId_Type_GetRange == idType) {
            std::set<SubItemData> range;
            if (getSurportRange(range)) {
                parsedData.range = range;
            }
            else {
                ret = false;
            }
        }
        else {
            parsedData.custom = data.data;
        }
    }
    else {
        parsedData.custom = data.data;
    }

    return ret;
}

boost::shared_ptr<ReceiveDataHandler> ReceiveDataHandler::getInstance()
{
    //multi thread not safe
    static boost::shared_ptr<ReceiveDataHandler> instance(new ReceiveDataHandler);
    ReceiveDataProc::getInstance()->registerHandler(instance);
    return instance;
}

void ReceiveDataHandler::popData(CmdContent &data)
{
    boost::unique_lock<boost::mutex> lock(mtx_parsedDataQueue);
    while (parsedDataQueue.empty()) {
        cv_parsedDataQueue.wait(lock);
    }
    data = parsedDataQueue.front();
    parsedDataQueue.pop_front();
}

void ReceiveDataHandler::handle()
{
    if (data.data.size() < 1) return;

    Remo_CmdId_Type_e idType = static_cast<Remo_CmdId_Type_e>(data.cmdID >> 9);
    uint16_t idValue = data.cmdID & 0x1ff;

    //解析第一个字节:返回值
    Remo_CmdId_SetCmd_ReturnValue_e ret = static_cast<Remo_CmdId_SetCmd_ReturnValue_e>(data.data.at(0));
    if (CmdId_Type_Get == idType || (CmdId_Type_Set == idType && ret == Return_OK)) {
        uint32_t key = data.cmdSet;
        key = key << 24 | idValue;
        deviceStatus[key] = data.data;
    }

    CmdContent cc;
    cc.ret = ret;
    cc.isRestore = false;
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
                cc.isRestore = true;
            }
            else {
                LOG(INFO) << "restore device status failed, can not find device key !!!!!!!!!!!!!!!!!!!!";
            }
        }
        else {
        }

//        pushData(cc);
//        return;
    }

    data.data.assign(data.data.begin() + 1, data.data.end());

    if(dataParser(cc)) {
        pushData(cc);
    }
}
