#ifndef RECEIVEDATAPROC_H
#define RECEIVEDATAPROC_H

#include "mainwindow.h"
#include "receivedatahandler.h"
#include "../thirdparty/Protocol.hpp"
#include <vector>
#include <boost/thread.hpp>
#include <boost/noncopyable.hpp>


class ReceiveDataProc : public boost::noncopyable
{

public:
    static boost::shared_ptr<ReceiveDataProc> getInstance();
    
    void registerHandler(boost::shared_ptr<ReceiveDataHandler> handler) {
        handler_ = handler;
    }

    void start();

signals:

public slots:

private:
    boost::shared_ptr<boost::thread> thread_;
    boost::shared_ptr<ReceiveDataHandler> handler_;
    ReceiveDataProc();

    void run();
    void protocolStructProc(const ProtocolStruct & ps);
};

#endif // RECEIVEDATAPROC_H
