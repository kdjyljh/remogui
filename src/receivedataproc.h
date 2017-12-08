#ifndef RECEIVEDATAPROC_H
#define RECEIVEDATAPROC_H

#include "mainwindow.h"
#include <QObject>
#include <vector>
#include <boost/thread.hpp>
#include "../thirdparty/Protocol.hpp"


class ReceiveDataProc : public QObject
{
    Q_OBJECT
public:
    explicit ReceiveDataProc(QObject *parent = nullptr);

    typedef std::vector<uint8_t> ReceivedDataType;
    typedef void (*DataHandler)(const ReceivedDataType & data);

    void registerDataHandler(DataHandler handler) {
        if (nullptr != handler)
            dataHandler = handler;
    }

    void start();

signals:

public slots:

private:
    boost::shared_ptr<boost::thread> thread_;
    DataHandler dataHandler;

    void run();
    void protocolStructProc(const ProtocolStruct & ps);
};

#endif // RECEIVEDATAPROC_H
