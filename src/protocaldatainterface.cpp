#include "protocaldatainterface.h"
#include "../thirdparty/commlog.h"
#include "receivedatadispatcher.h"
#include <boost/lexical_cast.hpp>
#include <boost/asio.hpp>
#include <iostream>

int ProtocolDataInterface::reqMaxRetry = 0;
long ProtocolDataInterface::reqIntervalUS = 5000000;
ProtocolDataInterface::ProtocolDataInterface(DispatcheType type) :
    dispatcheType(type),
    valid(false)
{
//    valid = transmitLocaleIp();
}

void ProtocolDataInterface::registerSelf2Handler()
{
    //can not use shared_from_this() in constructor, so use it here.
    ReceiveDataDispatcher::getInstance()->registerDataHandler(shared_from_this());
}

void ProtocolDataInterface::sendCmd(CommDeviceEnum device, Remo_CmdSet_e cmdSet, int cmdId, bool needAckApp, bool needAckProto,
                 CommProtoVariables::RequestRespond reqres, std::vector<uint8_t> data, int maxRetry, long intervalUS)
{
//    if (!(cmdSet == Remo_CmdSet_Camera &&(cmdId == Remo_CmdId_Camera_Set_CapOperation || cmdId == Remo_CmdId_Camera_Set_RecOperation ||
//          cmdId == Remo_CmdId_Camera_Get_WorkMode || cmdId == Remo_CmdId_Camera_Set_WorkMode ||
//                                          cmdId == Remo_CmdId_Camera_Get_ImageResolution_Range ||
//                                          cmdId == Remo_CmdId_Camera_Set_ImageResolution ||
//                                          cmdId == Remo_CmdId_Camera_Get_ImageResolution ||
//                                          cmdId == Remo_CmdId_Camera_Get_LapseCap_Interval_Range ||
//                                          cmdId == Remo_CmdId_Camera_Get_LapseCap_TotalTime_Range))) {
//        return;
//    }

//    if (((cmdId >> 3) >= 0x0 && (cmdId >> 3) < 0x60 ||
//         (cmdId >> 3) >= 0x67 && (cmdId >> 3) < 0x78 ||
//         (cmdId >> 3) >= 0x7b && (cmdId >> 3) < 0x85)) {
//        return;
//    }

//    if (!((cmdId & 0x1ff) >= 0x0 && (cmdId & 0x1ff) < 0x60)) return;
//    if (Remo_CmdId_Camera_Get_SlowMotion_Resolution_Range != cmdId) {
//        return;
//    }

//    if (!(cmdSet == Remo_CmdSet_Camera &&(cmdId == Remo_CmdId_Camera_Get_WorkMode || cmdId == Remo_CmdId_Camera_Set_WorkMode))) return;

//    if (cmdSet != Remo_CmdSet_Gimbal) return;


//    if (!(cmdSet == Remo_CmdSet_Camera &&(cmdId == Remo_CmdId_Camera_Get_WhiteBalance ||
//                                          cmdId == Remo_CmdId_Camera_Set_WhiteBalance ||
//                                          cmdId == Remo_CmdId_Camera_Get_WhiteBalance_Range))) return;

//    if (cmdId != Remo_CmdId_Camera_Set_ZoomControlParam) return;

//    if (!(cmdId == Remo_CmdId_Camera_Get_WhiteBalance || cmdId == Remo_CmdId_Camera_Get_WhiteBalance_Range)) return;

//    if (cmdId != Remo_CmdId_Camera_Get_VideoFormat) return;

//    if (cmdSet != Remo_CmdSet_Camera) return;

//    if (cmdSet != Remo_CmdSet_Battery) return;

//    if (cmdId != 0x20) return;

    return;

    Remo_CmdId_Camera_e idValue = static_cast<Remo_CmdId_Camera_e>(cmdId & 0x1ff);
    Remo_CmdId_Type_e idType = static_cast<Remo_CmdId_Type_e>(cmdId >> 9);
    LOG(INFO) << "#########################ProtocolDataInterface::sendCmd" << std::hex << " cmdSet = " <<  cmdSet << " cmdId = " << cmdId
              << " idValue = " << idValue << " idType = " << idType << std::endl;
    LOG(INFO) << "data is ";
    CHAR_BUFF_TO_LOG(data);
//    CHAR_BUFF_TO_LOG_STDERROR(data);

    auto sender = CommProtoVariables::Get();
    auto msginfo = sender->gen_request_respond(
                device, static_cast<CommCmdSetEnum>(cmdSet),
                static_cast<CommCmdIDEnum>(cmdId), needAckApp, needAckProto,
                reqres, reinterpret_cast<char*>(data.data()), data.size());

//    LOG(INFO) << "Id forward is " << msginfo.taskidForward << std::endl;

    if (-1 == maxRetry) maxRetry = reqMaxRetry;
    if (-1 == intervalUS) intervalUS = reqIntervalUS;
    sender->do_request(msginfo, maxRetry, intervalUS);
}

