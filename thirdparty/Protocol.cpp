#include "Protocol.hpp"
#include "shareddata.h"

const int protocolHeaderLength       = 12; // 完整版协议的通信帧的帧头长度

/**
* CRC逆序/8005/初始值为0xFFFF/异或值为0xFFFF
*/

/* CRC 高位字节值表 */
static const uint8_t AuchCrcHi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};

/* CRC低位字节值表*/
static const uint8_t AuchCrcLo[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
    0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
    0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
    0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
    0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
    0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
    0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
    0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
    0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
    0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
    0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
    0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
    0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
    0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
    0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
    0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
    0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
    0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};

/****************************************************************
CRC校验返回正常的16位校验结果
* @param[in] 	DataBuf     要进行CRC校验的消息
* @param[in] 	DataLen     消息中字节数
注意这里的CRC16校验的初始值为0xFF 0xFF
*****************************************************************/
uint16_t ProtocolStruct::crc16(uint8_t *DataBuf, uint16_t DataLen) {
  CHECK(DataBuf && DataLen);

  uint8_t UchCrcHi = 0xFF;   /* 高CRC字节初始化 */
  uint8_t UchCrcLo = 0xFF;   /* 低CRC 字节初始化 */
  uint16_t Index;              /* CRC循环中的索引 */
  uint16_t ret = 0;

  while (DataLen--) {
    Index = UchCrcLo ^ *DataBuf++;
    UchCrcLo = UchCrcHi ^ AuchCrcHi[Index];
    UchCrcHi = AuchCrcLo[Index];
  }

  ret = ((UchCrcHi << 8) | (UchCrcLo));
  ret = ret ^ 0xFFFF;

  return (ret);
}

CommMessagePtr ProtocolStruct::encode() {
  char buf[4096];
  int len = encode(buf, sizeof(buf));
  return boost::make_shared<vector<char>>(buf, buf + len);
}

bool ProtocolStruct::decode(CommMessagePtr msgptr) {
  if (msgptr->empty())
    return false;

  char buf[msgptr->size()], *ptr = buf;
  for (auto &&byte : *msgptr)
    *ptr++ = byte;
  return decode(buf, sizeof(buf));
}

int ProtocolStruct::encode(char *const msg, int msglen) {
  this->len = protocolHeaderLength + this->data.size();
  CHECK_GE(msglen, this->len); // encode时,需要的msg内存由本地提供,可以控制,因此不满足条件时直接退出程序

  uint16_t u16;
  uint8_t *ptr = (uint8_t *) msg;
  *ptr++ = this->sync; // 同步头

  CHECK_EQ((this->len & 0xFFF), this->len);
  u16 = (this->ver << 12) | (this->len & 0xFFF); // 协议版本与帧长度
  *ptr++ = u16 >> 8;
  *ptr++ = u16;

  *ptr++ = this->packFlags.byte; // 帧标识

  u16 = this->packSeq; // 帧序号
  *ptr++ = u16 >> 8;
  *ptr++ = u16;

  uint8_t *ptrChecksum = ptr;
  *ptr++ = 0; // 先跳过checksum
  *ptr++ = 0;

  *ptr++ = this->encryIdx; // 加密随机数

  *ptr++ = (this->sender << 4) | (this->recver & 0xF); // 发送者和接收者

  u16 = (this->cmdSet << 12) | (this->cmdID & 0xFFF); // 指令集与指令ID
  *ptr++ = u16 >> 8;
  *ptr++ = u16;

  CHECK_EQ(ptr - (uint8_t *) msg, protocolHeaderLength);
  for (auto &&byte : data)
    *ptr++ = byte;
  CHECK_EQ(ptr - (uint8_t *) msg, this->len);

  this->checksum = crc16((uint8_t *) msg, this->len); // 计算crc校验和
  u16 = this->checksum;
  *ptrChecksum++ = u16 >> 8;
  *ptrChecksum++ = u16;

  return this->len;
}

