#ifndef CMDSENDER_H
#define CMDSENDER_H
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include "../thirdparty/Protocol.hpp"
#include "camerafielddef.h"

typedef void (*workModeHandler)(Remo_Camera_WorkMode_s & workMode);

class CmdSender : public boost::noncopyable
{
public:
    static boost::shared_ptr<CmdSender> getInstance();
    void async_getWorkMode(workModeHandler handler);
private:
    CmdSender();
};

#endif // CMDSENDER_H
