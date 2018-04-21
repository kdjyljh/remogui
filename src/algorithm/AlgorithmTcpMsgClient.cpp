//
// Created by jianghualuo on 18-4-8.
//

#include "AlgorithmTcpMsgClient.h"

#include <glog/logging.h>

AlgorithmTcpMsgClient::AlgorithmTcpMsgClient() :
        socket(ioService),
        remoteIp("192.168.61.164"),
        remotePort("24680"),
        errorOccurs(false),
        deadlineTimer(ioService) {
    deadlineTimer.expires_at(boost::posix_time::pos_infin);
    boost::system::error_code ec;
    checkTimer(ec);
    receiveMsgThread = boost::thread(&AlgorithmTcpMsgClient::receiveMsgLoop, this);
}

bool AlgorithmTcpMsgClient::connect(int timeout) {
    cntEc = boost::asio::error::would_block;
    try {
        tcp::resolver resolver(ioService);
        tcp::resolver::query query(remoteIp, remotePort);
        tcp::resolver::iterator iterator = resolver.resolve(query);
        deadlineTimer.expires_from_now(boost::posix_time::seconds(timeout));
        boost::asio::async_connect(socket, iterator,
                                   boost::bind(&AlgorithmTcpMsgClient::handleConnect, this, _1));
        do {
            ioService.run_one();
        } while (cntEc == boost::asio::error::would_block);
        if (cntEc) {

        }
        LOG(INFO) << "AlgorithmTcpMsgClient::connect local:" << socket.local_endpoint();
        LOG(INFO) << "AlgorithmTcpMsgClient::connect remote:" << socket.remote_endpoint();
        LOG(INFO) << "AlgorithmTcpMsgClient::connect status:" << cntEc.message();
        return !cntEc;
    } catch (std::exception &e) {
        LOG(INFO) << "TcpMsgProxy::connect error:" << e.what();
        errorOccurs = true;
        return false;
    }
}

bool AlgorithmTcpMsgClient::asyncSendMsg_(std::vector<uint8_t> buff) {
    //如果没有connect，先connect
    boost::system::error_code ec;
    socket.remote_endpoint(ec);
    if (ec) {
        LOG(INFO) << "AlgorithmTcpMsgClient::asyncSendMsg_ no socket and try to connect";
        if (!connect()) {//可能阻塞
            LOG(INFO) << "AlgorithmTcpMsgClient::asyncSendMsg_ no socket and reconnect failed";
            return false;
        }
    }
    //使用bind保存buff直到handle被调用
    boost::asio::async_write(socket, boost::asio::buffer(buff) ,
                             boost::bind(&AlgorithmTcpMsgClient::handleSendMsg, this, _1, _2, buff));
    return true;
}

void AlgorithmTcpMsgClient::handleConnect(const boost::system::error_code &error) {
    cntEc = error;
    if (error) {
        LOG(INFO) << "TcpMsgProxy::handleConnect error:" << error.message();
        errorOccurs = true;
        return;
    }

    //在定时器时间内成功链接，取消定时器
    if (deadlineTimer.expires_at() > deadline_timer::traits_type::now()) {
        deadlineTimer.expires_at(boost::posix_time::pos_infin);
    }

//    if (runThread.joinable()) {
//        runThread.interrupt();
//        runThread.join();
//    }
    ioService.stop();
    runThread.detach(); //自动释放上一个线程
    runThread = boost::thread(boost::bind(&boost::asio::io_service::run, &ioService));

    //读取4个字节包头，为长度
    boost::asio::async_read(socket, boost::asio::buffer(curPkt.dataHead(), AlgorithmPkt::HEADER_LENGTH),
                            boost::bind(&AlgorithmTcpMsgClient::handleReceiveMsgHeader, this, _1, _2));
    LOG(INFO) << "AlgorithmTcpMsgClient::handleConnect end bind async_read done";
}

void AlgorithmTcpMsgClient::handleSendMsg(const boost::system::error_code &error, std::size_t bytes_transferred,
                                          std::vector<uint8_t> buff) {
    if (error) {
        LOG(INFO) << "TcpMsgProxy::handleSendMsg error:" << error.message();
        errorOccurs = true;
        return;
    }

    if (bytes_transferred != buff.size()) {
        LOG(INFO) << "TcpMsgProxy::handleSendMsg error: some bytes not send";
        errorOccurs = true;
        return;
    }
}

