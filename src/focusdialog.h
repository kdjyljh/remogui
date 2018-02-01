#ifndef FOCUS_H
#define FOCUS_H
#include "protocoldataInterfaceimpl.h"
#include "ui_focus.h"
#include <QDialog>

class FocusDialog : public QDialog, public ProtocolDataInterfaceImpl
{
Q_OBJECT
public:
    explicit FocusDialog(QWidget * parent);

    void closeEvent(QCloseEvent *event) override;

signals:
    void focusStatusChange(bool);

protected:
    virtual void surportRangeGot(std::set<SubItemData> rangeSet, Remo_CmdId_Camera_e cmdId) override;
    virtual void settingGot(const std::vector<uint8_t> & data, Remo_CmdId_Camera_e cmdId) override;

private:
    Ui::Focus * ui;

private slots:
    void on_ComboBox_AFMode_activated(int index);
    void on_ComboBox_ZoomSpeed_activated(int index);
    void on_horizontalSlider_FocalLengthPosNo_sliderReleased();
    void on_pushButton_StopZoom_clicked();
    void on_pushButton_StartFocus_clicked();
};

#endif // FOCUS_H