void ProtocolDataInterface::sendCmdCamera(Remo_CmdId_Camera_e cmdId, std::vector<uint8_t> data, CommProtoVariables::RequestRespond reqres)
{
    ProtocolDataInterface::sendCmd(COMMDEVICE_CAMERA, Remo_CmdSet_Camera, cmdId, true, false, reqres, data);
}

void ProtocolDataInterface::sendCmdCamera(Remo_CmdId_Camera_e cmdId, int maxRety, long intervalUS, std::vector<uint8_t> data, CommProtoVariables::RequestRespond reqres)
{
    ProtocolDataInterface::sendCmd(COMMDEVICE_CAMERA, Remo_CmdSet_Camera, cmdId, true, false, reqres, data, maxRety, intervalUS);
}

void ProtocolDataInterface::sendCmdGimbal(Remo_CmdId_Gimbal_e cmdId, std::vector<uint8_t> data, CommProtoVariables::RequestRespond reqres)
{
    ProtocolDataInterface::sendCmd(COMMDEVICE_GIMBAL, Remo_CmdSet_Gimbal, cmdId, true, false, reqres, data);
}

void ProtocolDataInterface::sendCmdBattery(Remo_CmdId_Battery_e cmdId, std::vector<uint8_t> data, CommProtoVariables::RequestRespond reqres)
{
    ProtocolDataInterface::sendCmd(COMMDEVICE_BATTERY, Remo_CmdSet_Battery, cmdId, true, false, reqres, data);
}

void ProtocolDataInterface::sendCmdUniversal(int cmdId, vector<uint8_t> data,
                                                 CommProtoVariables::RequestRespond reqres) {
    ProtocolDataInterface::sendCmd(COMMDEVICE_CAMERA, Remo_CmdSet_Universal, cmdId, true, false, reqres, data);
}

bool ProtocolDataInterface::isBigEndian()
{
    uint16_t * u16 = new uint16_t(0xff01);
    return !((*(char*)u16) == 0x1);
}

void ProtocolDataInterface::endianTurn(std::vector<uint8_t> &data)
{
//    std::vector<uint8_t> turnData;
//    for (std::vector<uint8_t>::reverse_iterator it = data.rbegin(); it != data.rend(); ++it) {
//        turnData.push_back(*it);
//    }

//    data = turnData;
}

