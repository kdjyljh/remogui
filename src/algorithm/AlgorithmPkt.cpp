//
// Created by jianghualuo on 18-4-8.
//

#include "AlgorithmPkt.h"

#include <boost/lexical_cast.hpp>
#include <boost/crc.hpp>
#include <glog/logging.h>

AlgorithmPkt::AlgorithmPkt() :
        bodyLength_(0) {

}

bool AlgorithmPkt::decodeHeader() {
    //大小端处理
    int length = buffHead_[0];
    length = buffHead_[0];
    length = length << 8 | buffHead_[1];
    length = length << 8 | buffHead_[2];
    length = length << 8 | buffHead_[3];

    if (length > MAX_PACKET_LENGTH) {
        LOG(INFO) << "AlgorithmPkt::decodeHeader length is greater than MAX_PACKET_LENGTH";
        return false;
    }

    //body至少包含4字节类型和4字节crc校验,msg不能为空，否则也会扔掉
    if (length - HEADER_LENGTH > 4) {
        bodyLength_ = length - HEADER_LENGTH;
    } else {
        LOG(INFO) << "AlgorithmPkt::decodeHeader length is too short";
        return false;
    }

    //data_为收到到网络字节，没有经过大小端处理
    data_.clear();
    data_.insert(data_.end(), buffHead_, buffHead_ + HEADER_LENGTH);
    std::vector<uint8_t> body(bodyLength_);
    data_.insert(data_.end(), body.begin(), body.end());
    return true;
}

bool AlgorithmPkt::decodeBody(AlgoParamMsg &pkt) {
    bool ret = false;

    boost::crc_optimal<32, 0x04C11DB7, 0xFFFFFFFF, 0xFFFFFFFF, true, true> result;
    result.process_bytes(data_.data(), data_.size() - 4);
    uint32_t checkSum = result.checksum();
    //大小端处理
    uint8_t buffCS[4];
    memcpy(buffCS, data_.data() + data_.size() - 4, 4);
    uint32_t readCS = buffCS[0];
    readCS = (readCS << 8) | uint32_t(buffCS[1]);
    readCS = (readCS << 8) | uint32_t(buffCS[2]);
    readCS = (readCS << 8) | uint32_t(buffCS[3]);
    //校验crc值
    ret = checkSum == readCS;
    if (!ret) {
        LOG(INFO) << "AlgorithmPkt::decodeBody crc check failed";
        return ret;
    }

    try {
        //除去头尾个4字节为msg内容
        if (!pkt.ParseFromArray(data_.data() + 4, data_.size() - 8)) {
            LOG(INFO) << "AlgorithmPkt::decodeBody ParseFromArray error";
            return false;
        }
        return true;
    } catch (std::exception &e) {
        LOG(INFO) << "AlgorithmPkt::decodeBody ParseFromArray error";
        return false;
    }
}

bool AlgorithmPkt::encode(std::vector<uint8_t> &sendData, const AlgoParamMsg &msg) {
    int objSize = msg.ByteSize();
    std::vector<uint8_t> msgBuff(objSize);
    bool ret = false;
    try {
        if (!msg.SerializeToArray(msgBuff.data(), objSize)) {
            LOG(INFO) << "AlgorithmPkt::encode SerializeToArray error";
            return false;
        }
    } catch (std::exception &e) {
        LOG(INFO) << "AlgorithmPkt::encode SerializeToArray error:" << e.what();
        return false;
    }

    //数据长度为长度字段4字节 + crc校验4字节 + 数据长度
    int length = msgBuff.size() + 4 + 4;
    //大小端处理
    char lengthBuff[4];
    lengthBuff[0] = length >> 24;
    lengthBuff[1] = length >> 16;
    lengthBuff[2] = length >> 8;
    lengthBuff[3] = length;
    sendData.insert(sendData.end(), lengthBuff, lengthBuff + 4);

    //数据内容，大小端已经由protobuff处理了，不用大小端处理
    sendData.insert(sendData.end(), msgBuff.begin(), msgBuff.end());

    //crc
    boost::crc_optimal<32, 0x04C11DB7, 0xFFFFFFFF, 0xFFFFFFFF, true, true> result;
    result.process_bytes(sendData.data(), sendData.size());
    //在大端序下生成crc校验和
    int checkSum = result.checksum();
    char crcBuff[4];
    crcBuff[0] = checkSum >> 24;
    crcBuff[1] = checkSum >> 16;
    crcBuff[2] = checkSum >> 8;
    crcBuff[3] = checkSum;
    sendData.insert(sendData.end(), crcBuff, crcBuff + 4);
    return true;
}