bool ProtocolStruct::decode(const char *const msgconst, int msglen) {
  if (msglen < protocolHeaderLength) { // decode时,接收到的消息长度可能不够,这种情况不能由本地避免,因此返回错误而不是直接退出
//    LOG(INFO) << "msglen = " << msglen << ", protocolHeaderLength = " << protocolHeaderLength;
    return false;
  }
  char msg[msglen];
  memcpy(msg, msgconst, msglen); // 为了能够生成checksum

  uint8_t u8;
  uint16_t u16;
  uint8_t *ptr = (uint8_t *) msg;
  if (*ptr++ != this->sync) { // 同步头
//    LOG(INFO) << "this->sync = " << int(this->sync) << ", sync in msg = " << msg[0];
    return false;
  }

  u16 = *ptr++;
  u16 = (u16 << 8) | *ptr++;
  this->ver = u16 >> 12;
  this->len = u16 & 0xFFF; // 协议版本与帧长度
  if (msglen != this->len) {
//    LOG(INFO) << "this->len = " << int(this->len) << ", msglen = " << msglen;
    return false;
  }

  this->packFlags.byte = *ptr++; // 帧标识

  u16 = *ptr++;
  u16 = (u16 << 8) | *ptr++;
  this->packSeq = u16; // 帧序号

  uint8_t *ptrChecksum = ptr;
  u16 = *ptr++;
  u16 = (u16 << 8) | *ptr++;
  this->checksum = u16; // 检校和
  *ptrChecksum++ = 0;
  *ptrChecksum++ = 0;

  this->encryIdx = *ptr++; // 加密随机数

  u8 = *ptr++; // 发送者和接收者
  this->sender = u8 >> 4;
  this->recver = u8 & 0xF;

  u16 = *ptr++;
  u16 = (u16 << 8) | *ptr++; // 指令集与指令ID
  this->cmdSet = u16 >> 12;
  this->cmdID = u16 & 0xFFF;

  CHECK_EQ(ptr - (const uint8_t *) msg, protocolHeaderLength);
  int datalen = this->len - protocolHeaderLength;
  data.resize(datalen);
  for (int idx = 0; idx < datalen; ++idx)
    data[idx] = (*ptr++);

  if (packFlags.bits.CheckSumEnable) {
    uint16_t checksum = crc16((uint8_t *) msg, msglen);
    if (checksum != this->checksum) {
      LOG(INFO) << "this->checksum = " << int(this->checksum) << ", checksum = " << checksum;
      return false;
    }
  }

  return true;
}

string ProtocolStruct::print() {
  std::stringstream ss;
//  ss << CommDeviceEnum_getstr(static_cast<CommDeviceEnum>(sender)) << " -> "
//     << CommDeviceEnum_getstr(static_cast<CommDeviceEnum>(recver)) << ": ";
//  ss << CommCmdSetEnum_getstr(static_cast<CommCmdSetEnum>(cmdSet)) << ", "
//     << CommCmdIDEnum_getstr(static_cast<CommCmdIDEnum>(cmdID)) << ", seq = " << packSeq;
  return ss.str();
}

void ProtocolStruct::test() {
  char bufPack[100000], bufPack2[100000];
  for (int loop = 0; loop < 1e5; ++loop) {
    ver = rand();
    packFlags.byte = rand();
    packSeq = rand();
    checksum = rand();
    encryIdx = rand();
    sender = rand();
    recver = rand();
    cmdSet = rand();
    cmdID = rand();
    int datalen = rand() % 4000;
    data.resize(datalen);
    for (int idx = 0; idx < datalen; ++idx)
      data[idx] = rand();
    int msglen = encode(bufPack, sizeof(bufPack));
    if (loop % 1000 == 0)
      LOG(INFO) << "datalen = " << datalen << ", msglen = " << msglen;
    bool valid = decode(bufPack, msglen);
    CHECK(valid);
    int msglen2 = encode(bufPack2, sizeof(bufPack2));
    CHECK(msglen == msglen2);
    CHECK(memcmp(bufPack, bufPack2, msglen) == 0);
  }
}

#define REQUESTRESPOND_SOMETHING(TYPE, MAXRETRY, INTERVALUS) \
void CommProtoVariables::request_##TYPE(CommDeviceEnum target) { \
  int maxRetry = MAXRETRY; long intervalUS = INTERVALUS; \
  auto msginfo = gen_request_##TYPE(target); \
  do_request(msginfo, maxRetry, intervalUS); \
} \
void CommProtoVariables::respond_##TYPE(CommDeviceEnum target, uint16_t packseq) { \
  auto msginfo = gen_respond_##TYPE(target, packseq); \
  do_respond(msginfo); \
}

REQUESTRESPOND_SOMETHING(ping, 0, 50000) // 50ms
REQUESTRESPOND_SOMETHING(status, 50, 50000) // 50ms
REQUESTRESPOND_SOMETHING(version, 50, 50000) // 50ms

#undef REQUESTRESPOND_SOMETHING

