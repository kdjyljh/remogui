#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP

#include "systemusrlib.h"
#include "TimedTask.hpp"
#include "CommAsyncUDP.hpp"

typedef std::vector<char> CommMessage;
typedef boost::shared_ptr<CommMessage> CommMessagePtr;
extern const int protocolHeaderLength; // 完整版协议的通信帧的帧头长度

/*
 Sync(8b) Version(4b) Len(12b) PackFlags(8b)
 PackSeq(16b) Checksum(16b)
 EncryIdx(8b) Sender(4b) Recver(4b) CmdSet(4b) CmdID(12b)
 */

struct ProtocolStruct {
    uint8_t sync = 0xAA;
    uint16_t ver;   //  : 4;
    uint16_t len;   //  : 12;
    union {
        uint8_t byte;
        struct {
            uint8_t ReqResp        : 1;
            uint8_t AppAckType     : 1;
            uint8_t ProtoAckType   : 1;
            uint8_t EncryEnable    : 1;
            uint8_t CheckSumEnable : 1;
            uint8_t SubSystem      : 1;
            uint8_t Reserve        : 2;
        } bits;
    } packFlags;
    uint16_t packSeq;
    uint16_t checksum;
    uint8_t encryIdx;
    uint8_t sender;// : 4;
    uint8_t recver;// : 4;
    uint16_t cmdSet;// : 4;
    uint16_t cmdID; // : 12;
    vector<uint8_t> data;

    // msg和msglen: 存储的位置与其长度;  返回: 消息总长
    int encode(char *const msg, int msglen); // 会更新len和checksum
    // msg和msglen: 接收的消息与其长度;  返回: true(正确), false(错误)
    bool decode(const char *const msg, int msglen);

    CommMessagePtr encode();

    bool decode(CommMessagePtr msgptr);

    inline TimedTaskID idForward() { // 从sender到recver
        uint8_t sendrecv = (sender << 4) | recver;
        uint16_t cmd = (cmdSet << 12) | cmdID;
        uint64_t idPostfix = (static_cast<uint64_t>(ver) << 40) | // 4  bit
                             (static_cast<uint64_t>(packSeq) << 24) | // 16 bit
                             (static_cast<uint64_t>(sendrecv) << 16) | // 8  bit
                             (static_cast<uint64_t>(cmd) << 0);  // 16 bit
        LOG(INFO) << "TimedTaskID idForward " << print();
        return timed_taskid_gen(timedTaskIDPrefix4CommTask, idPostfix);
    }

    inline TimedTaskID idBackward() { // 从recver到sender
        uint8_t recvsend = (recver << 4) | sender;
        uint16_t cmd = (cmdSet << 12) | cmdID;
        uint64_t idPostfix = (static_cast<uint64_t>(ver) << 40) | // 4  bit
                             (static_cast<uint64_t>(packSeq) << 24) | // 16 bit
                             (static_cast<uint64_t>(recvsend) << 16) | // 8  bit
                             (static_cast<uint64_t>(cmd) << 0);  // 16 bit
        LOG(INFO) << "id backward: " << print();
        return timed_taskid_gen(timedTaskIDPrefix4CommTask, idPostfix);
    }

    string print();

    void test();

private:
    uint16_t crc16(uint8_t *DataBuf, uint16_t DataLen);
};

#include "CommDeviceEnum.hpp"
#include "CommCmdSetEnum.hpp"
#include "CommCmdIDEnum.hpp"

class CommProtoVariables : public boost::noncopyable {
public:
    enum RequestRespond {
        REQUEST = 0,
        RESPOND = 1,
    };
    // 生成用于发送的request和respond数据包
    struct MSGinfo {
        CommMessagePtr msgptr;
        TimedTaskID taskidForward; // 发送的消息使用taskidForward注册定时任务,接收的消息使用taskidBackward取消定时任务
    };

    void request_ping(CommDeviceEnum target);

    void request_status(CommDeviceEnum target);

    void request_version(CommDeviceEnum target);

    void respond_ping(CommDeviceEnum target, uint16_t packseq);

    void respond_status(CommDeviceEnum target, uint16_t packseq);

    void respond_version(CommDeviceEnum target, uint16_t packseq);

    void check_recved(ProtocolStruct &proto);

    static boost::shared_ptr<CommProtoVariables> Get();

    CommProtoVariables::MSGinfo gen_request_respond(
            CommDeviceEnum target, CommCmdSetEnum cmdSet, CommCmdIDEnum cmdID,
            bool needAckApp, bool needAckProto, RequestRespond reqres,
            char *data, int datalen);

    // 处理用于发送的request和respond数据包
    void do_request(MSGinfo msginfo, int maxRetry, long intervalUS);
    void do_respond(MSGinfo msginfo);
    //同步发送数据包:timeout单位ms
    bool sync_do_request(MSGinfo msginfo, int timeout);

private:
    boost::atomic_bool recording; // 是否录像
    boost::atomic_bool AFCenabled; // 连续对焦是否使能
    boost::atomic<double> zoomValue; // 4-13cm
    DataGuarded<vector<float> > triopHeadAngles; // 云台角度，三个浮点数
    DataGuarded<vector<float> > triopHeadSpeeds; // 云台速度，三个浮点数

    CommMessagePtr recording_MSG();

    CommMessagePtr AFCenabled_MSG();

    CommMessagePtr zoomValue_MSG();

    CommMessagePtr triopHeadAngles_MSG();

    CommMessagePtr triopHeadSpeeds_MSG();

    uint16_t packSeq_;

    MSGinfo gen_request_ping(CommDeviceEnum target);

    MSGinfo gen_respond_ping(CommDeviceEnum target, uint16_t packseq);

    MSGinfo gen_request_status(CommDeviceEnum target);

    MSGinfo gen_respond_status(CommDeviceEnum target, uint16_t packseq);

    MSGinfo gen_request_version(CommDeviceEnum target);

    MSGinfo gen_respond_version(CommDeviceEnum target, uint16_t packseq);

    MSGinfo gen_request_respond(CommDeviceEnum target, CommCmdSetEnum cmdSet, CommCmdIDEnum cmdID,
                                bool needAckApp, bool needAckProto, uint16_t packSeq, RequestRespond reqres,
                                char *data = nullptr, int datalen = 0);

    typedef uint64_t UniqueRespondID;

    inline UniqueRespondID uniquerespid(CommDeviceEnum cmdDev, CommCmdSetEnum cmdSet, CommCmdIDEnum cmdID) {
        return (uint64_t(cmdDev) << 32) | (uint64_t(cmdSet) << 16) | (uint64_t(cmdID) << 0);
    }

    boost::unordered_map<UniqueRespondID, boost::function<void(
            uint16_t)>> mapRespondAction2func_; // uint16_t is packseq
    CommProtoVariables();
};

void do_recved_protocol(UDPEndpoint, CommMessagePtr);

#endif //PROTOCOL_HPP
