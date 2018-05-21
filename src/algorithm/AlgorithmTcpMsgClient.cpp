//
// Created by jianghualuo on 18-4-8.
//

#include "AlgorithmTcpMsgClient.h"
#include "commlog.h"

#include <glog/logging.h>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/crc.hpp>

AlgorithmTcpMsgClient::AlgorithmTcpMsgClient() :
        socket(ioService),
        errorOccurs(false),
        deadlineTimer(ioService) {
    deadlineTimer.expires_at(boost::posix_time::pos_infin);
    boost::system::error_code ec;
    checkTimer(ec);
    try {
        boost::property_tree::ptree root;
        boost::property_tree::read_json("remo_gui.json", root);
        remoteIp = root.get<std::string>("Algorithm_IP");
        remotePort = root.get<std::string>("Algorithm_Port");
    } catch (boost::property_tree::ptree_error &e) {
        LOG(INFO) << "AlgorithmTcpMsgClient::AlgorithmTcpMsgClient json parse error:" << e.what();
    }
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
    if (runThread.joinable()) {
        ioService.stop();
        runThread.detach(); //自动释放上一个线程
    }
    runThread = boost::thread(boost::bind(&boost::asio::io_service::run, &ioService));

    if (receiveMsgDecodeLoopThread.joinable()) {
        runThread.detach(); //自动释放上一个线程
    }
    receiveMsgDecodeLoopThread = boost::thread(&AlgorithmTcpMsgClient::receiveMsgDecodeLoop, this);

    //读取4个字节包头，为长度
//    boost::asio::async_read(socket, boost::asio::buffer(revMsgBuf.data + revMsgBuf.start, HEADER_LENGTH),
//                            boost::bind(&AlgorithmTcpMsgClient::handleReceiveMsg, this, _1, _2));
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

void AlgorithmTcpMsgClient::handleReceiveMsg(const boost::system::error_code &error, std::size_t bytes_transferred) {
    if (error) {
        LOG(INFO) << "AlgorithmTcpMsgClient::handleReceiveMsg error:" << error.message();
        errorOccurs = true;
        return;
    }

//    if (bytes_transferred != HEADER_LENGTH) {
//        LOG(INFO) << "AlgorithmTcpMsgClient::handleReceiveMsg error body length incorrect" << error.message();
//        return;
//    }
    //加上读取到的长度
    revMsgBuf.end += bytes_transferred;

    int length = 0;
    int curDataLength = 0;


length_handle:
    //大小端处理
    length = revMsgBuf.data[revMsgBuf.start];
    length = length << 8 | revMsgBuf.data[revMsgBuf.start + 1];
    length = length << 8 | revMsgBuf.data[revMsgBuf.start + 2];
    length = length << 8 | revMsgBuf.data[revMsgBuf.start + 3];

    if (length > MAX_PACKET_LENGTH) {//length错误，读取下一个字节
        CHAR_BUFF_TO_LOG(std::vector<char>((char*)&length, ((char*)&length) + 4));
        LOG(INFO) << "AlgorithmPkt::decodeHeader length：" << length << " is greater than MAX_PACKET_LENGTH";
        curDataLength = revMsgBuf.end - revMsgBuf.start;
        if (curDataLength >= 4) {
            ++revMsgBuf.start; //当前缓冲区还有至少一个MAX_PACKET_LENGTH长度
            goto length_handle;
        } else if(curDataLength > 0) { //不够一个MAX_PACKET_LENGTH长度，从网络读取相应个字节
            ++revMsgBuf.start;
            boost::asio::async_read(socket, boost::asio::buffer(revMsgBuf.data + revMsgBuf.start, curDataLength),
                                    boost::bind(&AlgorithmTcpMsgClient::handleReceiveMsg, this, _1, _2));
        } else {
            LOG(INFO) << "AlgorithmPkt::decodeHeader index error start：" << revMsgBuf.start << " end:" << revMsgBuf.end;
            return;
        }
    }
}

//void process_mem_usage(double& vm_usage, double& resident_set)
//{
//    using std::ios_base;
//    using std::ifstream;
//    using std::string;
//
//    vm_usage     = 0.0;
//    resident_set = 0.0;
//
//    // 'file' stat seems to give the most reliable results
//    //
//    ifstream stat_stream("/proc/self/stat",ios_base::in);
//
//    // dummy vars for leading entries in stat that we don't care about
//    //
//    string pid, comm, state, ppid, pgrp, session, tty_nr;
//    string tpgid, flags, minflt, cminflt, majflt, cmajflt;
//    string utime, stime, cutime, cstime, priority, nice;
//    string O, itrealvalue, starttime;
//
//    // the two fields we want
//    //
//    unsigned long vsize;
//    long rss;
//
//    stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
//                >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
//                >> utime >> stime >> cutime >> cstime >> priority >> nice
//                >> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest
//
//    stat_stream.close();
//
//    long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
//    vm_usage     = vsize / 1024.0;
//    resident_set = rss * page_size_kb;
//}

//double vm, rss;
//process_mem_usage(vm, rss);
//LOG(INFO) << "VM: " << vm << "; RSS: " << rss << endl;


void AlgorithmTcpMsgClient::receiveMsgDecodeLoop() {
    boost::system::error_code ec;
    int bytesNeedRead = 0;
    int length = 0;
    int bodyLength = 0;
    bool crcCheckRet = false;
    AlgoParamMsg pkt;
    std::vector<uint8_t> peekBuff;
    boost::asio::streambuf revBuff;
    std::istream revStream(&revBuff);

    boost::asio::read(socket, revBuff, boost::asio::transfer_at_least(HEADER_LENGTH), ec);
    if (ec) goto error;

    while (true) {
retry:
        boost::this_thread::interruption_point();

        bytesNeedRead = 4 - revBuff.size();
        if (bytesNeedRead > 0) {
            boost::asio::read(socket, revBuff,
                              boost::asio::transfer_at_least(bytesNeedRead), ec); //同步读取
            if (ec) goto error;
        }
        //处理length
        peekBuff.clear(); //清除上次的buff
        peekBuff.resize(HEADER_LENGTH);
        boost::asio::buffer_copy(boost::asio::buffer(peekBuff), revBuff.data());
        //大小端处理
        length = peekBuff[0];
        length = (length << 8) | peekBuff[1];
        length = (length << 8) | peekBuff[2];
        length = (length << 8) | peekBuff[3];
        if (length > MAX_PACKET_LENGTH) { //length数据错误
            revBuff.consume(1); //读取下一个字节继续处理length,直到正确为止
            goto retry;
        }
        if (length - HEADER_LENGTH > 4) { //body至少包含4字节类型和4字节crc校验,msg不能为空
            bodyLength = length - HEADER_LENGTH;
        } else {
            LOG(INFO) << "AlgorithmPkt::receiveMsgDecodeLoop length:" << length << " is too short";
            revBuff.consume(1); //否则读取下一个字节继续处理length,直到正确为止
            goto retry;
        }

        //现在length正确
        //处理crc校验
        bytesNeedRead = bodyLength - revBuff.size();
        if (bytesNeedRead > 0) {
            boost::asio::read(socket, revBuff,
                              boost::asio::transfer_at_least(bytesNeedRead), ec);
            if (ec) goto error;
        }

        peekBuff.resize(length);
        boost::asio::buffer_copy(boost::asio::buffer(peekBuff.data(), length), revBuff.data());

        boost::crc_optimal<32, 0x04C11DB7, 0xFFFFFFFF, 0xFFFFFFFF, true, true> result;
        result.process_bytes(peekBuff.data(), peekBuff.size() - 4);
        uint32_t checkSum = result.checksum();
        //大小端处理
        uint8_t buffCS[4];
        memcpy(buffCS, peekBuff.data() + peekBuff.size() - 4, 4);
        uint32_t readCS = buffCS[0];
        readCS = (readCS << 8) | uint32_t(buffCS[1]);
        readCS = (readCS << 8) | uint32_t(buffCS[2]);
        readCS = (readCS << 8) | uint32_t(buffCS[3]);
        //校验crc值
        crcCheckRet = checkSum == readCS;
        if (!crcCheckRet) { //crc校验错误
            LOG(INFO) << "AlgorithmPkt::receiveMsgDecodeLoop crc check failed";
            revBuff.consume(1); //读取下一个字节继续处理length,直到正确为止
            goto retry;
        }

        //现在crc校验完成,数据正确,进行protobuf反序列化
        //不论反序列化正确与否,都应该将当前的msg从sreambuf中拿掉
        try {
            //除去头尾个4字节为msg内容
            if (!pkt.ParseFromArray(peekBuff.data() + 4, peekBuff.size() - 8)) {
                LOG(INFO) << "AlgorithmPkt::receiveMsgDecodeLoop ParseFromArray error";
                goto consume_msg;
            }

            pushReceiveMsg(pkt); //反序列化正确
        } catch (std::exception &e) {
            LOG(INFO) << "AlgorithmPkt::receiveMsgDecodeLoop ParseFromArray error:" << e.what();
            goto consume_msg;
        }
consume_msg:
        revBuff.consume(length);
    }

error:
    LOG(INFO) << "AlgorithmTcpMsgClient::receiveMsgDecodeLoop error:" << ec.message();
    errorOccurs = true;
    return;
}
