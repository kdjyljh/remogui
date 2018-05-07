//
// Created by jianghualuo on 18-4-8.
//

#ifndef ALGORITHMCLIENTTEST_ALGORITHMMSG_H
#define ALGORITHMCLIENTTEST_ALGORITHMMSG_H

#include <vector>
#include <stdint.h>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include "test.pb.h"
#include "AlgoParam.pb.h"

//typedef boost::shared_ptr<google::protobuf::Message> MsgType;
//struct AlgorithmMsgPkt {
//    AlgorithmMsgPkt(){}
//
//    AlgorithmMsgPkt(AlgoParam::MessageType_Enum type) {
//        this->type = type;
//        msg = generateMsgByType(type);
//    }
//
//    static MsgType generateMsgByType(AlgoParam::MessageType_Enum type) {
//        if (TEST == type) {
//            return boost::make_shared<test>();
//        }
//        else if (AlgoParam::MessageType::Selection == type) {
//            return boost::make_shared<AlgoParam::Selection>();
//        } else if (AlgoParam::MessageType::Capture == type) {
//            return boost::make_shared<AlgoParam::Capture>();
//        }
//        //没有对应到msg类型，返回空msg
//        return MsgType();
//    }
//
//    AlgoParam::MessageType_Enum type;
//    //google::protobuf::Message为抽象基类，使用boost智能指针管理对象生命周期
//    //使用时注意判断是否为有效
//    MsgType msg;
//};

typedef AlgoParam::MsgUnity AlgoParamMsg;
class AlgorithmPkt {
public:
    enum {
        HEADER_LENGTH = 4
    };
    enum {
        MAX_PACKET_LENGTH = AlgoParam::MSG_MAX_LENGTH
    };
    AlgorithmPkt();
    char *dataHead() { return buffHead_;}
    char *dataBody() { return reinterpret_cast<char*>(data_.data() + HEADER_LENGTH);}
    int bodyLength() { return bodyLength_;}
    bool decodeHeader();
    bool decodeBody(AlgoParamMsg &content);
    static bool encode(std::vector<uint8_t> &sendData, const AlgoParamMsg &msg);

private:

private:
    int bodyLength_;
    char buffHead_[HEADER_LENGTH];
    std::vector<uint8_t> data_;
};


#endif //ALGORITHMCLIENTTEST_ALGORITHMMSG_H