void AlgorithmTcpMsgClient::handleReceiveMsgHeader(const boost::system::error_code &error, std::size_t bytes_transferred) {
//    LOG(INFO) << "AlgorithmTcpMsgClient::handleReceiveMsgHeader status:" << error.message();
    if (error) {
        LOG(INFO) << "AlgorithmTcpMsgClient::handleReceiveMsgHeader error:" << error.message();
        errorOccurs = true;
        return;
    }

    if (bytes_transferred != AlgorithmPkt::HEADER_LENGTH) {
        LOG(INFO) << "AlgorithmTcpMsgClient::handleReceiveMsgHeader error header length incorrect" << error.message();
        return;
    }

    if (curPkt.decodeHeader()) {
        boost::asio::async_read(socket, boost::asio::buffer(curPkt.dataBody(), curPkt.bodyLength()),
                                boost::bind(&AlgorithmTcpMsgClient::handleReceiveMsgBody, this, _1, _2));
    } else {
        //decode失败，继续读取下一条数据
        boost::asio::async_read(socket, boost::asio::buffer(curPkt.dataHead(), AlgorithmPkt::HEADER_LENGTH),
                                boost::bind(&AlgorithmTcpMsgClient::handleReceiveMsgHeader, this, _1, _2));
    }
}

void AlgorithmTcpMsgClient::handleReceiveMsgBody(const boost::system::error_code &error, std::size_t bytes_transferred) {
    if (error) {
        LOG(INFO) << "AlgorithmTcpMsgClient::handleReceiveMsgBody error:" << error.message();
        errorOccurs = true;
        return;
    }

    if (bytes_transferred != curPkt.bodyLength()) {
        LOG(INFO) << "AlgorithmTcpMsgClient::handleReceiveMsgBody error body length incorrect" << error.message();
        return;
    }

    AlgoParamMsg pkt;
    //decodeBody失败会丢弃包
    if (curPkt.decodeBody(pkt)) {
        pushReceiveMsg(pkt);
    }

    boost::asio::async_read(socket, boost::asio::buffer(curPkt.dataHead(), AlgorithmPkt::HEADER_LENGTH),
                            boost::bind(&AlgorithmTcpMsgClient::handleReceiveMsgHeader, this, _1, _2));
}

void AlgorithmTcpMsgClient::checkTimer(boost::system::error_code &ec) {
    if (deadlineTimer.expires_at() <= deadline_timer::traits_type::now()) {
        socket.close();
        deadlineTimer.expires_at(boost::posix_time::pos_infin);
    }
    deadlineTimer.async_wait(boost::bind(&AlgorithmTcpMsgClient::checkTimer, this, ec));
}

AlgorithmTcpMsgClient::~AlgorithmTcpMsgClient() {
    receiveMsgThread.interrupt();
    receiveMsgThread.join();
    doClose();
}

void AlgorithmTcpMsgClient::pushReceiveMsg(const AlgoParamMsg &pkt) {
    boost::unique_lock<boost::mutex> lock(receiveMsgQueueMtx);
    receiveMsgQueue.push_back(pkt);
    receiveMsgQueueCv.notify_one();
}

void AlgorithmTcpMsgClient::popReceiveMsg(AlgoParamMsg &pkt) {
    boost::unique_lock<boost::mutex> lock(receiveMsgQueueMtx);
    while (receiveMsgQueue.empty()) {
        receiveMsgQueueCv.wait(lock);
    }

    pkt = receiveMsgQueue.front();
    receiveMsgQueue.pop_front();
}

bool AlgorithmTcpMsgClient::asyncSendMsg(const AlgoParamMsg &msg) {
    std::vector<uint8_t> sendData;
    if (AlgorithmPkt::encode(sendData, msg)) {
        return asyncSendMsg_(sendData);
    }
    else {
        return false;
    }
}

void AlgorithmTcpMsgClient::receiveMsgLoop() {
    AlgoParamMsg msg;
    while (true) {
        boost::this_thread::interruption_point();
        popReceiveMsg(msg);
        receiveMsgHandler(msg);
    }
}

void AlgorithmTcpMsgClient::doClose() {
    socket.close();
    ioService.stop();
}
