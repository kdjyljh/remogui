//
// Created by jianghualuo on 18-4-9.
//

#include "AlgorithmManager.h"
#include <boost/make_shared.hpp>
#include <glog/logging.h>
#include <boost/chrono.hpp>

uint64_t AlgorithmManager::msgSeqNum = 0;
boost::mutex AlgorithmManager::mtxMsgSeqNum;
uint64_t AlgorithmManager::syncWaitForMsgSeqNum = -1; //初始化为最大uint64值
boost::mutex AlgorithmManager::mtxSyncWaitForMsg;
boost::condition_variable AlgorithmManager::cvSyncWaitForMsg;
AlgoParamMsg AlgorithmManager::syncWaitForMsg;
AlgorithmManager::SyncWaitForMsgCallBackFunc AlgorithmManager::syncWaitForMsgCallBack;
boost::thread AlgorithmManager::syncWaitForMsgThread;
AlgorithmManager::AlgorithmManager(QObject *parent) :
        tcpMsgClient(boost::make_shared<AlgorithmTcpMsgClient>()),
        QObject(parent) {
    tcpMsgClient->registerReceiveMsgHandler(boost::bind(&AlgorithmManager::receiveMsgHandler, this, _1));
}

AlgorithmManager::~AlgorithmManager() {

}

bool AlgorithmManager::asyncSendMsg(const AlgoParamMsg &msg) {
    LOG(INFO) << "AlgorithmManager::asyncSendMsg send msg type:" << msg.type(0) << " id:" << msg.seq_num(0);
    return tcpMsgClient->asyncSendMsg(msg);
}

void AlgorithmManager::receiveMsgHandler(const AlgoParamMsg &msg) {
//    std::string strType;
//    for (int i = 0; i < msg.type().size(); ++i) {
//        strType += " ";
//        strType += msg.type(i);
//    }
//    LOG(INFO) << "AlgorithmManager::receiveMsgHandler called pkt.type:" << strType;
    receiveMsgDispatcher(msg);
}

void AlgorithmManager::receiveMsgDispatcher(const AlgoParamMsg &msg) {
    if (msg.error_code() != AlgoParam::SUCCEED) {
        //丢弃包
        LOG(INFO) << "AlgorithmManager::receiveMsgDispatcher error_code:" << msg.error_code();
        return;
    }

    status.set_contain_allget(msg.contain_allget());
    for (int i = 0; i < msg.type().size(); ++i) {
        int type = msg.type(i);
        LOG(INFO) << "AlgorithmManager::receiveMsgDispatcher use async handler type:" << type << " id:" << msg.seq_num(i);
        if (msg.seq_num_size() > 0) {
            if (msg.seq_num_size() != msg.type_size()) {
                //丢弃包
                LOG(INFO) << "AlgorithmManager::receiveMsgDispatcher has seq num but is not equ to type size";
                continue;
            }

            if (syncWaitForMsgCallBack) {
                //使用同步非阻塞接口
                int *ec = new int(0);
                syncWaitForMsg = msg;
                syncWaitForMsg.clear_type();
                syncWaitForMsg.add_type(static_cast<AlgoParam::MsgUnity_MsgType>(type));
                syncWaitForMsgCallBack(syncWaitForMsg, ec);
                syncWaitForMsgCallBack.clear(); //还原
                LOG(INFO) << "AlgorithmManager::receiveMsgDispatcher use noblock function type = " << type;
            } else {
                //使用同步阻塞接口
                boost::unique_lock<boost::mutex> lock(mtxSyncWaitForMsg);
                if (msg.seq_num(i) == syncWaitForMsgSeqNum) {
                    //为同步消息
                    syncWaitForMsg = msg;
                    syncWaitForMsg.clear_type();
                    syncWaitForMsg.add_type(static_cast<AlgoParam::MsgUnity_MsgType>(type));
                    cvSyncWaitForMsg.notify_one();
                    LOG(INFO) << "AlgorithmManager::receiveMsgDispatcher use block function type = " << type;
                }
            }
            continue;
        }

        //使用异步接口
        if (type == AlgoParam::MsgUnity::SelectionSet) {
            (*status.mutable_selection_set()) = msg.selection_set();//深拷贝
        } else if (type == AlgoParam::MsgUnity::ControlSet) {
            (*status.mutable_control_set()) = msg.control_set();
        } else if (type == AlgoParam::MsgUnity::ZoomMode) {
            (*status.mutable_zoom_mode()) = msg.zoom_mode();
        } else if (type == AlgoParam::MsgUnity::VersionGet) {
            (*status.mutable_version_get()) = msg.version_get();
        } else if (type == AlgoParam::MsgUnity::FaceTemplLibGet) {
            (*status.mutable_face_templlib_get()) = msg.face_templlib_get();
        } else if (type == AlgoParam::MsgUnity::SpecialShot) {
            (*status.mutable_special_shot()) = msg.special_shot();
        }  else {
        }

        msgGot(type);
        LOG(INFO) << "AlgorithmManager::receiveMsgDispatcher use async handler type = " << type;
    }
}

