#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "imagestreamproc.h"
#include "photoandvideodialog.h"
#include "camerasetting.h"
#include "receivedataproc.h"

#include <QMainWindow>
#include <QLabel>
#include <QImage>
#include <QActionGroup>
#include <QSpinBox>

namespace Ui {
class MainWindow;
}

class ReceiveDataProc;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    ~MainWindow();

    static MainWindow * getWindInstace();

    void receiveDataDistribute(const std::vector<uint8_t> & data);

private:
    Ui::MainWindow *ui;
    QLabel *viewLable;
    QLayout *mainLayout;
    PhotoAndVideoDialog *photoAndVideoDialog;
    QSpinBox *exposureCompensationSpinBox;

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
    ImageStreamProc *imagProc;
    CameraSetting *cameraSetting;
    ReceiveDataProc *receiveDataProc;

private:
    void setupAction();

    explicit MainWindow(QWidget *parent = 0);
    MainWindow(MainWindow &wind);

private slots:
    void setLabelPix(const QImage & image);
    void updatePhotoAndVideoSetting(const PhotoAndVideoSetting & vcSetting);

private slots:
    //auto connect slots
    void on_action_photoAndVideo_triggered();
    void on_action_continuousAutoFocus_triggered();
    void on_action_autoRotate_triggered();
    void on_action_voices_triggered();
    void on_action_recoveryFactSetting_triggered();
    void on_action_PTZ_reset_triggered();
    void on_action_userManage_triggered();
    void on_action_deviceInfo_triggered();
    void on_action_storageInfo_triggered();
    void on_action_intelligence_sportMode_triggered();
    void on_action_intelligence_zoomedLens_triggered();
    void on_action_exposureCompensation_triggered();

    //slots need connect by code
    void actionGroup_resolution_triggered(QAction *action);
    void actionGroup_videoStandard_triggered(QAction *action);
    void actionGroup_WhiteBalance_triggered(QAction *action);
    void actionGroup_exposure_triggered(QAction *action);
    void actionGroup_ISO_triggered(QAction *action);
    void actionGroup_exposureGear_triggered(QAction *action);
    void actionGroup_grid_triggered(QAction *action);
    void actionGroup_pictureSize_triggered(QAction *action);
    void actionGroup_quality_triggered(QAction *action);
    void actionGroup_corscatAvoidance_triggered(QAction *action);
    void actionGroup_sharpening_triggered(QAction *action);
    void actionGroup_HDR_triggered(QAction *action);
    void actionGroup_lens_triggered(QAction *action);
    void actiongroup_PTZspeed_triggered(QAction *action);
    void actionGroup_PTZCalibration_triggered(QAction *action);
    void actionGroup_intelliMode_triggered(QAction *action);
    void actionGroup_closeup_triggered(QAction *action);
    void actionGroup_scene_triggered(QAction *action);
    void actionGroup_intelligLens_triggered(QAction *action);
};

#endif // MAINWINDOW_H
