#ifndef RECEIVEDATADISPATCHER_H
#define RECEIVEDATADISPATCHER_H
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <QObject>

#include "protocaldatainterface.h"
#include "receivedatahandler.h"

class ReceiveDataDispatcher : public boost::noncopyable, public QObject
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

    void run();

signals:
    void dataGot(ParsedData data);

private slots:
    void dataDispatcher(ParsedData data);

private:
    ReceiveDataDispatcher();

private:
    //must be ptr, using virtual funciton handle
    std::vector<boost::shared_ptr<ProtocolDataInterface>> handlerPtrList;
};

#endif // RECEIVEDATADISPATCHER_H
