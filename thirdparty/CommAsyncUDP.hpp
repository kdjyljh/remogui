#ifndef COMMASYNCUDP_HPP
#define COMMASYNCUDP_HPP

#include "Mix.hpp"
#include "CommGlobal.hpp"
#include <boost/function.hpp>

class CommAsyncUDP : public boost::noncopyable {
public:
    typedef boost::function<void(UDPEndpoint, CommMessagePtr)> RecvhandlerExternal;

    void send_msg(EndpointEnum endp, const char *msg, int msglen);

    void send_msg(EndpointEnum endp, CommMessagePtr msg);
    bool sync_send_msg(EndpointEnum endp, CommMessagePtr msg, int timeout = 1000); //同步发送数据包
    void register_recvhandler(RecvhandlerExternal recvHandlerExternal);

    UDPEndpoint getLocaleEp() {return sdp_->local_endpoint();}

    static boost::shared_ptr<CommAsyncUDP> Get();

private:
    UDPSocketPtr sdp_;
    boost::asio::io_service io_service_;

    boost::mutex syncReqRespMtx;
    boost::condition_variable syncReqRespCv;
    bool syncSendMsg;

    void handle_send(const boost::system::error_code &error, std::size_t sz, EndpointEnum endp, CommMessagePtr msg);

    boost::array<char, 4096> recvBuf_;
    UDPEndpoint peerEndp_;

    void handle_recv(const boost::system::error_code &error, std::size_t sz);

    DataGuardedManual<RecvhandlerExternal> recvHandlerExternal_;

    boost::shared_ptr<boost::thread> threadp_;

    void run();

    CommAsyncUDP();
};

#endif //COMMASYNCUDP_HPP
