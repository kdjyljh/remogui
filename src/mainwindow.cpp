#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Protocol.hpp"
#include "receivedatadispatcher.h"

#include <QPixmap>
#include <QHBoxLayout>
#include <QDesktopWidget>
#include <QtConcurrent/QtConcurrent>
#include <QDebug>
#include <boost/thread.hpp>

const unsigned DEFAULT_WINDOW_WIDTH = 1300;
const unsigned DEFAULT_WINDOW_HEIGHT = 800;

const unsigned IMAGE_RESOLUTION_WIDTH = 1280;
const unsigned IMAGE_RESOLUTION_HEIGHT = 720;

static bool addActionToGroupByMenu(QMenu *menu, QActionGroup *group) {
    if (!menu || !group) {
        return false;
    }

    QList<QAction *> actionList = menu->actions();
    for (int i = 0; i < actionList.size(); ++i) {
        group->addAction(actionList.at(i));
    }

    return true;
}

bool MainWindow::initialized = false;
QPoint MainWindow::centerPoint;
MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ProtocolDataInterfaceImpl(DispatcheType_CameraDefault),
        ui(new Ui::MainWindow),
        mainLayout(new QHBoxLayout) {
    if (isBigEndian()) {
        LOG(INFO) << "System is Big Endian exit !!!!!!!!!!!!!!!!!!!!!!!!";
        QMessageBox::warning(nullptr, "警告", "小端机器无法正常工作", QMessageBox::Ok);
        exit(-1);
    }

    customWBWidget = new QWidget;
    customWBSlider = new QSlider(customWBWidget);

    ui->setupUi(this);
    resize(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
    QRect screenRect = QApplication::desktop()->screenGeometry();
    centerPoint.setX(screenRect.width() / 2 - width() / 2);
    centerPoint.setY(screenRect.height() / 2 - height() / 2);
    move(centerPoint);

//    setFixedSize(1300, 1000);
//    setFixedHeight(DEFAULT_WINDOW_HEIGHT);
}

MainWindow::~MainWindow() {
//    delete ui;
//    delete mainLayout;
//    delete customWBWidget;
//    delete customWBSlider;
}

boost::shared_ptr<MainWindow> MainWindow::getWindInstace() {
    //多线程不安全
    static boost::shared_ptr<MainWindow> instance(new MainWindow);
    static bool showInfo = true;
    if (!instance->init(showInfo, true)) {
        LOG(INFO) << "MainWindow::getWindInstace failed";
        return nullptr;
    }
    showInfo = false;
    instance->registerSelf2Handler();
    return instance;
}

void MainWindow::receiveDataDispatch(const std::vector<uint8_t> &data) {
    unsigned char d = data.at(0);
    qDebug() << "Got data respand and data is:" << d;
}

void MainWindow::initAfterConstruct() {
}

void MainWindow::surportRangeGot(std::set<SubItemData> rangeSet, Remo_CmdId_Camera_e cmdId) {
//    if (!((cmdId >> 3) >= 0x0 && (cmdId >> 3) < 0x60 ||
//         (cmdId >> 3) >= 0x67 && (cmdId >> 3) < 0x78 ||
//         (cmdId >> 3) >= 0x7b && (cmdId >> 3) < 0x85)) {
//        return;
//    }

    LOG(INFO) << "MainWindow::surportRangeGot cmdId = " << std::hex << cmdId;

//    QMenu * menu = static_cast<QMenu*>(findUiPtrById(cmdId));
    QMenu *menu = qobject_cast<QMenu *>(static_cast<QObject *>(findUiPtrById(cmdId)));
//    qDebug() << "find menu" << menu << "sharpness " << ui->menu_Sharpness;
    if (nullptr != menu) {
        for (auto it : menu->actions()) menu->removeAction(it);//先删除menu的子菜单

        connect(menu, SIGNAL(triggered(QAction * )), this, SLOT(menu_action_triggered(QAction * )));

        QActionGroup *group = new QActionGroup(this);
        for (auto it : rangeSet) {
            QAction *action = new QAction(QString::fromUtf8(it.ShowStr.data()), menu);
            action->setData(QVariant(it.Index));//将enum值放入Action的QVariant中
            menu->addAction(action);
            group->addAction(action);
            action->setCheckable(true);
        }

        ItemData itemData;
        if (findItemByUiPtr(menu, itemData)) {
            sendCmdCamera(static_cast<Remo_CmdId_Camera_e>(itemData.CmdId_GetData));
        }
    }
}

void MainWindow::settingGot(const std::vector<uint8_t> &data, Remo_CmdId_Camera_e cmdId) {
//    if (!((cmdId & 0x1ff) >= 0x0 && (cmdId & 0x1ff) < 0x60 ||
//         (cmdId & 0x1ff) >= 0x67 && (cmdId & 0x1ff) < 0x78 ||
//         (cmdId & 0x1ff) >= 0x7b && (cmdId & 0x1ff) < 0x85)) {
//        return;
//    }

    if (data.empty()) return;

    if (cmdId == Remo_CmdId_Camera_Get_AELockStatus) {
        bool status = data[0];
        ui->action_Aelock->setChecked(status);
        return;
    }

    if (cmdId == Remo_CmdId_Camera_Get_CustomWB_ColorTemp) {
        int d = 0;
        memcpy(&d, data.data(), 2);
        int value = d % 100;
        customWBSlider->setValue(value);
        customWBSlider->setEnabled(true);
        return;
    }

    QMenu *group = static_cast<QMenu *>(findUiPtrById(cmdId));
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

void MainWindow::paintEvent(QPaintEvent *ev) {
//    QMainWindow::paintEvent(ev);
//    const QPixmap *ptr = viewLable->pixmap();
//    if (nullptr == ptr) return;
//
//    QPixmap pix(*ptr);
//    pix = pix.scaled(size(), Qt::KeepAspectRatio);
//    viewLable->setGeometry((size().width() - pix.width()) / 2, (size().height() - pix.height()) / 2,
//                           pix.width(), pix.height());
//    viewLable->setPixmap(pix);
}

void MainWindow::setLabelPix(const QImage &image) {
//    QPixmap pix = QPixmap::fromImage(image);
//
//    viewLable->setGeometry((size().width() - pix.width()) / 2, (size().height() - pix.height()) / 2,
//                           pix.width(), pix.height());
//    viewLable->setPixmap(pix);
}

void MainWindow::on_action_photoAndVideo_triggered() {
    if (photoAndVideoDialog) {
        photoAndVideoDialog->show();
    }
}

void MainWindow::on_action_exposureCompensation_triggered() {
    if (aeModeDialog) {
        aeModeDialog->show();
    }
}

void MainWindow::on_action_FocusAndZoom_triggered() {
    if (focusDialog) {
        focusDialog->show();
    }
}

void MainWindow::on_action_Gimbal_triggered() {
    if (gimbalDialog) {
        gimbalDialog->show();
    }
}

void MainWindow::on_action_deviceInfo_triggered() {
    if (deviceInfoDialog) {
        deviceInfoDialog->show();
    }
}

void MainWindow::on_action_Aelock_triggered(bool status) {
    sendCmdCamera(Remo_CmdId_Camera_Set_AELockStatus, std::vector<uint8_t>{status});
}

void MainWindow::on_action_playBack_triggered() {
    if (playBackDialog) {
        playBackDialog->show();
    }
}

void MainWindow::on_action_mediaView_triggered() {
    if (mediaViewWidget) {
        if (mediaViewWidget->reloadImages()) {
            mediaViewWidget->show();
        } else {
            QMessageBox::warning(nullptr, "网络错误", "网络错误", QMessageBox::Ok);
        }
    }
}

void MainWindow::menu_action_triggered(QAction *action) {
    if (nullptr == action) return;

    QMenu *menu = dynamic_cast<QMenu *>(action->parent());
    int data = action->data().toInt();
    if (menu == ui->menu_whiteBalance && data == WhiteBalance_Custom) {
        sendCmdCamera(Remo_CmdId_Camera_Set_WhiteBalance, std::vector<uint8_t>{WhiteBalance_Custom});
        sendCmdCamera(Remo_CmdId_Camera_Get_CustomWB_ColorTemp);
        customWBWidget->show();
        customWBSlider->setEnabled(false);
        return;
    }
    ItemData itemData;
    if (findItemByUiPtr(menu, itemData)) {
        sendCmdCamera(static_cast<Remo_CmdId_Camera_e>(itemData.CmdId_SetData),
                      std::vector<uint8_t>{static_cast<uint8_t>(data)});
    }
}

void MainWindow::customWBSlider_sliderReleased() {
    int data = customWBSlider->value() * 100 + 2000;
    LOG(INFO) << "MainWindow::customWBSlider_sliderReleased value = " << data;
    std::vector<uint8_t> v(reinterpret_cast<uint8_t *>(&data), reinterpret_cast<uint8_t *>(&data) + 2);
    sendCmdCamera(Remo_CmdId_Camera_Set_CustomWB_ColorTemp, v);
}

void MainWindow::showVideoStreamResult(bool result) {
    LOG(INFO) << "MainWindow::showVideoStreamResult result:" << result;
    if (!result) {
        QMessageBox::warning(nullptr, "网络错误", "无视频流", QMessageBox::Ok);
    }
}

void MainWindow::on_action_algorithm_triggered() {
//    auto dialog = AlgorithmDialog::getInstance();
//    if (dialog->init()) {
//        dialog->show();
//    }
//    if (algorithmDialog && algorithmDialog->init()) {
//        algorithmDialog->show();
//    }
}

bool MainWindow::init(bool showInfo, bool initNet) {
    if (initialized) {
//        LOG(INFO) << "MainWindow::init has initialized";
        return true;
    }

    bool ret = false;
    if (initNet) {
        ret = initNetwork(showInfo);
    } else {
        ret = true;
    }

    initialized = ret;
    return ret;
}

bool MainWindow::initNetwork(bool showInfo) {
    //上报IP是否成功
    transmitLocaleIp();
    if (!isValid() && showInfo) {
        LOG(INFO) << "Initialing incorrect no internet !!!!!!!!!!!!!!!!!!!!!!!!";
        if (QMessageBox::Cancel == QMessageBox::warning(nullptr, "网络错误", "网络错误", QMessageBox::Ok | QMessageBox::Cancel)) {
            return false;
        }
    }

//    cameraImageWidget = boost::shared_ptr<CameraImageWidget>(new CameraImageWidget);
    mainWorkSpace = boost::make_shared<MainWorkSpaceWidget>();
    mainWorkSpace->init();

    //    imagProc(new ImageStreamProc),
    receiveDataProc = ReceiveDataProc::getInstance();
    photoAndVideoDialog = boost::shared_ptr<PhotoAndVideoDialog>(new PhotoAndVideoDialog(this));
    aeModeDialog = boost::shared_ptr<AeModeDialog>(new AeModeDialog(this));
    focusDialog = boost::shared_ptr<FocusDialog>(new FocusDialog(this));
    gimbalDialog = boost::shared_ptr<GimbalDialog>(new GimbalDialog(this));
    deviceInfoDialog = DeviceInfoDialog::createInstance(this);
    playBackDialog = boost::shared_ptr<PlayBackDialog>(new PlayBackDialog);
    mediaViewWidget = boost::shared_ptr<WaterFallScrollArea>(new WaterFallScrollArea);
    algorithmProtoTestDialog = boost::shared_ptr<AlgorithmProtoTest>(new AlgorithmProtoTest);

    setCentralWidget(mainWorkSpace.get());
//    setCentralWidget(cameraImageWidget.get());
    customWBWidget->setGeometry(QRect(centerPoint, QSize(400, 50)));
    customWBSlider->setGeometry(QRect(0, 25, 400, 10));
    customWBSlider->setOrientation(Qt::Horizontal);
    customWBSlider->setRange(0, 80);
    customWBWidget->setFixedSize(400, 50);

    photoAndVideoDialog->registerSelf2Handler();
    focusDialog->registerSelf2Handler();
    gimbalDialog->registerSelf2Handler();
    aeModeDialog->registerSelf2Handler();
    mainWorkSpace->registerSelf2Handler();
    algorithmProtoTestDialog->registerSelf2Handler();
//    deviceInfoDialog->registerSelf2Handler();
//    workModeDialog->registerSelf2Handler();

//    connect(imagProc, SIGNAL(imageGot(const QImage&)), this, SLOT(setLabelPix(const QImage&)));
//    connect(photoAndVideoDialog.get(), SIGNAL(getVideoStreamAgain()), imagProc, SLOT(readStream()));
//    connect(imagProc, SIGNAL(readStreamDone(bool)), photoAndVideoDialog.get(), SLOT(readVideoStreamDoneSlot(bool)));
//    connect(imagProc, SIGNAL(readStreamDone(bool)), this, SLOT(showVideoStreamResult(bool)));

//    imgStreamProcThread = boost::thread(&ImageStreamProc::play, imagProc);

    boost::shared_ptr<CameraImageWidget> imageWidget = boost::dynamic_pointer_cast<CameraImageWidget>(mainWorkSpace->getImageWidget());
    if (imageWidget) {
        connect(photoAndVideoDialog.get(), SIGNAL(getVideoStreamAgain()), imageWidget->getDecoder().get(),
                SLOT(readStream()));
        connect(imageWidget->getDecoder().get(), SIGNAL(readStreamDone(bool)), photoAndVideoDialog.get(),
                SLOT(readVideoStreamDoneSlot(bool)));
        connect(imageWidget->getDecoder().get(), SIGNAL(readStreamDone(bool)), this,
                SLOT(showVideoStreamResult(bool)));
        connect(focusDialog.get(), SIGNAL(focusStatusChange(bool)), imageWidget.get(), SLOT(setFocusStatus(bool)));
    }

    receiveDataProc->start();
    ReceiveDataDispatcher::getInstance()->start();

    connect(customWBSlider, SIGNAL(sliderReleased()), this, SLOT(customWBSlider_sliderReleased()));

    //    QActionGroup * whiteBalanceGroup = new QActionGroup(this);
    addItem2Map(ui->menu_CapStorageType, Remo_CmdId_Camera_Get_CapStorageType);
    addItem2Map(ui->menu_CapStorageQuality, Remo_CmdId_Camera_Get_CapStorageQuality);
    addItem2Map(ui->menu_PhotoColorType, Remo_CmdId_Camera_Get_PhotoColorType);
    addItem2Map(ui->menu_VideoMuxerType, Remo_CmdId_Camera_Get_VideoMuxerType);
    addItem2Map(ui->menu_VideoFormat, Remo_CmdId_Camera_Get_VideoFormat);
//    addItem2Map(customWBSlider, Remo_CmdId_Camera_Get_CustomWB_ColorTemp);
    addItem2Map(ui->menu_whiteBalance, Remo_CmdId_Camera_Get_WhiteBalance);
    addItem2Map(ui->menu_Sharpness, Remo_CmdId_Camera_Get_Sharpness);
    addItem2Map(ui->menu_MeterMode, Remo_CmdId_Camera_Get_MeterMode);
    addItem2Map(ui->menu_Antiflick, Remo_CmdId_Camera_Get_Antiflick);
    addItem2Map(ui->menu_Rotation, Remo_CmdId_Camera_Get_Rotation);

//    sendCmdCamera(Remo_CmdId_Camera_Get_CapStorageType_Range);
//    sendCmdCamera(Remo_CmdId_Camera_Get_CapStorageQuality_Range);
//    sendCmdCamera(Remo_CmdId_Camera_Get_PhotoColorType_Range);
//    sendCmdCamera(Remo_CmdId_Camera_Get_VideoMuxerType_Range);
    sendCmdCamera(Remo_CmdId_Camera_Get_WhiteBalance_Range);
    sendCmdCamera(Remo_CmdId_Camera_Get_MeterMode_Range);
    sendCmdCamera(Remo_CmdId_Camera_Get_Antiflick_Range);
//    sendCmdCamera(Remo_CmdId_Camera_Get_Rotation_Range);
    ItemData itemData;
    if (findItemByUiPtr(ui->menu_Sharpness, itemData))
        surportRangeGot(itemData.subItemData, Remo_CmdId_Camera_Get_Sharpness);
    if (findItemByUiPtr(ui->menu_VideoFormat, itemData))
        surportRangeGot(itemData.subItemData, Remo_CmdId_Camera_Get_VideoFormat);

    sendCmdCamera(Remo_CmdId_Camera_Get_AELockStatus);
    sendCmdCamera(Remo_CmdId_Camera_Get_CustomWB_ColorTemp);

//    Remo_Camera_ZoomControlParam_s data ;
//    data.ZoomControlType = 0;
//    data.Speed = SpeedLevelNum_Lowest;
//    data.TargetPosNo = 0;
//    sendCmdCamera(Remo_CmdId_Camera_Set_ZoomControlParam,
//                  std::vector<uint8_t>(reinterpret_cast<uint8_t*>(&data), reinterpret_cast<uint8_t*>(&data) + 5));
    return true;
}

void MainWindow::deInit() {
    cameraImageWidget = nullptr;
    receiveDataProc = nullptr;
    photoAndVideoDialog = nullptr;
    aeModeDialog = nullptr;
    focusDialog = nullptr;
    gimbalDialog = nullptr;
    playBackDialog = nullptr;
    mediaViewWidget = nullptr;
}

void MainWindow::on_action_AlgorithmProtoTest_triggered() {
    if (algorithmProtoTestDialog) {
        algorithmProtoTestDialog->show();
    }
}