AlgoParamMsg AlgorithmManager::generateMsgByType(AlgoParam::MsgUnity_MsgType type, AlgoParam::Command cmdType) {
    AlgoParamMsg msg;
    if (AlgoParam::MsgUnity::SelectionSet == type) {
        msg.mutable_selection_set();
    } else if (AlgoParam::MsgUnity::ControlSet == type) {
        msg.mutable_control_set();
    } else if (AlgoParam::MsgUnity::FaceTemplLibGet == type) {
        msg.mutable_face_templlib_get();
    } else if (AlgoParam::MsgUnity::FaceTemplLibSet == type) {
        msg.mutable_face_templlib_set();
    } else if (AlgoParam::MsgUnity::FaceTemplLibModify == type) {
        msg.mutable_face_templlib_modify();
    }  else if (AlgoParam::MsgUnity::ZoomMode == type) {
        msg.mutable_zoom_mode();
    }  else if (AlgoParam::MsgUnity::SpecialShot == type) {
        msg.mutable_special_shot();
    } else {
        LOG(INFO) << "AlgorithmManager::generateMsgByType no such type:" << type;
    }

    {
        boost::unique_lock<boost::mutex> lock(mtxMsgSeqNum);
        msg.add_seq_num(msgSeqNum);//服务端不校验seq_num合法性，重复，负数等等情况，所以需要客户端保证不会出错
        ++msgSeqNum;
    }

    msg.add_type(type);
    return msg;
}

AlgoParamMsg AlgorithmManager::generateMsgsByTypes(google::protobuf::RepeatedField<int> typs,
                                                   google::protobuf::RepeatedField<int> cmds) {
    return AlgoParamMsg();
}

bool AlgorithmManager::tcpClientConnect() {
    return tcpMsgClient->connect();
}

bool AlgorithmManager::syncSendMsg(const AlgoParamMsg &msgSend, AlgoParamMsg &msgRet, int timeOut) {
    bool ret = asyncSendMsg(msgSend);
    if (!ret) {
        return ret;
    }

    {
        boost::unique_lock<boost::mutex> lock(mtxSyncWaitForMsg);
        syncWaitForMsgSeqNum = msgSend.seq_num(0);
        ret = boost::cv_status::no_timeout ==
                cvSyncWaitForMsg.wait_for(lock, boost::chrono::milliseconds(timeOut));
        syncWaitForMsgSeqNum = -1; //恢复成最大uint64值
        if (ret) {
            msgRet = syncWaitForMsg;
        }
    }

    if (!ret) {
        LOG(INFO) << "AlgorithmManager::syncSendMsg timeout";
    }
    return ret;
}

bool AlgorithmManager::syncSendMsg(const AlgoParamMsg &msgSend, SyncWaitForMsgCallBackFunc callback, int timeOut) {
    if (callback) {
        return false;
    }
    bool ret = asyncSendMsg(msgSend);
    if (!ret) {
        return false;
    }

    syncWaitForMsgCallBack = callback;
    syncWaitForMsgThread = boost::thread([&](){
        boost::this_thread::sleep_for(boost::chrono::milliseconds(timeOut));
        if (syncWaitForMsgCallBack) {
            int *ec = new int(-1);
            syncWaitForMsgCallBack(AlgoParamMsg(), ec);
            syncWaitForMsgCallBack.clear();
        }
    });
    return true;
}
