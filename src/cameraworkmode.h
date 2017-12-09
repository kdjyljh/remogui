#ifndef CAMERAWORKMODE_H
#define CAMERAWORKMODE_H
#include "protocaldatainterface.h"
#include "camerafielddef.h"

class CameraWorkMode : public ProtocalDataInterface
{
public:
    CameraWorkMode();
    virtual void handle() override;

    void async_getWorkMode();

protected:
    virtual void workModeGot(const Remo_Camera_WorkMode_S & workmode) {}

private:

};

#endif // CAMERAWORKMODE_H
