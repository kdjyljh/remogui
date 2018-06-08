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
#include "playbackdialog.h"
#include "mediaview/waterfallscrollarea.h"
#include "CameraImageWidget.h"
#include "MainWorkSpaceWidget.h"
#include "AlgorithmProtoTest.h"

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
//    ImageStreamProc *imagProc;
    ~MainWindow();

    static boost::shared_ptr<MainWindow> getWindInstace();

    void receiveDataDispatch(const std::vector<uint8_t> & data);

    void initAfterConstruct();

//    boost::thread imgStreamProcThread;

public:
    static QPoint centerPoint;

protected:
    virtual void surportRangeGot(std::set<SubItemData> rangeSet, Remo_CmdId_Camera_e cmdId) override;
    virtual void settingGot(const std::vector<uint8_t> & data, Remo_CmdId_Camera_e cmdId) override;

    virtual void paintEvent(QPaintEvent * ev) override;

private:
    Ui::MainWindow *ui;
//    ViewLable *viewLable;
    QLayout *mainLayout;
    boost::shared_ptr<PhotoAndVideoDialog> photoAndVideoDialog;
//    boost::shared_ptr<WorkModeDialog> workModeDialog;
    boost::shared_ptr<AeModeDialog> aeModeDialog;
    boost::shared_ptr<FocusDialog> focusDialog;
    boost::shared_ptr<GimbalDialog> gimbalDialog;
    boost::shared_ptr<DeviceInfoDialog> deviceInfoDialog;
    boost::shared_ptr<PlayBackDialog> playBackDialog;
    boost::shared_ptr<WaterFallScrollArea> mediaViewWidget;
    boost::shared_ptr<CameraImageWidget> cameraImageWidget;
    boost::shared_ptr<MainWorkSpaceWidget> mainWorkSpace;
    boost::shared_ptr<AlgorithmProtoTest> algorithmProtoTestDialog;
    QWidget *customWBWidget;
    QSlider *customWBSlider;
    static bool initialized;

private:
    boost::shared_ptr<ReceiveDataProc> receiveDataProc;

private:
    void setupAction();
    explicit MainWindow(QWidget *parent = 0);
    MainWindow(MainWindow &wind);
    bool init(bool showInfo = false, bool initNet = true);
    void deInit();
    bool initNetwork(bool showInfo = false);

private slots:
    void setLabelPix(const QImage & image);
    void showVideoStreamResult(bool result);

private slots:
    void on_action_photoAndVideo_triggered();
    void on_action_exposureCompensation_triggered();
    void on_action_FocusAndZoom_triggered();
    void on_action_Gimbal_triggered();
    void on_action_deviceInfo_triggered();
    void on_action_Aelock_triggered(bool status);
    void on_action_playBack_triggered();
    void on_action_mediaView_triggered();
    void on_action_algorithm_triggered();
    void on_action_AlgorithmProtoTest_triggered();
    void menu_action_triggered(QAction *action);
    void customWBSlider_sliderReleased();
};

#endif // MAINWINDOW_H
