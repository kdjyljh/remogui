//
// Created by jianghualuo on 18-4-8.
//

#ifndef ALGORITHMCLIENTTEST_ALGORITHMTCPMSGCLIENT_H
#define ALGORITHMCLIENTTEST_ALGORITHMTCPMSGCLIENT_H

#include <iostream>
#include <string>
#include <deque>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread.hpp>

#include "AlgorithmPkt.h"

using boost::asio::ip::tcp;
using boost::asio::deadline_timer;

class AlgorithmTcpMsgClient {
public:
    typedef boost::function<void (const AlgoParamMsg &msg)> ReceiveMsgHandler;
    AlgorithmTcpMsgClient();
    ~AlgorithmTcpMsgClient();
    bool connect(int timeout = 5);
    bool asyncSendMsg(const AlgoParamMsg &msg);
    void registerReceiveMsgHandler(ReceiveMsgHandler handler) {receiveMsgHandler = handler;}

private:
    void handleConnect(const boost::system::error_code& error);
    void handleSendMsg(const boost::system::error_code& error,
                       std::size_t bytes_transferred, std::vector<uint8_t> buff);
    void handleReceiveMsgHeader(const boost::system::error_code& error, std::size_t bytes_transferred);
    void handleReceiveMsgBody(const boost::system::error_code& error, std::size_t bytes_transferred);
    void checkTimer(boost::system::error_code& ec);
    void doClose();
    void pushReceiveMsg(const AlgoParamMsg &pkt);
    void popReceiveMsg(AlgoParamMsg &pkt);
    bool asyncSendMsg_(std::vector<uint8_t> buff);
    void receiveMsgLoop();

private:
    boost::asio::io_service ioService;
    boost::system::error_code cntEc;
    tcp::socket socket;
    deadline_timer deadlineTimer;
    std::string remoteIp;
    std::string remotePort;
    bool errorOccurs;
    AlgorithmPkt curPkt;
    std::deque<AlgoParamMsg> receiveMsgQueue;
    boost::mutex receiveMsgQueueMtx;
    boost::condition_variable receiveMsgQueueCv;
    ReceiveMsgHandler receiveMsgHandler;
    boost::thread receiveMsgThread;
    boost::thread runThread;
};

#endif //ALGORITHMCLIENTTEST_ALGORITHMTCPMSGCLIENT_H
