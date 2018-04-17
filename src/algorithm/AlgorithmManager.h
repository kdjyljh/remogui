//
// Created by jianghualuo on 18-4-9.
//

#ifndef REMO_GUI_ALGORITHMMANAGER_H
#define REMO_GUI_ALGORITHMMANAGER_H

#include <QObject>
#include <boost/thread/condition_variable.hpp>
#include "AlgorithmTcpMsgClient.h"
#include <boost/function.hpp>
#include <boost/asio/deadline_timer.hpp>

class AlgorithmManager : public QObject {
    Q_OBJECT
public:
    typedef boost::function<void (const AlgoParamMsg &msgSend, void*)> SyncWaitForMsgCallBackFunc;
    AlgorithmManager(QObject *parent = nullptr);
    ~AlgorithmManager();
    bool asyncSendMsg(const AlgoParamMsg &msg);
    //一次只能发一个包，seq_num只能有一个
    //会阻塞
    bool syncSendMsg(const AlgoParamMsg &msgSend, AlgoParamMsg &msgRet, int timeOut = 5000);
    //不会阻塞,立即返回，回包到了会调用callback
    //如果timeout到了，回包还是没到达，则还是会调用callback，
    //callbakce 里面参数void* 为int参数: 0表示SUCCEED，-1为超时，其他为错误代码
    //callbacke函数实现必须freee这个指针
    //每调用一次都会有一个线程建立
    bool syncSendMsg(const AlgoParamMsg &msgSend, SyncWaitForMsgCallBackFunc callback, int timeOut = 5000);
    bool tcpClientConnect();
    //生成单个msg
    static AlgoParamMsg generateMsgByType(AlgoParam::MsgUnity_MsgType type, AlgoParam::Command cmdType = AlgoParam::CmdGet);
    //生成多个msg
    //FIXME
    static AlgoParamMsg generateMsgsByTypes(google::protobuf::RepeatedField<int> typs, google::protobuf::RepeatedField<int> cmds);

signals:
    void msgGot(int type);

public:
    AlgoParamMsg status;
    boost::shared_ptr<AlgorithmTcpMsgClient> tcpMsgClient;

private:
    void receiveMsgHandler(const AlgoParamMsg &pkt);
    void receiveMsgDispatcher(const AlgoParamMsg &pkt);
    static uint64_t msgSeqNum;
    static boost::mutex mtxMsgSeqNum;
    static uint64_t syncWaitForMsgSeqNum;
    static AlgoParamMsg syncWaitForMsg;
    static boost::mutex mtxSyncWaitForMsg;
    static boost::condition_variable cvSyncWaitForMsg;
    static SyncWaitForMsgCallBackFunc syncWaitForMsgCallBack;
    static boost::thread syncWaitForMsgThread;
};


#endif //REMO_GUI_ALGORITHMMANAGER_H
