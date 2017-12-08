#include "receivedataproc.h"
#include "../thirdparty/shareddata.h"

#include <boost/make_shared.hpp>
#include <stdlib.h>
#include <QDebug>


typedef enum Range_Type_E
{
    SINGLE = 0,//单个离散的数
    RANGE_1,//最小和最大值
    RANGE_2//最小和最大值和step，类似等差数列
}Range_Type;

typedef struct Range_Data_S
{
    Range_Type type;
    int length;
    int * data;
}Range_Data;
static bool RangePayloadParer(uint8_t * srcData, int srcLength, Range_Data ** destData, int * destLength)
{
    if (NULL == srcData || srcLength <= 0 || NULL == destData) {
        return false;
    }

    uint8_t *ptr = srcData;
    int payloadBytes = srcData[0];//每个元组中数据字节数
    int elemNum = srcData[1];//元组数量
    ptr += 2;
    int remainBytes = srcLength - 2;//剩余字节数，用于校验数据是否有错误

    Range_Data * rangeData = (Range_Data *)malloc(sizeof(Range_Data) * elemNum);
    memset(rangeData, 0, sizeof(Range_Data) * elemNum);
    int rangeDataNum = 0;
    for (; rangeDataNum < elemNum; ++rangeDataNum) {
        Range_Type rangType = (Range_Type)ptr[0];
        ++ptr;
        --remainBytes;

        if (remainBytes <= 0) goto error;

        if (SINGLE == rangType) {
            rangeData[rangeDataNum].type = SINGLE;
            rangeData[rangeDataNum].length = 1;//单个值，取一个payload长度

            rangeData[rangeDataNum].data = (int*)malloc(sizeof(int));//目前数据值都是int型变量
            memset(rangeData[rangeDataNum].data, 0, sizeof(int));

            memcpy(rangeData[rangeDataNum].data, ptr, payloadBytes);
            ptr += payloadBytes;

            remainBytes -= payloadBytes;
        }
        else if (RANGE_1 == rangType) {
            rangeData[rangeDataNum].type = RANGE_1;
            rangeData[rangeDataNum].length = 2;//取最大最小值，两个长度

            rangeData[rangeDataNum].data = (int*)malloc(sizeof(int) * 2);
            memset(rangeData[rangeDataNum].data, 0, sizeof(int) * 2);

            memcpy(rangeData[rangeDataNum].data + 1, ptr, payloadBytes);
            ptr += payloadBytes;
            memcpy(rangeData[rangeDataNum].data, ptr, payloadBytes);
            ptr += payloadBytes;

            remainBytes -= payloadBytes * 2;
        }
        else if (RANGE_2 == rangType) {
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
            rangeData[rangeDataNum].type = SINGLE;
            int length = 0;
            bool dataInvalid = true;
            if (step <= 0 || step >= (max - min)) {
                length = 2;//step值不合理，数列只有最小和最大值，长度为2
            }
            else if (0 == max - min) {
                length = 1;//max和min相等，数列只有一个值，长度为1
            }
            else if (0 > max - min) {
                length = 0;//max比min小，数列0个值
            }
            else {
                //数据正确，数列长度为：如果（max-min)/step能整除则长度为(max-min)/step+1，否则为(max-min)/step+2
                length = ((max - min) % step) ? ((max - min) / step + 2) : ((max - min) / step + 1);
                dataInvalid = false;
            }

            rangeData[rangeDataNum].length = length;
            rangeData[rangeDataNum].data = (int*)malloc(length);
            memset(rangeData[rangeDataNum].data, 0, length);

            if (dataInvalid) {
                if (0 == length) {
                    continue;
                }
                else if (1 == length) {
                    memcpy(rangeData[rangeDataNum].data, &max, payloadBytes);
                }
                else if (2 == length) {
                    memcpy(rangeData[rangeDataNum].data, &min, payloadBytes);
                    memcpy(rangeData[rangeDataNum].data + 1, &max, payloadBytes);
                }
            }
            else {
                for (int i = 0; i < rangeData[rangeDataNum].length; ++i) {
                    if (min + i * step < max) {
                        rangeData[rangeDataNum].data[i] = min + i * step;//每次将数据放入数列中
                    }
                    else {
                        rangeData[rangeDataNum].data[i] = max;//放入最后一个数max
                    }
                }
            }
        }
        else {
            goto error;
        }
    }

    *destData = rangeData;
    *destLength = elemNum;
    return true;

error:
    for (int i = 0; i < rangeDataNum; ++i) {
        if (rangeData->data) free(rangeData->data);
    }
    free(rangeData);
    return false;
}


ReceiveDataProc::ReceiveDataProc(QObject *parent) :
    QObject(parent),
    dataHandler(nullptr)
{
}

void ReceiveDataProc::start()
{
    if (nullptr == dataHandler) {
        qDebug() << "No data handler registered!!!!";
        return;
    }

    thread_ = boost::make_shared<boost::thread>(&ReceiveDataProc::run, this);
}

void ReceiveDataProc::run()
{
    while (true) {
//        ReceivedDataType d;
//        d.push_back(100);
//        dataHandler(d);
        ProtocolStruct protoData;
        //popData maybe blocked
        SharedData::Get()->popData(protoData);
        protocolStructProc(protoData);
    }
}

void ReceiveDataProc::protocolStructProc(const ProtocolStruct &ps)
{
//    if (protoData.data.size() == 0) {
//        continue;
//    }

    //get lower 8 bit and 4 bit cmdSet
    uint32_t cmdSet = static_cast<uint32_t>(ps.cmdSet & 0xffu);
    //get lower 16 bit and 12 bit cmdId
    uint32_t cmdId = static_cast<uint32_t>(ps.cmdID & 0xffffu);
    //get high 8 bit cmdSet and low 24 bit cmdId
    uint32_t key = (cmdSet << 24) | cmdId;

    if (dataHandler) {
        dataHandler(ps.data);
    }
}
