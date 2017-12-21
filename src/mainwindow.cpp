#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "../thirdparty/shareddata.h"
#include "../thirdparty/Protocol.hpp"

#include <QPixmap>
#include <QHBoxLayout>
#include <QDesktopWidget>
#include <QDebug>
#include <boost/thread.hpp>



const unsigned DEFAULT_WINDOW_WIDTH = 1000;
const unsigned DEFAULT_WINDOW_HEIGHT = 500;
const unsigned IMAGE_RESOLUTION_WIDTH = 1280;
const unsigned IMAGE_RESOLUTION_HEIGHT = 720;

QPoint centerPoint;

static bool addActionToGroupByMenu(QMenu *menu, QActionGroup *group)
{
    if (!menu || !group) {
        return false;
    }

    QList<QAction*> actionList = menu->actions();
    for (int i = 0; i < actionList.size(); ++i) {
        group->addAction(actionList.at(i));
    }

    return true;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ProtocolDataInterfaceImpl(),
    ui(new Ui::MainWindow),
    mainLayout(new QHBoxLayout(this)),
//    viewLable(new ViewLable(this)),
    imagProc(new ImageStreamProc),
//    cameraSetting(new CameraSetting),
    receiveDataProc(ReceiveDataProc::getInstance()),
    photoAndVideoDialog(boost::shared_ptr<PhotoAndVideoDialog>(new PhotoAndVideoDialog(this))),
    aeModeDialog(boost::shared_ptr<AeModeDialog>(new AeModeDialog(this))),
    focusDialog(boost::shared_ptr<FocusDialog>(new FocusDialog(this))),
    gimbalDialog(boost::shared_ptr<GimbalDialog>(new GimbalDialog(this))),
//    workModeDialog(boost::shared_ptr<WorkModeDialog>(new WorkModeDialog)),
    actionGroupResolution(new QActionGroup(this)),
    actionGroupVideoStandard(new QActionGroup(this)),
    actionGroupWhiteBalance(new QActionGroup(this)),
    actionGroupExposure(new QActionGroup(this)),
    actionGroupISO(new QActionGroup(this)),
    actionGroupExposureGear(new QActionGroup(this)),
    actionGroupGrid(new QActionGroup(this)),
    actionGroupPictureSize(new QActionGroup(this)),
    actionGroupQuality(new QActionGroup(this)),
    actionGroupCorscatAvoidance(new QActionGroup(this)),
    actionGroupSharpening(new QActionGroup(this)),
    actionGroupHDR(new QActionGroup(this)),
    actionGroupLens(new QActionGroup(this)),
    actionGroupPTZSpeed(new QActionGroup(this)),
    actionGroupPTZCalibration(new QActionGroup(this)),
    actionGroupIntelliMode(new QActionGroup(this)),
    actionGroupCloseup(new QActionGroup(this)),
    actionGroupScene(new QActionGroup(this)),
    actionGroupIntelligLens(new QActionGroup(this))
{
    if (isBigEndian()) {
        qDebug() << "System is Big Endian exit !!!!!!!!!!!!!!!!!!!!!!!!";
        exit(-1); //
    }

    ui->setupUi(this);

//    QWidget *cw = new QWidget(this);
//    setCentralWidget(cw);
//    cw->setLayout(mainLayout);

//    mainLayout->addWidget(viewLable);
    viewLable = new ViewLable(centralWidget());

//    setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    QRect screenRect = QApplication::desktop()->screenGeometry();
    centerPoint.setX(screenRect.width() / 2 - width() / 2);
    centerPoint.setY(screenRect.height() / 2 - height() / 2);
    move(centerPoint);

    photoAndVideoDialog->registerSelf2Handler();
//    workModeDialog->registerSelf2Handler();

    connect(imagProc, SIGNAL(imageGot(const QImage&)), this, SLOT(setLabelPix(const QImage&)));

    setupAction();

    boost::thread(&ImageStreamProc::play, imagProc);

    receiveDataProc->start();

    QPixmap pix;
    pix.load("/home/jianghua/Pictures/gui.png");
    QSize s(size());
    QSize ss(pix.size());
    pix = pix.scaled(size(), Qt::KeepAspectRatio);
    ss = pix.size();
    viewLable->setGeometry((size().width() - pix.width()) / 2, (size().height() - pix.height()) / 2,
                           pix.width(), pix.height());
    viewLable->setPixmap(pix);

    connect(focusDialog.get(), SIGNAL(focusStatusChange(bool)), viewLable, SLOT(setFocusStatus(bool)));

    //    QActionGroup * whiteBalanceGroup = new QActionGroup(this);
    addItem2Map(ui->menu_CapStorageType, Remo_CmdId_Camera_Get_CapStorageType);
    addItem2Map(ui->menu_CapStorageQuality, Remo_CmdId_Camera_Get_CapStorageQuality);
    addItem2Map(ui->menu_PhotoColorType, Remo_CmdId_Camera_Get_PhotoColorType);
    addItem2Map(ui->menu_VideoMuxerType, Remo_CmdId_Camera_Get_VideoMuxerType);
    addItem2Map(ui->menu_VideoFormat, Remo_CmdId_Camera_Get_VideoFormat);
    addItem2Map(ui->menu_CustomWB_ColorTemp, Remo_CmdId_Camera_Get_CustomWB_ColorTemp);
    addItem2Map(ui->menu_whiteBalance, Remo_CmdId_Camera_Get_WhiteBalance);
    addItem2Map(ui->menu_Sharpness, Remo_CmdId_Camera_Get_Sharpness);
    addItem2Map(ui->menu_MeterMode, Remo_CmdId_Camera_Get_MeterMode);
    addItem2Map(ui->menu_Antiflick, Remo_CmdId_Camera_Get_Antiflick);
    addItem2Map(ui->menu_Rotation, Remo_CmdId_Camera_Get_Rotation);

    sendCmdCamera(Remo_CmdId_Camera_Get_CapStorageType_Range);
    sendCmdCamera(Remo_CmdId_Camera_Get_CapStorageQuality_Range);
    sendCmdCamera(Remo_CmdId_Camera_Get_PhotoColorType_Range);
    sendCmdCamera(Remo_CmdId_Camera_Get_VideoMuxerType_Range);
    sendCmdCamera(Remo_CmdId_Camera_Get_CustomWB_ColorTemp_Range);
    sendCmdCamera(Remo_CmdId_Camera_Get_WhiteBalance_Range);
    sendCmdCamera(Remo_CmdId_Camera_Get_MeterMode_Range);
    sendCmdCamera(Remo_CmdId_Camera_Get_Antiflick_Range);
    sendCmdCamera(Remo_CmdId_Camera_Get_Rotation_Range);
    ItemData itemData;
    if (findItemByUiPtr(ui->menu_Sharpness, itemData))
        surportRangeGot(itemData.subItemData, Remo_CmdId_Camera_Get_Sharpness);
    if (findItemByUiPtr(ui->menu_VideoFormat, itemData))
        surportRangeGot(itemData.subItemData, Remo_CmdId_Camera_Get_VideoFormat);

//    addActionToGroupByMenu(ui->menu_whiteBalance, actionGroupWhiteBalance);
}

MainWindow::~MainWindow()
{
//    delete imagProc;
//    delete viewLable;
//    delete mainLayout;
//    delete imagProc;
//    delete actionGroupResolution;
//    delete cameraSetting;

//    delete ui;
}

boost::shared_ptr<MainWindow> MainWindow::getWindInstace()
{
    //多线程不安全
    static boost::shared_ptr<MainWindow> instance(new MainWindow);
    instance->registerSelf2Handler();
    return instance;
}

void MainWindow::setupAction()
{
//    addActionToGroupByMenu(ui->menu_resolution, actionGroupResolution);
//    connect(this->actionGroupResolution, SIGNAL(triggered(QAction*)), this, SLOT(actionGroup_resolution_triggered(QAction*)));
//    ui->action_resolution_2_7Kp24_2_7Kp24->setData(QVariant("2704,1520,24,2704,1520,24"));
//    ui->action_resolution_2_7Kp30_2_7Kp25->setData(QVariant("2704,1520,30,2704,1520,25"));
//    ui->action_resolution_4Kp24_4Kp24->setData(QVariant("4096,2160,24,4096,2160,24"));
//    ui->action_resolution_4Kp30_4Kp25->setData(QVariant("3840,2160,30,4096,2160,25"));
//    ui->action_resolution_720p24_720p24->setData(QVariant("1080,720,24,1080,720,24"));
//    ui->action_resolution_720p30_720p25->setData(QVariant("1280,720,30,1080,720,25"));
//    ui->action_resolution_720p48_720p48->setData(QVariant("1280,720,48,1080,720,48"));
//    ui->action_resolution_720p60_720p50->setData(QVariant("1280,720,60,1080,720,50"));
//    ui->action_resolution_720p120_720p100->setData(QVariant("1280,720,120,1080,720,100"));
//    ui->action_resolution_720p240_720p200->setData(QVariant("1280,720,240,1080,720,200"));
//    ui->action_resolution_1080p24_1080p24->setData(QVariant("1920,1080,24,1920,1080,24"));
//    ui->action_resolution_1080p30_1080p25->setData(QVariant("1920,1080,30,1920,1080,25"));
//    ui->action_resolution_1080p48_1080p48->setData(QVariant("1920,1080,48,1920,1080,48"));
//    ui->action_resolution_1080p60_1080p50->setData(QVariant("1920,1080,60,1920,1080,50"));
//    ui->action_resolution_1080p120_1080p100->setData(QVariant("1920,1080,120,1920,1080,100"));

//    addActionToGroupByMenu(ui->menu_videoStandard, actionGroupVideoStandard);
//    connect(this->actionGroupVideoStandard, SIGNAL(triggered(QAction*)), this, SLOT(actionGroup_videoStandard_triggered(QAction*)));

//    addActionToGroupByMenu(ui->menu_whiteBalance, actionGroupWhiteBalance);
//    connect(this->actionGroupWhiteBalance, SIGNAL(triggered(QAction*)), this, SLOT(actionGroup_WhiteBalance_triggered(QAction*)));

//    addActionToGroupByMenu(ui->menu_exposure, actionGroupExposure);
//    connect(actionGroupExposure, SIGNAL(triggered(QAction*)), this, SLOT(actionGroup_exposure_triggered(QAction*)));

//    addActionToGroupByMenu(ui->menu_ISO, actionGroupISO);
//    connect(this->actionGroupISO, SIGNAL(triggered(QAction*)), this, SLOT(actionGroup_ISO_triggered(QAction*)));

//    addActionToGroupByMenu(ui->menu_exposureGear, actionGroupExposureGear);
//    connect(actionGroupExposureGear, SIGNAL(triggered(QAction*)), this, SLOT(actionGroup_exposureGear_triggered(QAction*)));

//    addActionToGroupByMenu(ui->menu_grid, actionGroupGrid);
//    connect(actionGroupGrid, SIGNAL(triggered(QAction*)), this, SLOT(actionGroup_grid_triggered(QAction*)));

//    addActionToGroupByMenu(ui->menu_pictureSize, actionGroupPictureSize);
//    connect(actionGroupPictureSize, SIGNAL(triggered(QAction*)), this, SLOT(actionGroup_pictureSize_triggered(QAction*)));

//    addActionToGroupByMenu(ui->menu_quality, actionGroupQuality);
//    connect(actionGroupQuality, SIGNAL(triggered(QAction*)), this, SLOT(actionGroup_quality_triggered(QAction*)));

//    addActionToGroupByMenu(ui->menu_corscatAvoidance, actionGroupCorscatAvoidance);
//    connect(actionGroupCorscatAvoidance, SIGNAL(triggered(QAction*)), this, SLOT(actionGroup_corscatAvoidance_triggered(QAction*)));

//    addActionToGroupByMenu(ui->menu_sharpening, actionGroupSharpening);
//    connect(actionGroupSharpening, SIGNAL(triggered(QAction*)), this, SLOT(actionGroup_sharpening_triggered(QAction*)));

//    addActionToGroupByMenu(ui->menu_HDR, actionGroupHDR);
//    connect(actionGroupHDR, SIGNAL(triggered(QAction*)), this, SLOT(actionGroup_HDR_triggered(QAction*)));

//    addActionToGroupByMenu(ui->menu_lens, actionGroupLens);
//    connect(actionGroupLens, SIGNAL(triggered(QAction*)), this, SLOT(actionGroup_lens_triggered(QAction*)));

//    addActionToGroupByMenu(ui->menu_PTZSpeed, actionGroupPTZSpeed);
//    connect(actionGroupPTZSpeed, SIGNAL(triggered(QAction*)), this, SLOT(actiongroup_PTZspeed_triggered(QAction*)));

//    addActionToGroupByMenu(ui->menu_PTZCalibration, actionGroupPTZCalibration);
//    connect(actionGroupPTZCalibration, SIGNAL(triggered(QAction*)), this, SLOT(actionGroup_PTZCalibration_triggered(QAction*)));

//    addActionToGroupByMenu(ui->menu_intelliMode, actionGroupIntelliMode);
//    connect(actionGroupIntelliMode, SIGNAL(triggered(QAction*)), this, SLOT(actionGroup_intelliMode_triggered(QAction*)));

//    addActionToGroupByMenu(ui->menu_closeup, actionGroupCloseup);
//    connect(actionGroupCloseup, SIGNAL(triggered(QAction*)), this, SLOT(actionGroup_closeup_triggered(QAction*)));

//    addActionToGroupByMenu(ui->menu_scene, actionGroupScene);
//    connect(actionGroupScene, SIGNAL(triggered(QAction*)), this, SLOT(actionGroup_scene_triggered(QAction*)));

//    addActionToGroupByMenu(ui->menu_intelligLens, actionGroupIntelligLens);
//    connect(actionGroupIntelligLens, SIGNAL(triggered(QAction*)), this, SLOT(actionGroup_intelligLens_triggered(QAction*)));
}

void MainWindow::receiveDataDispatch(const std::vector<uint8_t> &data)
{
    unsigned char d = data.at(0);
    qDebug() << "Got data respand and data is:" << d;

}

void MainWindow::initAfterConstruct()
{
}

void MainWindow::surportRangeGot(std::set<SubItemData> rangeSet, Remo_CmdId_Camera_e cmdId)
{
    if (!((cmdId >> 3) >= 0x0 && (cmdId >> 3) < 0x60 ||
         (cmdId >> 3) >= 0x67 && (cmdId >> 3) < 0x78 ||
         (cmdId >> 3) >= 0x7b && (cmdId >> 3) < 0x85)) {
        return;
    }

    qDebug() << "MainWindow::surportRangeGot cmdId = " << cmdId;

    QMenu * menu = static_cast<QMenu*>(findUiPtrById(cmdId));
    qDebug() << "find menu" << menu << "sharpness " << ui->menu_Sharpness;
    if (nullptr != menu) {
        for (auto it : menu->actions()) menu->removeAction(it);//先删除menu的子菜单

        QActionGroup *group = new QActionGroup(this);
        for (auto it : rangeSet) {
            QAction * action = new QAction(QString::fromUtf8(it.ShowStr.data()), this);
            action->setData(QVariant(it.Index));//将enum值放入Action的QVariant中
            menu->addAction(action);
            group->addAction(action);
            action->setCheckable(true);
            connect(menu, SIGNAL(triggered(QAction*)), this, SLOT(menu_action_triggered(QAction*)));
        }

        ItemData itemData;
        if (findItemByUiPtr(menu, itemData)) {
            sendCmdCamera(static_cast<Remo_CmdId_Camera_e>(itemData.CmdId_GetData));
        }
    }
}

void MainWindow::settingGot(const std::vector<uint8_t> &data, Remo_CmdId_Camera_e cmdId)
{
    if (!((cmdId & 0x1ff) >= 0x0 && (cmdId & 0x1ff) < 0x60 ||
         (cmdId & 0x1ff) >= 0x67 && (cmdId & 0x1ff) < 0x78 ||
         (cmdId & 0x1ff) >= 0x7b && (cmdId & 0x1ff) < 0x85)) {
        return;
    }

    if (data.empty()) return;

    QMenu * group = static_cast<QMenu*>(findUiPtrById(cmdId));
    if (nullptr != group) {
        ItemData itemData;
        int activeIndex = static_cast<int>(data.at(0));
        if (findItemByUiPtr(group, itemData)) {
            for (auto it : group->actions()) {
                if (activeIndex == it->data().toInt()) {
                    it->setChecked(true);
                }
            }
        }
    }
}

void MainWindow::paintEvent(QPaintEvent *ev)
{
    QMainWindow::paintEvent(ev);
    const QPixmap *ptr = viewLable->pixmap();
    if (nullptr == ptr) return;

    QPixmap pix(*ptr);
    pix = pix.scaled(size(), Qt::KeepAspectRatio);
    viewLable->setGeometry((size().width() - pix.width()) / 2, (size().height() - pix.height()) / 2,
                           pix.width(), pix.height());
    viewLable->setPixmap(pix);
}

void MainWindow::setLabelPix(const QImage &image)
{
//    QPixmap pix = QPixmap::fromImage(image);
//    QPixmap pix;
//    pix.load("/home/jianghua/Pictures/gui.png");
//    pix.scaledToWidth(IMAGE_RESOLUTION_WIDTH);
//    pix.scaledToHeight(IMAGE_RESOLUTION_HEIGHT);
//    viewLable->setGeometry((size().width() - IMAGE_RESOLUTION_WIDTH) / 2, (size().height() - IMAGE_RESOLUTION_HEIGHT) / 2,
//                           IMAGE_RESOLUTION_WIDTH, IMAGE_RESOLUTION_HEIGHT);

    QPixmap pix;
    pix.load("/home/jianghua/Pictures/gui.png");
    QSize s(size());
    QSize ss(pix.size());
    pix = pix.scaled(size(), Qt::KeepAspectRatio);
    ss = pix.size();
    viewLable->setGeometry((size().width() - pix.width()) / 2, (size().height() - pix.height()) / 2,
                           pix.width(), pix.height());
    viewLable->setPixmap(pix);
}

void MainWindow::on_action_photoAndVideo_triggered()
{
    photoAndVideoDialog->show();
//    workModeDialog->show();
}

//void MainWindow::on_action_continuousAutFocusDialogs_triggered()
//{
//        qDebug() << "continuus Atuo Foces";
//}

//void MainWindow::on_action_autoRotate_triggered()
//{

//}

//void MainWindow::on_action_voices_triggered()
//{

//}

//void MainWindow::on_action_recoveryFactSetting_triggered()
//{

//}

//void MainWindow::on_action_PTZ_reset_triggered()
//{

//}

//void MainWindow::on_action_userManage_triggered()
//{

//}

//void MainWindow::on_action_deviceInfo_triggered()
//{

//}

//void MainWindow::on_action_storageInfo_triggered()
//{

//}

//void MainWindow::on_action_intelligence_sportMode_triggered()
//{

//}

//void MainWindow::on_action_intelligence_zoomedLens_triggered()
//{

//}

//void MainWindow::on_action_exposureCompensation_triggered()
//{
////    exposureCompensationSpinBox->setRange(-3,3);
////    QRect screenRect = QApplication::desktop()->screenGeometry();
////    exposureCompensationSpinBox->move(screenRect.width() / 2, screenRect.height() / 2);
////    exposureCompensationSpinBox->setFixedSize(100, 20);
////    exposureCompensationSpinBox->show();
//    aeModeDialog->show();
//}

//void MainWindow::actionGroup_resolution_triggered(QAction *action)
//{
//    if (nullptr == action) {
//        return;
//    }

////    Resolution resolution;
//    if (ui->action_videoStandard_PAL->isChecked()) {
//        if (action == ui->action_resolution_2_7Kp24_2_7Kp24) {
////            resolution.NTSCSize.setWidth(2704);
////            resolution.NTSCSize.setHeight(1520);
////            resolution.NTSCFps = 24;
////            resolution.PALSize.setWidth(2704);
////            resolution.PALSize.setHeight(1520);
////            resolution.PALFps = 24;
//        }
//        else if (action == ui->action_resolution_2_7Kp30_2_7Kp25) {
////            resolution.NTSCSize.setWidth(2704);
////            resolution.NTSCSize.setHeight(1520);
////            resolution.NTSCFps = 30;
////            resolution.PALSize.setWidth(2704);
////            resolution.PALSize.setHeight(1520);
////            resolution.PALFps = 25;
//        }
//        else if (action == ui->action_resolution_4Kp24_4Kp24) {
////            resolution.NTSCSize.setWidth(4096);
////            resolution.NTSCSize.setHeight(2160);
////            resolution.NTSCFps = 24;
////            resolution.PALSize.setWidth(4096);
////            resolution.PALSize.setHeight(2160);
////            resolution.PALFps = 24;
//        }
//        else if (action == ui->action_resolution_4Kp30_4Kp25) {

//        }
//        else if (action == ui->action_resolution_720p48_720p48) {

//        }
//        else if (action == ui->action_resolution_720p24_720p24) {

//        }
//        else if (action == ui->action_resolution_720p30_720p25) {

//        }
//        else if (action == ui->action_resolution_720p60_720p50) {

//        }
//        else if (action == ui->action_resolution_720p120_720p100) {

//        }
//        else if (action == ui->action_resolution_720p240_720p200) {

//        }
//        else if (action == ui->action_resolution_1080p24_1080p24) {

//        }
//        else if (action == ui->action_resolution_1080p30_1080p25) {

//        }
//        else if (action == ui->action_resolution_1080p48_1080p48) {

//        }
//        else if (action == ui->action_resolution_1080p60_1080p50) {

//        }
//        else if (action == ui->action_resolution_1080p120_1080p100) {

//        }
//        else {

//        }
//    }
//    else if (ui->action_videoStandard_NTSC->isChecked()) {
//        if (action == ui->action_resolution_2_7Kp24_2_7Kp24) {
////            resolution.NTSCSize.setWidth(2704);
////            resolution.NTSCSize.setHeight(1520);
////            resolution.NTSCFps = 24;
////            resolution.PALSize.setWidth(2704);
////            resolution.PALSize.setHeight(1520);
////            resolution.PALFps = 24;
//        }
//        else if (action == ui->action_resolution_2_7Kp30_2_7Kp25) {
////            resolution.NTSCSize.setWidth(2704);
////            resolution.NTSCSize.setHeight(1520);
////            resolution.NTSCFps = 30;
////            resolution.PALSize.setWidth(2704);
////            resolution.PALSize.setHeight(1520);
////            resolution.PALFps = 25;
//        }
//        else if (action == ui->action_resolution_4Kp24_4Kp24) {
////            resolution.NTSCSize.setWidth(4096);
////            resolution.NTSCSize.setHeight(2160);
////            resolution.NTSCFps = 24;
////            resolution.PALSize.setWidth(4096);
////            resolution.PALSize.setHeight(2160);
////            resolution.PALFps = 24;
//        }
//        else if (action == ui->action_resolution_4Kp30_4Kp25) {

//        }
//        else if (action == ui->action_resolution_720p48_720p48) {

//        }
//        else if (action == ui->action_resolution_720p24_720p24) {

//        }
//        else if (action == ui->action_resolution_720p30_720p25) {

//        }
//        else if (action == ui->action_resolution_720p60_720p50) {

//        }
//        else if (action == ui->action_resolution_720p120_720p100) {

//        }
//        else if (action == ui->action_resolution_720p240_720p200) {

//        }
//        else if (action == ui->action_resolution_1080p24_1080p24) {

//        }
//        else if (action == ui->action_resolution_1080p30_1080p25) {

//        }
//        else if (action == ui->action_resolution_1080p48_1080p48) {

//        }
//        else if (action == ui->action_resolution_1080p60_1080p50) {

//        }
//        else if (action == ui->action_resolution_1080p120_1080p100) {

//        }
//        else {

//        }
//    }
//}

//void MainWindow::actionGroup_videoStandard_triggered(QAction *action)
//{
//    if (nullptr == action) {
////        return NTSC;
//    }
//    else if (ui->action_videoStandard_NTSC == action) {
////        return NTSC;
//    }
//    else if (ui->action_videoStandard_PAL == action) {
////        return PAL;
//    }
//    else {
////        return NTSC;
//    }
//}

//void MainWindow::actionGroup_WhiteBalance_triggered(QAction *action)
//{
//    if (nullptr == action) {
////        return whiteBalance_auto;
//    }
//    else if (ui->action_whiteBalance_daylight == action) {
////        return whiteBalance_daylight;
//    }
//    else if (ui->action_whiteBalance_dusk == action) {
////        return whiteBalance_dusk;
//    }
//    else if (ui->action_whiteBalance_fluoresctLamp == action) {
////        return whiteBalance_fluoresctLamp;
//    }
//    else if (ui->action_whiteBalance_incandesctLamp == action) {
////        return whiteBalance_incandesctLamp;
//    }
//    else if (ui->action_whiteBalance_overcast == action) {
////        return whiteBalance_overcast;
//    }
//    else if (ui->action_whiteBalance_shadow == action) {
////        return whiteBalance_shadow;
//    }
//    else {
////        return whiteBalance_auto;
//    }
//}

//void MainWindow::actionGroup_exposure_triggered(QAction *action)
//{
//    if (nullptr == action) {

//    }
//    else if (ui->action_exposure_globalMetering == action) {

//    }
//    else if (ui->action_exposure_spotMetering == action) {

//    }
//    else {

//    }
//}

//void MainWindow::actionGroup_ISO_triggered(QAction *action)
//{
//    if (nullptr == action) {

//    }
//    else if (ui->action_ISO_100_400 == action) {
//    }
//    else if (ui->action_ISO_200_800 == action) {

//    }
//    else if (ui->action_ISO_400_1200 == action) {

//    }
//    else if (ui->action_ISO_800_1600 == action) {

//    }
//    else if (ui->action_ISO_1600_3200 == action) {

//    }
//    else if (ui->action_ISO_auto == action) {

//    }
//    else {

//    }
//}

//void MainWindow::actionGroup_exposureGear_triggered(QAction *action)
//{
//    if (nullptr == action) {

//    }
//    else if (ui->action_exposureGear_M == action) {

//    }
//    else if (ui->action_exposureGear_P == action) {

//    }
//    else {

//    }
//}

//void MainWindow::actionGroup_grid_triggered(QAction *action)
//{
////    if (nullptr == action) {

////    }
////    else if (ui->action_grid_centerPoint == action) {

////    }
////    else if (ui->action_grid_grid == action) {

////    }
////    else if (ui->action_grid_gFocusDialogdFocus == action) {

////    }
////    else if (ui->action_grid_NONE == action) {

////    }
////    else {

////    }
//}

//void MainWindow::actionGroup_pictureSize_triggered(QAction *action)
//{
//    if (nullptr == action) {

//    }
//    else if (ui->action_pictureSize_fullScreen == action) {

//    }
//    else if (ui->action_pictureSize_standard == action) {

//    }
//    else {

//    }
//}

//void MainWindow::actionGroup_quality_triggered(QAction *action)
//{
//    if (nullptr == action) {

//    }
//    else if (ui->action_quality_high == action) {

//    }
//    else if (ui->action_quality_low == action) {

//    }
//    else if (ui->action_quality_standard == action) {

//    }
//    else {

//    }
//}

//void MainWindow::actionGroup_corscatAvoidance_triggered(QAction *action)
//{
//    if (nullptr == action) {

//    }
//    else if (ui->action_corscatAvoidance_50Hz == action) {

//    }
//    else if (ui->action_corscatAvoidance_60Hz == action) {

//    }
//    else if (ui->action_corscatAvoidance_auto == action) {

//    }
//    else if (ui->action_corscatAvoidance_off == action) {

//    }
//    else {

//    }
//}

//void MainWindow::actionGroup_sharpening_triggered(QAction *action)
//{
//    if (nullptr == action) {

//    }
//    else if (ui->action_sharpening_high == action) {

//    }
//    else if (ui->action_sharpening_higher == action) {

//    }
//    else if (ui->action_sharpening_low == action) {

//    }
//    else if (ui->action_sharpening_lower == action) {

//    }
//    else if (ui->action_sharpening_lowest == action) {

//    }
//    else if (ui->action_sharpening_standard == action) {

//    }
//    else if (ui->action_sharpening_highest == action) {

//    }
//    else {

//    }
//}

//void MainWindow::actionGroup_HDR_triggered(QAction *action)
//{
//    if (nullptr == action) {

//    }
//    else if (ui->action_HDR_auto == action) {

//    }
//    else if (ui->action_HDR_off == action) {

//    }
//    else if (ui->action_HDR_on == action) {

//    }
//    else {

//    }
//}

//void MainWindow::actionGroup_lens_triggered(QAction *action)
//{
//    if (nullptr == action) {

//    }
//    else if (ui->action_lens_hanging == action) {

//    }
//    else if (ui->action_lens_horizontal == action) {

//    }
//    else if (ui->action_lens_standerd == action) {

//    }
//    else if (ui->action_lens_vertical == action) {

//    }
//    else {

//    }
//}

//void MainWindow::actiongroup_PTZspeed_triggered(QAction *action)
//{
//    if (nullptr == action) {

//    }
//    else if (ui->action_PTZSpeed_slow == action) {

//    }
//    else if (ui->action_PTZSpeed_fast == action) {

//    }
//    else if (ui->action_PTZSpeed_intermediary == action) {

//    }
//    else {

//    }
//}

//void MainWindow::actionGroup_PTZCalibration_triggered(QAction *action)
//{
//    if (nullptr == action) {

//    }
//    else if (ui->action_PTZCalibration_auto == action) {

//    }
//    else if (ui->action_PTZCalibration_manual == action) {

//    }
//    else {

//    }
//}

//void MainWindow::actionGroup_intelliMode_triggered(QAction *action)
//{
//    if (nullptr == action) {

//    }
//    else if (ui->action_intelliMode_doublePerson == action) {

//    }
//    else if (ui->action_intelliMode_MultiPerson == action) {

//    }
//    else if (ui->action_intelliMode_universal == action) {

//    }
//    else {

//    }
//}

//void MainWindow::actionGroup_closeup_triggered(QAction *action)
//{
//    if (nullptr == action) {

//    }
//    else if (ui->action_closeup_3 == action) {

//    }
//    else if (ui->action_closeup_5 == action) {

//    }
//    else if (ui->action_closeup_7 == action) {

//    }
//    else if (ui->action_closeup_wholeBody == action) {

//    }
//    else {

//    }
//}

//void MainWindow::actionGroup_scene_triggered(QAction *action)
//{
//    if (nullptr == action) {

//    }
//    else if (ui->action_scene_activeShoot == action) {

//    }
//    else if (ui->action_scene_intelliSnap == action) {

//    }
//    else {

//    }
//}

//void MainWindow::actionGroup_intelligLens_triggered(QAction *action)
//{
//    if (nullptr == action) {

//    }
//    else if (ui->action_intelligLens_otherLens == action) {

//    }
//    else if (ui->action_intelligLens_scanningLens == action) {

//    }
//    else {

//    }
//}

void MainWindow::on_action_FocusAndZoom_triggered()
{
    focusDialog->show();
}

void MainWindow::on_action_Gimbal_triggered()
{
    gimbalDialog->show();
}

void MainWindow::menu_action_triggered(QAction *action)
{
    if (nullptr == action) return;

    QMenu *menu = action->menu();
    ItemData itemData;
    if (findItemByUiPtr(menu, itemData)) {
        std::vector<uint8_t> data{action->data().toInt()};
        sendCmdCamera(static_cast<Remo_CmdId_Camera_e>(itemData.CmdId_SetData), data);
    }
}


