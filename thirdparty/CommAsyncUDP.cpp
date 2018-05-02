#include "CommAsyncUDP.hpp"
#include "commlog.h"
#include "shareddata.h"

using boost::asio::ip::udp;

static boost::mutex glog_f_mtx;

void CommAsyncUDP::send_msg(EndpointEnum endp, const char *msg, int msglen) {
    if (msglen <= 0)
        return;

    auto msgptr = boost::make_shared<CommMessage>(msg, msg + msglen);
    send_msg(endp, msgptr);
}

void CommAsyncUDP::send_msg(EndpointEnum endp, CommMessagePtr msg) {
    if (msg->size() == 0)
        return;

//  STR_TO_LOG_FILE("*******************send buffer*******************");
//  CHAR_BUFF_TO_LOG_FILE(*msg);
//  STR_TO_LOG_FILE("*******************send buffer end*******************");
    LOG(INFO) << "*******************send buffer*******************";
    CHAR_BUFF_TO_LOG(*msg);

    UDPEndpointPtr peerEndpPtr = UDPEndpoints::Get()->fetch(endp);
    sdp_->async_send_to(boost::asio::buffer(*msg), *peerEndpPtr, // 立即异步发送该msg,注意该msg在bind中被保留
                        boost::bind(&CommAsyncUDP::handle_send, this, _1, _2, endp, msg)); // 发送结束后的处理
}

bool CommAsyncUDP::sync_send_msg(EndpointEnum endp, CommMessagePtr msg, int timeout) {
    UDPEndpointPtr peerEndpPtr = UDPEndpoints::Get()->fetch(endp);
    sdp_->async_send_to(boost::asio::buffer(*msg), *peerEndpPtr, // 立即异步发送该msg,注意该msg在bind中被保留
                        boost::bind(&CommAsyncUDP::handle_send, this, _1, _2, endp, msg)); // 发送结束后的处理
    //等服务器返回，10S
    boost::unique_lock<boost::mutex> lock(syncReqRespMtx);
    syncSendMsg = true;
    return boost::cv_status ::no_timeout == syncReqRespCv.wait_for(lock, boost::chrono::milliseconds(timeout));
}

void CommAsyncUDP::register_recvhandler(RecvhandlerExternal recvHandlerExternal) {
    recvHandlerExternal_.lock();
    CHECK(!recvHandlerExternal_.data) << "can only registered once";
    recvHandlerExternal_.data = recvHandlerExternal;
    recvHandlerExternal_.unlock();
}

void CommAsyncUDP::handle_send(const boost::system::error_code &error, std::size_t sz, EndpointEnum endp,
                               CommMessagePtr msg) {
    size_t msglen = msg->size();
    if (error) {
        LOG(INFO) << error.message();
        return;
    }
    if (msglen != sz) {
        LOG(INFO) << "need to send " << msglen << " bytes, but actually send " << sz << "bytes";
        return;
    }
    if (sz == 0) {
        LOG(INFO) << "send a zero length msg";
        return;
    }

    int buflen = msg->size();
//    char buffer[buflen]{0};
    std::vector<char> buffer(buflen);
    std::copy(msg->begin(), msg->end(), buffer.data());
    ProtocolStruct protocolStruct;
    if (protocolStruct.decode(buffer.data(), buflen) && CommProtoVariables::REQUEST == protocolStruct.packFlags.bits.ReqResp)
        SharedData::Get()->pushSendData(protocolStruct);
}

void CommAsyncUDP::handle_recv(const boost::system::error_code &error, std::size_t sz) {
    if (error) {
        LOG(INFO) << error.message();
    } else {
        // 现在信息有效
        if(syncSendMsg) {
            //通知同步等待的发送线程
            LOG(INFO) << "CommAsyncUDP::handle_recv sync received a packet";
            boost::unique_lock<boost::mutex> lock(syncReqRespMtx);
            syncSendMsg = false;
            syncReqRespCv.notify_one();
        }
        LOG(INFO) << "*******************receive buffer*******************";
        char *char_buff = recvBuf_.data();
        CHAR_BUFF_TO_LOG(std::vector<char>(recvBuf_.data(), recvBuf_.data() + sz));

        recvHandlerExternal_.lock();
        if (recvHandlerExternal_.data) {
            CommMessagePtr msgptr = boost::make_shared<CommMessage>(recvBuf_.data(), recvBuf_.data() + sz);
            recvHandlerExternal_.data(peerEndp_, msgptr);
        } else {
            LOG(INFO) << "recvHandlerExternal is invalid";
        }
        recvHandlerExternal_.unlock();
    }

    sdp_->async_receive_from(boost::asio::buffer(recvBuf_), peerEndp_,
                             boost::bind(&CommAsyncUDP::handle_recv, this, _1, _2));
}

void CommAsyncUDP::run() {
    boost::asio::io_service::work work(io_service_);
    io_service_.run();
}

CommAsyncUDP::CommAsyncUDP() {
    syncSendMsg = false;
    try {
//        sdp_ = boost::make_shared<udp::socket>(io_service_);
//        sdp_->open(udp::v4());
//        boost::asio::socket_base::reuse_address option(true);
//        sdp_->set_option(option);
//        sdp_->bind(udp::endpoint(udp::v4(), IPPortCfg::Get()->COMM_SELF_PORT()));

        udp::endpoint endpoint(udp::v4(), IPPortCfg::Get()->COMM_SELF_PORT());
        sdp_ = boost::make_shared<udp::socket>(io_service_, endpoint);

        std::string remoteIp = IPPortCfg::Get()->COMM_CAMERA_IP();
        unsigned short remotePort = boost::lexical_cast<unsigned short>(IPPortCfg::Get()->COMM_CAMERA_PORT());
        udp::endpoint remoteEp = udp::endpoint(boost::asio::ip::address_v4::from_string(remoteIp), remotePort);
        sdp_->connect(remoteEp);

        LOG(INFO) << "CommAsyncUDP::CommAsyncUDP locale endpoints:" << sdp_->local_endpoint();
        LOG(INFO) << "CommAsyncUDP::CommAsyncUDP remote endpoint:" << remoteEp;

        sdp_->async_receive_from(boost::asio::buffer(recvBuf_), peerEndp_,
                                 boost::bind(&CommAsyncUDP::handle_recv, this, _1, _2));
    } catch (std::exception &e) {
        sdp_.reset(); //绑定端口失败，返回空的socket
        // 退出程序
        LOG(FATAL) << "CommAsyncUDP::CommAsyncUDP got an error:" << e.what()
                   << "\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!exit!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
    }

    threadp_ = boost::make_shared<boost::thread>(&CommAsyncUDP::run, this);
}

boost::shared_ptr<CommAsyncUDP> CommAsyncUDP::Get() {
    static boost::shared_ptr<CommAsyncUDP> instance(new CommAsyncUDP);
    if (!instance->sdp_) {
        return nullptr;
    }
    else {
        return instance;
    }
}

//GENERATE_CLASS_SINGLETON_GET(CommAsyncUDP)
