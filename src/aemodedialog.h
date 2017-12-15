#ifndef AEMODE_H
#define AEMODE_H
#include <QDialog>

#include "ui_aemode.h"
#include "protocoldataInterfaceimpl.h"

class AeModeDialog : public QDialog, public ProtocolDataInterfaceImpl
{
public:
    explicit AeModeDialog(QWidget * parent);

protected:
    void cameraSettingGot(const std::vector<uint8_t> & data, Remo_CmdId_e cmdId) override;
    void surportRangeGot(std::set<SubItemData> rangeSet, Remo_CmdId_e cmdId) override;

private:
    Ui::AeMOde * ui;

    void syncUiModeByAeMode(Remo_Camera_AeMode_e aeMode);

private slots:
    void combox_activated(int index);
};

#endif // AEMODE_H