void CommProtoVariables::check_recved(ProtocolStruct &proto) {
  if (proto.recver != IPPortCfg::Get()->SelfCommDevice()) {
    LOG(INFO) << "ERROR: algorithm will not recv msg for " << static_cast<CommDeviceEnum>(proto.recver);
    return;
  }

  TimedTask::Get()->cancel_work(proto.idBackward()); // 取消可能的重发任务

  bool needAckApp = proto.packFlags.bits.AppAckType, needAckProto = proto.packFlags.bits.ProtoAckType;
  RequestRespond reqres = static_cast<RequestRespond>(proto.packFlags.bits.ReqResp);
  if ((needAckApp || needAckProto) && reqres == REQUEST) { // 需要回应
    if (needAckProto) { // 协议层echo,无数据原样送回
      auto msginfo = gen_request_respond(static_cast<CommDeviceEnum>(proto.sender),
                                         static_cast<CommCmdSetEnum>(proto.cmdSet),
                                         static_cast<CommCmdIDEnum> (proto.cmdID),
                                         false, true, proto.packSeq, RESPOND);
      do_respond(msginfo);
//      LOG(INFO) << "needAckProto msg: " << proto.print();
    }
    if (needAckApp) { // 采集数据并返回
      UniqueRespondID uniqueRespID = uniquerespid(static_cast<CommDeviceEnum>(proto.sender),
                                                  static_cast<CommCmdSetEnum>(proto.cmdSet),
                                                  static_cast<CommCmdIDEnum>(proto.cmdID));
      auto it = mapRespondAction2func_.find(uniqueRespID);
      if (it != mapRespondAction2func_.end()) {
        it->second(proto.packSeq);
//        LOG(INFO) << "needAckApp msg: " << proto.print();
      } else {
        LOG(INFO) << "ERROR: algorithm can not deal with msg: \n" << proto.print();
      }
    }
  } else { // 不需要回应,则说明是对方的回复信息
    LOG(INFO) << proto.print();

    //add by ljh
    SharedData::Get()->pushData(proto);
  }
}

CommProtoVariables::MSGinfo CommProtoVariables::gen_request_respond(
                              CommDeviceEnum target, CommCmdSetEnum cmdSet, CommCmdIDEnum cmdID,
                              bool needAckApp, bool needAckProto, RequestRespond reqres,
                              char *data, int datalen)
{
    return gen_request_respond(target, cmdSet, cmdID, needAckApp, needAckProto, packSeq_++, reqres, data, datalen);
}

CommProtoVariables::MSGinfo CommProtoVariables::gen_request_respond(
                              CommDeviceEnum target, CommCmdSetEnum cmdSet, CommCmdIDEnum cmdID,
                              bool needAckApp, bool needAckProto, uint16_t packSeq, RequestRespond reqres,
                              char *data, int datalen) {
  ProtocolStruct proto;
  proto.ver = 0;
  proto.len = protocolHeaderLength + datalen;
  proto.sender = IPPortCfg::Get()->SelfCommDevice();
  proto.recver = target;
  proto.packFlags.byte = 0;
  proto.packFlags.bits.ReqResp = reqres;
  proto.packFlags.bits.AppAckType = needAckApp;
  proto.packFlags.bits.ProtoAckType = needAckProto;
  proto.packFlags.bits.CheckSumEnable = 1;
  proto.packFlags.bits.SubSystem = 0;
  proto.packSeq = packSeq;
  proto.cmdSet = cmdSet;
  proto.cmdID = cmdID;

  proto.packFlags.bits.EncryEnable = 0;
  proto.encryIdx = 0;

  if (data && datalen > 0)
    proto.data.assign(data, data + datalen);
  else
    proto.data.clear();

  MSGinfo res = {
      .msgptr = proto.encode(),
      .taskidForward = proto.idForward(),
  };
  return res;
}

CommProtoVariables::MSGinfo CommProtoVariables::gen_request_ping(CommDeviceEnum target) {
  return gen_request_respond(target, COMMCMDSET_UNIVERSAL, COMMCMDID_PING, false, true, packSeq_++, REQUEST);
}

CommProtoVariables::MSGinfo CommProtoVariables::gen_respond_ping(CommDeviceEnum target, uint16_t packseq) {
  return gen_request_respond(target, COMMCMDSET_UNIVERSAL, COMMCMDID_PING, false, true, packseq, RESPOND);
}

