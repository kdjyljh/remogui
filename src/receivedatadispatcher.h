#ifndef RECEIVEDATADISPATCHER_H
#define RECEIVEDATADISPATCHER_H
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <QObject>
#include <QVariant>

#include "protocaldatainterface.h"
#include "receivedatahandler.h"

class ReceiveDataDispatcher : public QObject, public boost::noncopyable, public boost::enable_shared_from_this<ReceiveDataDispatcher>
{
Q_OBJECT
public:
    static boost::shared_ptr<ReceiveDataDispatcher> getInstance();

    void registerDataHandler(boost::shared_ptr<ProtocolDataInterface> handler) {
        for (auto it : handlerPtrList) {
            if (it == handler) return;
        }
        handlerPtrList.push_back(handler);
    }

    void start();

signals:
    void dataGot(QVariant content);

private slots:
    void dataDispatcher(QVariant content);

private:
    ReceiveDataDispatcher();
    DispatcheType mapToDispatcher(Remo_CmdSet_e cmdSet, int cmdId);
    void run();

private:
    //must be ptr, using virtual funciton handle
    std::vector<boost::shared_ptr<ProtocolDataInterface>> handlerPtrList;
};

#endif // RECEIVEDATADISPATCHER_H
