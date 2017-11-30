#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPixmap>
#include <QHBoxLayout>
#include <QDesktopWidget>
#include <QtConcurrent/QtConcurrent>
#include <QDebug>


const unsigned DEFAULT_WINDOW_WIDTH = 1000;
const unsigned DEFAULT_WINDOW_HEIGHT = 500;

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
    ui(new Ui::MainWindow),
    mainLayout(new QHBoxLayout(this)),
    viewLable(new QLabel(this)),
    imagProc(new ImageStreamProc),
    cameraSetting(new CameraSetting),
    photoAndVideoDialog(new PhotoAndVideoDialog(this)),
    actionGroupResolution(new QActionGroup(this)),
    actionGroupVideoStandard(new QActionGroup(this)),
    actionGroupWhiteBalance(new QActionGroup(this))
{
    ui->setupUi(this);

    QWidget *cw = new QWidget(this);
    setCentralWidget(cw);
    cw->setLayout(mainLayout);
    mainLayout->addWidget(viewLable);

//    setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    QRect screenRect = QApplication::desktop()->screenGeometry();
    move(screenRect.width() / 2 - width() / 2, screenRect.height() / 2 - height() / 2);

//    imagProc->init();
//    QtConcurrent::run(imagProc, &ImageStreamProc::play);

    connect(imagProc, SIGNAL(imageGot(const QImage&)), this, SLOT(setLabelPix(const QImage&)));

    connect(photoAndVideoDialog, SIGNAL(cameraSettingChanged(const PhotoAndVideoSetting&)), this, SLOT(updatePhotoAndVideoSetting(const PhotoAndVideoSetting &)));

    setupAction();
}

MainWindow::~MainWindow()
{
    imagProc->thread()->terminate();
    delete imagProc;
    delete viewLable;
    delete mainLayout;
    delete imagProc;
    delete actionGroupResolution;
    delete cameraSetting;

    delete ui;
}

void MainWindow::setupAction()
{
    addActionToGroupByMenu(ui->menu_resolution, actionGroupResolution);
    connect(this->actionGroupResolution, SIGNAL(triggered(QAction*)), this, SLOT(actionGroup_resolution_triggered(QAction*)));
    ui->action_resolution_2_7Kp24_2_7Kp24->setData(QVariant("2704,1520,24,2704,1520,24"));
    ui->action_resolution_2_7Kp30_2_7Kp25->setData(QVariant("2704,1520,30,2704,1520,25"));
    ui->action_resolution_4Kp24_4Kp24->setData(QVariant("4096,2160,24,4096,2160,24"));
    ui->action_resolution_4Kp30_4Kp25->setData(QVariant("3840,2160,30,4096,2160,25"));
    ui->action_resolution_720p24_720p24->setData(QVariant("1080,720,24,1080,720,24"));
    ui->action_resolution_720p30_720p25->setData(QVariant("1280,720,30,1080,720,25"));
    ui->action_resolution_720p48_720p48->setData(QVariant("1280,720,48,1080,720,48"));
    ui->action_resolution_720p60_720p50->setData(QVariant("1280,720,60,1080,720,50"));
    ui->action_resolution_720p120_720p100->setData(QVariant("1280,720,120,1080,720,100"));
    ui->action_resolution_720p240_720p200->setData(QVariant("1280,720,240,1080,720,200"));
    ui->action_resolution_1080p24_1080p24->setData(QVariant("1920,1080,24,1920,1080,24"));
    ui->action_resolution_1080p30_1080p25->setData(QVariant("1920,1080,30,1920,1080,25"));
    ui->action_resolution_1080p48_1080p48->setData(QVariant("1920,1080,48,1920,1080,48"));
    ui->action_resolution_1080p60_1080p50->setData(QVariant("1920,1080,60,1920,1080,50"));
    ui->action_resolution_1080p120_1080p100->setData(QVariant("1920,1080,120,1920,1080,100"));

    addActionToGroupByMenu(ui->menu_videoStandard, actionGroupVideoStandard);
    connect(this->actionGroupVideoStandard, SIGNAL(triggered(QAction*)), this, SLOT(actionGroup_videoStandard_triggered(QAction*)));

    addActionToGroupByMenu(ui->menu_whiteBalance, actionGroupWhiteBalance);
    connect(this->actionGroupWhiteBalance, SIGNAL(triggered(QAction*)), this, SLOT(actionGroup_WhiteBalance_triggered(QAction*)));
}