CommProtoVariables::MSGinfo CommProtoVariables::gen_request_status(CommDeviceEnum target) {
  return gen_request_respond(target, COMMCMDSET_UNIVERSAL, COMMCMDID_DEVICE_STATUS, true, false, packSeq_++, REQUEST);
}

CommProtoVariables::MSGinfo CommProtoVariables::gen_respond_status(CommDeviceEnum target, uint16_t packseq) {
  uint32_t status = (uint32_t(54321) << 16) | (uint32_t(12345));
  status = htonl(status);
  return gen_request_respond(target, COMMCMDSET_UNIVERSAL, COMMCMDID_DEVICE_STATUS, true, false, packseq, RESPOND, (char *)&status, sizeof(status));
}

CommProtoVariables::MSGinfo CommProtoVariables::gen_request_version(CommDeviceEnum target) {
  return gen_request_respond(target, COMMCMDSET_UNIVERSAL, COMMCMDID_DEVICE_VERSION, true, false, packSeq_++, REQUEST);
}

#pragma pack(push, 1)
struct Versions {
  char     hardware[16];
  uint32_t firmware;
  uint8_t  updateStatus;
};
#pragma pack(pop)
CommProtoVariables::MSGinfo CommProtoVariables::gen_respond_version(CommDeviceEnum target, uint16_t packseq) {
  Versions self;
  snprintf(self.hardware, sizeof(self.hardware), "I'm the algorithm");
  self.firmware = ntohl(0); self.updateStatus = 0;
  return gen_request_respond(target, COMMCMDSET_UNIVERSAL, COMMCMDID_DEVICE_VERSION, true, false, packseq, RESPOND, (char *)&self, sizeof(self));
}

void CommProtoVariables::do_request(CommProtoVariables::MSGinfo msginfo, int maxRetry, long intervalUS) {
  auto funcSend = [](CommMessagePtr msgptr) {
    CommAsyncUDP::Get()->send_msg(ENDPOINT_CAMERA, msgptr);
  };
  auto funcRetry = boost::bind<void>(funcSend, msginfo.msgptr);
  funcRetry(); // 先执行一次
  if (maxRetry > 0)
    TimedTask::Get()->register_work(msginfo.taskidForward, funcRetry, maxRetry, intervalUS); // 再注册maxRetry次
}

void CommProtoVariables::do_respond(CommProtoVariables::MSGinfo msginfo) {
  CommAsyncUDP::Get()->send_msg(ENDPOINT_CAMERA, msginfo.msgptr); // 无需重发
}

CommProtoVariables::CommProtoVariables() : packSeq_(0) {
  LOG(INFO) << "CommProtoVariables";
  auto &&commDeviceSet = CommDeviceEnumDesc::Get()->set();
  for (auto &&dev : commDeviceSet) {
    mapRespondAction2func_[uniquerespid(dev, COMMCMDSET_UNIVERSAL, COMMCMDID_DEVICE_STATUS)] =
        boost::bind(&CommProtoVariables::respond_status, this, dev, _1);
    mapRespondAction2func_[uniquerespid(dev, COMMCMDSET_UNIVERSAL, COMMCMDID_DEVICE_VERSION)] =
        boost::bind(&CommProtoVariables::respond_version, this, dev, _1);
  }
}
GENERATE_CLASS_SINGLETON_GET(CommProtoVariables)

static void do_recved_general(UDPEndpoint &endpoint, ProtocolStruct &protocolStruct);
static void do_recved_others(UDPEndpoint &endpoint, const char *const msg, int msglen);
void do_recved_protocol(UDPEndpoint endp, CommMessagePtr msgptr) {
  int buflen = msgptr->size();
  char buffer[buflen];
  std::copy(msgptr->begin(), msgptr->end(), buffer);
//  LOG(INFO) << convert_fromVector(buffer, buflen);

  ProtocolStruct protocolStruct;
  bool valid = protocolStruct.decode(buffer, buflen);
  if (valid) {
    do_recved_general(endp, protocolStruct);
  } else {
    do_recved_others(endp, buffer, buflen);
  }
}

static void do_recved_general(UDPEndpoint &endpoint, ProtocolStruct &proto) {
  LOG(INFO) << "recv msg from " << endpoint.address() << ":" << endpoint.port() << ", " << proto.print();
  CommProtoVariables::Get()->check_recved(proto);
}

static void do_recved_others(UDPEndpoint &endpoint, const char *const msg, int msglen) {
  LOG(INFO) << "ERROR! unknown peer: " << "IP = " << endpoint.address() << ", Port = " << endpoint.port();
}