bool ProtocolDataInterface::transmitLocaleIp() {
//    bool ret = false;
//    using boost::asio::ip::udp;
//
//    try {
//        std::string remoteIp =  IPPortCfg::Get()->COMM_CAMERA_IP();
//        std::string remotePort = IPPortCfg::Get()->COMM_CAMERA_PORT();
//
//        boost::asio::io_service netService;
//        udp::resolver   resolver(netService);
//        udp::resolver::query query(udp::v4(), remoteIp, remotePort);
//        udp::resolver::iterator endpoints = resolver.resolve(query);
//        udp::socket socket(netService);
//        udp::endpoint ep = *endpoints;
//        boost::system::error_code ec;
//
//        socket.connect(ep, ec);
//        LOG(INFO) << "ProtocolDataInterface::getConfigedIp ec:" << ec.message() << ec.value();
//        int size = socket.send_to(boost::asio::buffer("Hello"), ep);
//
////        boost::array<char, 128> recv_buffer;
////        socket.receive_from(boost::asio::buffer(recv_buffer), ep);
//
//        ip = socket.local_endpoint().address().to_v4().to_ulong();
//        port = IPPortCfg::Get()->COMM_SELF_PORT();
//        LOG(INFO) << "ProtocolDataInterface::getConfigedIp locale endpoints:" << socket.local_endpoint();
//        LOG(INFO) << "ProtocolDataInterface::getConfigedIp remote endpoint:" << ep;
//        ret = true;
//    } catch (std::exception &e) {
//        LOG(INFO) << "ProtocolDataInterface::getConfigedIp got an error:" << e.what();
//        ret = false;
//    }
//
//    return ret;

    using boost::asio::ip::udp;
    udp::endpoint dp = CommAsyncUDP::Get()->getLocaleEp();
    uint32_t ip = dp.address().to_v4().to_ulong();
    uint16_t port = dp.port();
    std::vector<uint8_t> data(6);
    memcpy(data.data(), &ip, 4);
    memcpy(data.data() + 4, &port, 2);
    //上报本机ip和端口
    valid = ProtocolDataInterface::syncSendUniversal(static_cast<Remo_CmdId_Camera_e>(0x20), data);
    return valid;
}

bool ProtocolDataInterface::syncSendCmd(CommDeviceEnum device, Remo_CmdSet_e cmdSet, int cmdId, bool needAckApp,
                                        bool needAckProto, CommProtoVariables::RequestRespond reqres,
                                        std::vector<uint8_t> data, int timeout) {
//    return true;

    Remo_CmdId_Camera_e idValue = static_cast<Remo_CmdId_Camera_e>(cmdId & 0x1ff);
    Remo_CmdId_Type_e idType = static_cast<Remo_CmdId_Type_e>(cmdId >> 9);
    LOG(INFO) << "#########################ProtocolDataInterface::syncSendCmd" << std::hex << " cmdSet = " <<  cmdSet << " cmdId = " << cmdId
              << " idValue = " << idValue << " idType = " << idType << std::endl;
    LOG(INFO) << "data is ";
    CHAR_BUFF_TO_LOG(data);
//    CHAR_BUFF_TO_LOG_STDERROR(data);

    auto sender = CommProtoVariables::Get();
    auto msginfo = sender->gen_request_respond(
            device, static_cast<CommCmdSetEnum>(cmdSet),
            static_cast<CommCmdIDEnum>(cmdId), needAckApp, needAckProto,
            reqres, reinterpret_cast<char*>(data.data()), data.size());

    return sender->sync_do_request(msginfo, timeout);
}

bool ProtocolDataInterface::syncSendCamera(Remo_CmdId_Camera_e cmdId, vector<uint8_t> data,int timeout,
                                           CommProtoVariables::RequestRespond reqres) {
    return ProtocolDataInterface::syncSendCmd(COMMDEVICE_CAMERA, Remo_CmdSet_Camera, cmdId, true, false, reqres, data, timeout);
}

bool ProtocolDataInterface::syncSendUniversal(Remo_CmdId_Camera_e cmdId, vector<uint8_t> data, int timeout,
                                              CommProtoVariables::RequestRespond reqres) {
    return ProtocolDataInterface::syncSendCmd(COMMDEVICE_CAMERA, Remo_CmdSet_Universal, cmdId, true, false, reqres, data, timeout);
}