void MainWindow::setLabelPix(const QImage &image)
{
    QPixmap pix = QPixmap::fromImage(image);
    viewLable->setPixmap(pix);
}

void MainWindow::updatePhotoAndVideoSetting(const PhotoAndVideoSetting & pvSetting)
{
    qDebug() << "get camera setting changed signal cs.photoContinusTime:" << pvSetting.photoContinusTime;
    cameraSetting->photoAndVideoSetting = pvSetting;
}

void MainWindow::on_action_photoAndVideo_triggered()
{
    photoAndVideoDialog->show();
}

void MainWindow::actionGroup_resolution_triggered(QAction *action)
{
    if (nullptr == action) {
        return;
    }

    Resolution resolution;
    if (ui->action_videoStandard_PAL->isChecked()) {
        if (action == ui->action_resolution_2_7Kp24_2_7Kp24) {
            resolution.NTSCSize.setWidth(2704);
            resolution.NTSCSize.setHeight(1520);
            resolution.NTSCFps = 24;
            resolution.PALSize.setWidth(2704);
            resolution.PALSize.setHeight(1520);
            resolution.PALFps = 24;
        }
        else if (action == ui->action_resolution_2_7Kp30_2_7Kp25) {
            resolution.NTSCSize.setWidth(2704);
            resolution.NTSCSize.setHeight(1520);
            resolution.NTSCFps = 30;
            resolution.PALSize.setWidth(2704);
            resolution.PALSize.setHeight(1520);
            resolution.PALFps = 25;
        }
        else if (action == ui->action_resolution_4Kp24_4Kp24) {
            resolution.NTSCSize.setWidth(4096);
            resolution.NTSCSize.setHeight(2160);
            resolution.NTSCFps = 24;
            resolution.PALSize.setWidth(4096);
            resolution.PALSize.setHeight(2160);
            resolution.PALFps = 24;
        }
    }
    else if (ui->action_videoStandard_NTSC->isChecked()) {

    }
}

void MainWindow::actionGroup_videoStandard_triggered(QAction *action)
{
    if (nullptr == action) {
//        return NTSC;
    }
    else if (ui->action_videoStandard_NTSC == action) {
//        return NTSC;
    }
    else if (ui->action_videoStandard_PAL == action) {
//        return PAL;
    }
    else {
//        return NTSC;
    }
}

void MainWindow::actionGroup_WhiteBalance_triggered(QAction *action)
{
    if (nullptr == action) {
//        return whiteBalance_auto;
    }
    else if (ui->action_whiteBalance_daylight == action) {
//        return whiteBalance_daylight;
    }
    else if (ui->action_whiteBalance_dusk == action) {
//        return whiteBalance_dusk;
    }
    else if (ui->action_whiteBalance_fluoresctLamp == action) {
//        return whiteBalance_fluoresctLamp;
    }
    else if (ui->action_whiteBalance_incandesctLamp == action) {
//        return whiteBalance_incandesctLamp;
    }
    else if (ui->action_whiteBalance_overcast == action) {
//        return whiteBalance_overcast;
    }
    else if (ui->action_whiteBalance_shadow == action) {
//        return whiteBalance_shadow;
    }
    else {
//        return whiteBalance_auto;
    }
}

void MainWindow::actionGroup_exposure_triggered(QAction *action)
{

}

