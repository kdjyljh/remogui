#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "imagestreamproc.h"
#include "photoandvideodialog.h"
#include "aemodedialog.h"
#include "workmodedialog.h"
#include "receivedataproc.h"
#include "focusdialog.h"
#include "gimbaldialog.h"
#include "deviceinfodialog.h"
#include "viewlable.h"

#include <QMainWindow>
#include <QImage>
#include <QActionGroup>
#include <boost/shared_ptr.hpp>

namespace Ui {
class MainWindow;
}

class ReceiveDataProc;

class MainWindow : public QMainWindow, public ProtocolDataInterfaceImpl
{
    Q_OBJECT

public:
    ImageStreamProc *imagProc;
    ~MainWindow();

    static boost::shared_ptr<MainWindow> getWindInstace();

    void receiveDataDispatch(const std::vector<uint8_t> & data);

    void initAfterConstruct();

    boost::thread imgStreamProcThread;

protected:
    virtual void surportRangeGot(std::set<SubItemData> rangeSet, Remo_CmdId_Camera_e cmdId) override;
    virtual void settingGot(const std::vector<uint8_t> & data, Remo_CmdId_Camera_e cmdId) override;

    virtual void paintEvent(QPaintEvent * ev) override;

private:
    Ui::MainWindow *ui;
    ViewLable *viewLable;
    QLayout *mainLayout;
    boost::shared_ptr<PhotoAndVideoDialog> photoAndVideoDialog;
//    boost::shared_ptr<WorkModeDialog> workModeDialog;
    boost::shared_ptr<AeModeDialog> aeModeDialog;
    boost::shared_ptr<FocusDialog> focusDialog;
    boost::shared_ptr<GimbalDialog> gimbalDialog;
    boost::shared_ptr<DeviceInfoDialog> deviceInfoDialog;

    QActionGroup *actionGroupResolution;
    QActionGroup *actionGroupVideoStandard;
    QActionGroup *actionGroupWhiteBalance;
    QActionGroup *actionGroupExposure;
    QActionGroup *actionGroupISO;
    QActionGroup *actionGroupExposureGear;
    QActionGroup *actionGroupGrid;
    QActionGroup *actionGroupPictureSize;
    QActionGroup *actionGroupQuality;
    QActionGroup *actionGroupCorscatAvoidance;
    QActionGroup *actionGroupSharpening;
    QActionGroup *actionGroupHDR;
    QActionGroup *actionGroupLens;
    QActionGroup *actionGroupPTZSpeed;
    QActionGroup *actionGroupPTZCalibration;
    QActionGroup *actionGroupIntelliMode;
    QActionGroup *actionGroupCloseup;
    QActionGroup *actionGroupScene;
    QActionGroup *actionGroupIntelligLens;

private:

    boost::shared_ptr<ReceiveDataProc> receiveDataProc;

private:
    void setupAction();

    explicit MainWindow(QWidget *parent = 0);
    MainWindow(MainWindow &wind);

    QWidget *customWBWidget;
    QSlider *customWBSlider;

private slots:
    void setLabelPix(const QImage & image);
//    void updatePhotoAndVideoSetting(const PhotoAndVideoSetting & vcSetting);

private slots:
    //auto connect slots
    void on_action_photoAndVideo_triggered();
//    void on_action_continuousAutoFocusDialog_triggered();
//    void on_action_autoRotate_triggered();
//    void on_action_voices_triggered();
//    void on_action_recoveryFactSetting_triggered();
//    void on_action_PTZ_reset_triggered();
//    void on_action_userManage_triggered();
//    void on_action_deviceInfo_triggered();
//    void on_action_storageInfo_triggered();
//    void on_action_intelligence_sportMode_triggered();
//    void on_action_intelligence_zoomedLens_triggered();
    void on_action_exposureCompensation_triggered();
    void on_action_FocusAndZoom_triggered();
    void on_action_Gimbal_triggered();
    void on_action_deviceInfo_triggered();
    void on_action_Aelock_triggered(bool status);

    //slots need connect by code
//    void actionGroup_resolution_triggered(QAction *action);
//    void actionGroup_videoStandard_triggered(QAction *action);
//    void actionGroup_WhiteBalance_triggered(QAction *action);
//    void actionGroup_exposure_triggered(QAction *action);
//    void actionGroup_ISO_triggered(QAction *action);
//    void actionGroup_exposureGear_triggered(QAction *action);
//    void actionGroup_grid_triggered(QAction *action);
//    void actionGroup_pictureSize_triggered(QAction *action);
//    void actionGroup_quality_triggered(QAction *action);
//    void actionGroup_corscatAvoidance_triggered(QAction *action);
//    void actionGroup_sharpening_triggered(QAction *action);
//    void actionGroup_HDR_triggered(QAction *action);
//    void actionGroup_lens_triggered(QAction *action);
//    void actiongroup_PTZspeed_triggered(QAction *action);
//    void actionGroup_PTZCalibration_triggered(QAction *action);
//    void actionGroup_intelliMode_triggered(QAction *action);
//    void actionGroup_closeup_triggered(QAction *action);
//    void actionGroup_scene_triggered(QAction *action);
//    void actionGroup_intelligLens_triggered(QAction *action);

    void menu_action_triggered(QAction *action);
    void customWBSlider_triggered();
};

#endif // MAINWINDOW_H
