#include "CommAsyncUDP.hpp"
#include "commlog.h"
#include "shareddata.h"

using boost::asio::ip::udp;

static boost::mutex glog_f_mtx;

void CommAsyncUDP::send_msg(EndpointEnum endp, const char *msg, int msglen) {
  if (msglen <= 0)
    return;

  auto msgptr = boost::make_shared<CommMessage>(msg, msg+msglen);
  send_msg(endp, msgptr);
}

void CommAsyncUDP::send_msg(EndpointEnum endp, CommMessagePtr msg) {
  if (msg->size() == 0)
    return;

//  STR_TO_LOG_FILE("*******************send buffer*******************");
//  CHAR_BUFF_TO_LOG_FILE(*msg);
//  STR_TO_LOG_FILE("*******************send buffer end*******************");

  UDPEndpointPtr peerEndpPtr = UDPEndpoints::Get()->fetch(endp);
  sdp_->async_send_to(boost::asio::buffer(*msg), *peerEndpPtr, // 立即异步发送该msg,注意该msg在bind中被保留
                      boost::bind(&CommAsyncUDP::handle_send, this, _1, _2, endp, msg)); // 发送结束后的处理
}

void CommAsyncUDP::register_recvhandler(RecvhandlerExternal recvHandlerExternal) {
  recvHandlerExternal_.lock();
  CHECK(! recvHandlerExternal_.data) << "can only registered once";
  recvHandlerExternal_.data = recvHandlerExternal;
  recvHandlerExternal_.unlock();
}

void CommAsyncUDP::handle_send(const boost::system::error_code &error, std::size_t sz, EndpointEnum endp, CommMessagePtr msg) {
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
  char buffer[buflen]{0};
  std::copy(msg->begin(), msg->end(), buffer);
  ProtocolStruct protocolStruct;
  if (protocolStruct.decode(buffer, buflen) && CommProtoVariables::REQUEST == protocolStruct.packFlags.bits.ReqResp)
      SharedData::Get()->pushSendData(protocolStruct);
}

void CommAsyncUDP::handle_recv(const boost::system::error_code &error, std::size_t sz) {
  if (error) {
    LOG(INFO) << error.message();
  } else {
    // 现在信息有效
    recvHandlerExternal_.lock();
    if (recvHandlerExternal_.data) {
      CommMessagePtr msgptr = boost::make_shared<CommMessage>(recvBuf_.data(), recvBuf_.data()+sz);
      recvHandlerExternal_.data(peerEndp_, msgptr);
    } else {
      LOG(INFO) << "recvHandlerExternal is invalid";
    }
    recvHandlerExternal_.unlock();
  }

//  STR_TO_LOG_FILE("*******************receive buffer*******************");
//  CHAR_BUFF_TO_LOG_FILE(recvBuf_);
//  STR_TO_LOG_FILE("*******************receive buffer end*******************");

  sdp_->async_receive_from(boost::asio::buffer(recvBuf_), peerEndp_,
                           boost::bind(&CommAsyncUDP::handle_recv, this, _1, _2));
}

void CommAsyncUDP::run() {
  boost::asio::io_service::work work(io_service_);
  io_service_.run();
}

CommAsyncUDP::CommAsyncUDP() {
  LOG(INFO) << "CommAsyncUDP";
//  sdp_ = boost::make_shared<udp::socket>(io_service_, udp::endpoint(udp::v4(), COMM_SELF_PORT));

  sdp_ = boost::make_shared<udp::socket>(io_service_);
  sdp_->open(udp::v4());
  boost::asio::socket_base::reuse_address option(true);
  sdp_->set_option(option);
  sdp_->bind(udp::endpoint(udp::v4(), IPPortCfg::Get()->COMM_SELF_PORT()));

  sdp_->async_receive_from(boost::asio::buffer(recvBuf_), peerEndp_,
                           boost::bind(&CommAsyncUDP::handle_recv, this, _1, _2));
  
  threadp_ = boost::make_shared<boost::thread>(&CommAsyncUDP::run, this);
}
GENERATE_CLASS_SINGLETON_GET(CommAsyncUDP)
