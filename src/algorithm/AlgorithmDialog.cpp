//
// Created by jianghualuo on 18-3-20.
//

#include "AlgorithmDialog.h"
#include <glog/logging.h>
#include <QDebug>
#include <QMessageBox>
#include <QAction>
#include <QPushButton>
#include <QCheckBox>
#include <QGroupBox>
#include <QTabBar>
#include <boost/lexical_cast.hpp>

AlgorithmDialog::AlgorithmDialog(QWidget *parent) :
        QDialog(parent),
        initialized(false),
        isSpecialShotting(false),
        manager(boost::make_shared<AlgorithmManager>()),
        faceTemplateDialog(boost::make_shared<FaceTemplateDialog>()),
        gimbalDialog(boost::make_shared<AlgorithmGimbalDialog>()),
        zoomSlider(boost::make_shared<AlgorithmZoomSlider>()),
        player(boost::make_shared<QMediaPlayer>()),
        ui(new Ui::Algorithm) {
}

AlgorithmDialog::~AlgorithmDialog() {

}

void AlgorithmDialog::resizeEvent(QResizeEvent *event) {
}

bool AlgorithmDialog::asyncSendMsg(const AlgoParamMsg &msg) {
    if (!manager->asyncSendMsg(msg)) {
        QMessageBox::warning(nullptr, "警告", "网络错误", QMessageBox::Ok);
        return false;
    }
    return true;
}

void AlgorithmDialog::onMsgGot(int msgType) {
    if (msgType == AlgoParam::MsgUnity::SelectionSet) {
        updateSelectionUi();
    } else if (msgType == AlgoParam::MsgUnity::ControlSet) {
        updateControl();
    }  else if (msgType == AlgoParam::MsgUnity::VersionGet) {
        updateVersionUi();
    } else if (msgType == AlgoParam::MsgUnity::FaceTemplLibGet) {
        updateFaceTemplateUi();
    } else if (msgType == AlgoParam::MsgUnity::ZoomMode) {
        updateZoomModeUi();
    }  else if (msgType == AlgoParam::MsgUnity::SpecialShot) {
        updateSpecialShotUi();
    } else {
        LOG(INFO) << "AlgorithmDialog::onMsgGot unknown msg type";
    }
}

void AlgorithmDialog::updateSelectionUi() {
    LOG(INFO) << "AlgorithmDialog::updateSelectionUi error:"
              << manager->status.mutable_selection_set()->error_code()
              << " mode:" << manager->status.mutable_selection_set()->select_mode()
              << " is_selection:" << manager->status.mutable_selection_set()->is_selecting();
    //如果有错误消息，则输出到界面
    if (manager->status.mutable_selection_set()->has_error_code()) {
        QString str;
        if (manager->status.mutable_selection_set()->error_code() == AlgoParam::ERR_WRONG_POINT) {
            str = QString::fromLocal8Bit("点错误");
        } else if (manager->status.mutable_selection_set()->error_code() == AlgoParam::ERR_WRONG_BBOX) {
            str = QString::fromLocal8Bit("框错误");
        } else if (manager->status.mutable_selection_set()->error_code() == AlgoParam::SUCCEED) {
            str = QString::fromLocal8Bit("");
        } else {
            str = QString::fromLocal8Bit("未知错误");
        }
        ui->lineEdit_error_info->setText(str);
    }

    //设置当前的选人模式
    AlgoParam::SelectionSet::SelectMode mode = manager->status.mutable_selection_set()->select_mode();
    QRadioButton *curRadioButton = nullptr;
    if (AlgoParam::SelectionSet::SELECTION_USERPICK == mode) {
        curRadioButton = nullptr;
    } else if (AlgoParam::SelectionSet::SELECTION_AUTO == mode) {
        curRadioButton = ui->radioButton_person_chose_auto;
    } else if (AlgoParam::SelectionSet::SELECTION_MASTER == mode) {
        curRadioButton = ui->radioButton_person_chose_master;
        ui->lineEdit_master_speed->setText(QString::number(manager->status.mutable_selection_set()->master_speed()));
        ui->lineEdit_master_time->setText(QString::number(manager->status.mutable_selection_set()->master_time()));
    } else if (AlgoParam::SelectionSet::SELECTION_MULTIPLE == mode) {
        curRadioButton = ui->radioButton_person_chose_multiple;
    }

    static QRadioButton *lastRadioButton = nullptr;
    //当前选人模式为非user_pick
    if (curRadioButton) {
        if (lastRadioButton) lastRadioButton->setAutoExclusive(false);
        curRadioButton->setChecked(true);
        if (lastRadioButton) lastRadioButton->setAutoExclusive(true);

        if (manager->status.mutable_selection_set()->is_selecting()) {
            //如果正在选人，则设置为红色
            if (lastRadioButton) {
                lastRadioButton->setStyleSheet("background-color:;");
            }
            curRadioButton->setStyleSheet("background-color:red;");
        } else {
            //选人结束恢复颜色
            curRadioButton->setStyleSheet("background-color:;");
        }
    } else {
        //当前选人模式为user_pick,将上次的选人状态结束，设置为正常颜色
        if (lastRadioButton) {
            lastRadioButton->setAutoExclusive(false);
            lastRadioButton->setChecked(false);
            lastRadioButton->setAutoExclusive(true);
            lastRadioButton->setStyleSheet("background-color:;");
        }
    }
    lastRadioButton = curRadioButton;

    //设置当前人脸模板
    for (int i = 0; i < manager->status.mutable_selection_set()->face_templ().size(); ++i) {
        QPixmap pix;
        pix.loadFromData((uchar*)manager->status.mutable_selection_set()->face_templ(i).face_image().data(),
                         manager->status.mutable_selection_set()->face_templ(i).face_image().size());
        AlgorithmImageWidget imageWidget;
        imageWidget.setPix(pix);
        ui->widget_chose_person_image->layout()->addWidget(&imageWidget);
    }
}

void AlgorithmDialog::checkBox_capture_stateChanged(int state) {
//    AlgoParam::Capture::CaptureType type;
//    QCheckBox *checkBox = dynamic_cast<QCheckBox *>(sender());
//    if (checkBox == ui->checkBox_capture_action) {
//        type = AlgoParam::Capture::CAPTURE_ACTION;
//    } else if (checkBox == ui->checkBox_capture_static) {
//        type = AlgoParam::Capture::CAPTURE_STATIC;
//    } else if (checkBox == ui->checkBox_capture_smile) {
//        type = AlgoParam::Capture::CAPTURE_SMILE;
//    }
//
//    bool enabled = false;
//    if (state == Qt::Checked) {
//        enabled = true;
//    }
//
//    AlgoParamMsg msg;
//    msg.set_type(0, AlgoParam::MsgUnity::Capture);
//    AlgoParam::Capture *capture = msg.mutable_capture();
//    capture->set_enabled(0, enabled);
}

void AlgorithmDialog::updateCaptureUi() {
//    for (int i = 0; i < manager->status.msgCapture->capture_type().size(); ++i) {
//        int type = manager->status.msgCapture->capture_type().data()[i];
//        int enable = manager->status.msgCapture->enabled().data()[i];
//        if (type == AlgoParam::Capture::CAPTURE_SMILE) {
//            ui->checkBox_capture_smile->setChecked(enable);
//        } else if (type == AlgoParam::Capture::CAPTURE_STATIC) {
//            ui->checkBox_capture_static->setChecked(enable);
//        } else if (type == AlgoParam::Capture::CAPTURE_ACTION) {
//            ui->checkBox_capture_action->setChecked(enable);
//        }
//    }
}

void AlgorithmDialog::on_groupBox_person_chose_clicked(bool checked) {
    ui->workSpaceWidget->setManuallyChosingPerson(checked);


    //当处于正在选人状态时才需要结束选人
    if (!checked && manager->status.selection_set().is_selecting()) {
        AlgoParamMsg msg =  AlgorithmManager::generateMsgByType(AlgoParam::MsgUnity::SelectionSet);
        msg.mutable_selection_set()->set_is_selecting(false);
        //user_pick需要用户点击确认结束。
        if (manager->status.selection_set().select_mode() == AlgoParam::SelectionSet::SELECTION_USERPICK) {
            msg.mutable_selection_set()->set_select_mode(AlgoParam::SelectionSet::SELECTION_USERPICK);
        } else {
            //剩下的mutil，master和auto选人会自动结束
            // 但是用户在还未结束时点击结束选人,则会取消当前选人并且进入auto选人
            msg.mutable_selection_set()->set_select_mode(AlgoParam::SelectionSet::SELECTION_AUTO);
            msg.mutable_selection_set()->set_is_selecting(true);
        }
        asyncSendMsg(msg);
    }
}

void AlgorithmDialog::radioButton_person_chose_clicked(bool checked) {
    QRadioButton *ptr = dynamic_cast<QRadioButton*>(sender());
    if (ptr) {
        AlgoParam::SelectionSet::SelectMode mode;
        AlgoParamMsg msg =  AlgorithmManager::generateMsgByType(AlgoParam::MsgUnity::SelectionSet);
        if (ptr == ui->radioButton_person_chose_multiple) {
            mode = AlgoParam::SelectionSet::SELECTION_MULTIPLE;
            double masterTime = 0.0;
            double masterSpeed = 0.0;
            try {
                masterTime = boost::lexical_cast<double>(ui->lineEdit_master_time->text().toStdString());
                masterSpeed = boost::lexical_cast<double>(ui->lineEdit_master_speed->text().toStdString());
            } catch (std::exception &e) {
            }
            msg.mutable_selection_set()->set_master_speed(masterSpeed);
            msg.mutable_selection_set()->set_master_time(masterTime);
        } else if (ptr == ui->radioButton_person_chose_auto) {
            mode = AlgoParam::SelectionSet::SELECTION_AUTO;
        } else if (ptr == ui->radioButton_person_chose_master) {
            mode = AlgoParam::SelectionSet::SELECTION_MASTER;
        }

        msg.mutable_selection_set()->set_select_mode(mode);
        msg.mutable_selection_set()->set_is_selecting(true);
        asyncSendMsg(msg);
    }
}

void AlgorithmDialog::updateControl() {
}

bool AlgorithmDialog::init() {
    if (initialized) {
        LOG(INFO) << "AlgorithmDialog::init has initialized";
        return initialized;
    }

    //检查网络是否通畅
    if (!manager->tcpClientConnect()) {
        QMessageBox::warning(nullptr, "警告", "网络错误", QMessageBox::Ok);
        return false;
    }

    //检查是否初始化
    int checkTimes = 1000;
    bool algorithmInitialized = false;
    while (!algorithmInitialized && checkTimes--) {
        algorithmInitialized = manager->status.contain_allget();
        boost::this_thread::sleep_for(boost::chrono::milliseconds(10));
    }
    if (!algorithmInitialized) {
        QMessageBox::warning(nullptr, "警告", "算法未初始化", QMessageBox::Ok);
        return false;
    }

    ui->setupUi(this);
    resize(1300, 900);
    ui->tabWidget->tabBar()->setMinimumWidth(800);

    connect(manager.get(), SIGNAL(msgGot(int)), this, SLOT(onMsgGot(int)));

    connect(ui->workSpaceWidget, SIGNAL(chosingPersonMouseReleased(double, double)), this, SLOT(sendManuallyChosing(double, double)));

    connect(ui->radioButton_person_chose_auto, SIGNAL(clicked(bool)), this, SLOT(radioButton_person_chose_clicked(bool)));
    connect(ui->radioButton_person_chose_master, SIGNAL(clicked(bool)), this, SLOT(radioButton_person_chose_clicked(bool)));
    connect(ui->radioButton_person_chose_multiple, SIGNAL(clicked(bool)), this, SLOT(radioButton_person_chose_clicked(bool)));

    connect(ui->checkBox_capture_smile, SIGNAL(stateChanged(int)), SLOT(checkBox_capture_stateChanged(int)));
    connect(ui->checkBox_capture_action, SIGNAL(stateChanged(int)), SLOT(checkBox_capture_stateChanged(int)));
    connect(ui->checkBox_capture_static, SIGNAL(stateChanged(int)), SLOT(checkBox_capture_stateChanged(int)));

    connect(ui->pushButton_zoom_auto, SIGNAL(clicked()), this, SLOT(pushButton_zoom_clicked()));
    connect(ui->pushButton_zoom_none, SIGNAL(clicked()), this, SLOT(pushButton_zoom_clicked()));
    connect(ui->pushButton_zoom_gap, SIGNAL(clicked()), this, SLOT(pushButton_zoom_clicked()));
    connect(ui->pushButton_zoom_cycle, SIGNAL(clicked()), this, SLOT(pushButton_zoom_clicked()));

    connect(ui->pushButton_slowzoom, SIGNAL(clicked()), this, SLOT(pushButton_specialShot_clicked()));
    connect(ui->pushButton_onceZoom, SIGNAL(clicked()), this, SLOT(pushButton_specialShot_clicked()));
    connect(ui->pushButton_cycleZoom, SIGNAL(clicked()), this, SLOT(pushButton_specialShot_clicked()));
    connect(ui->pushButton_roolSwing, SIGNAL(clicked()), this, SLOT(pushButton_specialShot_clicked()));
    connect(ui->pushButton_scan, SIGNAL(clicked()), this, SLOT(pushButton_specialShot_clicked()));
    connect(ui->pushButton_shot, SIGNAL(clicked()), this, SLOT(pushButton_specialShot_clicked()));

    QDoubleValidator *validator = new QDoubleValidator(0, 100000, 5, this);

    ui->lineEdit_master_time->setValidator(validator);
    ui->lineEdit_master_speed->setValidator(validator);

    ui->lineEdit_auto_scale->setValidator(validator);
    ui->lineEdit_auto_zmspd->setValidator(validator);
    ui->lineEdit_gap_lowerbound->setValidator(validator);
    ui->lineEdit_gap_upperbound->setValidator(validator);
    ui->lineEdit_gap_zmspd->setValidator(validator);
    ui->lineEdit_cycle_lowerbound->setValidator(validator);
    ui->lineEdit_cycle_upperbound->setValidator(validator);
    ui->lineEdit_cycle_zmspd->setValidator(validator);

    ui->lineEdit_slowzoom_time->setValidator(validator);
    ui->lineEdit_slowzoom_zmspd->setValidator(validator);
    ui->lineEdit_oncezoom_scale->setValidator(validator);
    ui->lineEdit_oncezoom_zmspd->setValidator(validator);
    ui->lineEdit_scan_scale->setValidator(validator);
    ui->lineEdit_scan_speed->setValidator(validator);
    ui->lineEdit_scan_time->setValidator(validator);
    ui->lineEdit_scan_time_shot->setValidator(validator);
    ui->lineEdit_scan_zmspd->setValidator(validator);
    ui->lineEdit_shot_focus_upper->setValidator(validator);
    ui->lineEdit_shot_scale->setValidator(validator);
    ui->lineEdit_shot_time->setValidator(validator);
    ui->lineEdit_shot_zmspd->setValidator(validator);

    initialized = true;
    LOG(INFO) << "AlgorithmDialog::init completed";
    return true;
}

void AlgorithmDialog::on_radioButton_person_chose_master_toggled(bool checked) {
//    ui->lineEdit_master_time->setEnabled(checked);
//    ui->lineEdit_master_speed->setEnabled(checked);
}

void AlgorithmDialog::sendManuallyChosing(double x, double y) {
}

void AlgorithmDialog::on_pushButton_faceTemplate_clicked() {
    if (faceTemplateDialog) {
        faceTemplateDialog->show();
    }
}

boost::shared_ptr<AlgorithmDialog> AlgorithmDialog::getInstance() {
    static boost::shared_ptr<AlgorithmDialog> instance = boost::shared_ptr<AlgorithmDialog>(new AlgorithmDialog);
    return instance;
}

bool AlgorithmDialog::syncSendMsg(const AlgoParamMsg &msgSend, AlgoParamMsg &msgRet, int timeOut) {
    if (manager) {
        return manager->syncSendMsg(msgSend, msgRet, timeOut);
    }

    return false;
}

bool AlgorithmDialog::syncSendMsg(const AlgoParamMsg &msgSend,
                                  boost::function<void(const AlgoParamMsg &msgSend, void *)> callback, int timeOut) {
    if (manager) {
        return manager->syncSendMsg(msgSend, callback);
    }

    return false;
}

void AlgorithmDialog::updateFaceTemplateUi() {
    if (faceTemplateDialog) {
        faceTemplateDialog->updateFaceTemplateUi(manager->status);
    }
}

void AlgorithmDialog::updateVersionUi() {
    QString version = QString::number(manager->status.mutable_version_get()->base()) + "." +
            QString::number(manager->status.mutable_version_get()->major()) + "." +
            QString::number(manager->status.mutable_version_get()->minor());
    QString modelVersion = QString(manager->status.mutable_version_get()->model_version().data());
    QString copy_right = QString(manager->status.mutable_version_get()->copy_right().data());
    QString showStr = QString::fromLocal8Bit("算法版本： ") + version + "\n" +
            QString::fromLocal8Bit("模型版本： ") + modelVersion + "\n" +
            copy_right;
    ui->textEdit_version->setText(showStr);
}

void AlgorithmDialog::on_pushButton_gimbal_clicked() {
    if (gimbalDialog) {
        gimbalDialog->show();
    }
}

//void AlgorithmDialog::on_pushButton_zoom_clicked() {
//    if (zoomSlider) {}
//    zoomSlider->show();
//}

void AlgorithmDialog::on_pushButton_music_play_clicked() {
    player->setMedia(QUrl::fromLocalFile("/home/jianghualuo/work/data/audio/mariah_carey-my_all.flac"));
    player->setVolume(50);
    player->play();
    player->position();
}

void AlgorithmDialog::pushButton_zoom_clicked() {
    QPushButton *ptr = dynamic_cast<QPushButton*>(sender());
    if (!ptr) {
        return;
    }

    double  auto_scale = 0.0;
    double  auto_zmspd = 0.0;
    double  gap_zmspd = 0.0;
    double  gap_upperbound = 0.0;
    double  gap_lowerbound = 0.0;
    double  cycle_zmspd = 0.0;
    double  cycle_lowerbound = 0.0;
    double  cycle_upperbound = 0.0;
    try {
        auto_scale = boost::lexical_cast<double>(ui->lineEdit_auto_scale->text().toStdString());
        auto_zmspd = boost::lexical_cast<double>(ui->lineEdit_auto_zmspd->text().toStdString());
        gap_zmspd = boost::lexical_cast<double>(ui->lineEdit_gap_zmspd->text().toStdString());
        gap_upperbound = boost::lexical_cast<double>(ui->lineEdit_gap_upperbound->text().toStdString());
        gap_lowerbound = boost::lexical_cast<double>(ui->lineEdit_gap_lowerbound->text().toStdString());
        cycle_zmspd = boost::lexical_cast<double>(ui->lineEdit_cycle_zmspd->text().toStdString());
        cycle_lowerbound = boost::lexical_cast<double>(ui->lineEdit_cycle_lowerbound->text().toStdString());
        cycle_upperbound = boost::lexical_cast<double>(ui->lineEdit_cycle_upperbound->text().toStdString());
    } catch (std::exception &e) {
        LOG(INFO) << "AlgorithmDialog::pushButton_specialShot_clicked lexical_cast error";
        return;
    }

    AlgoParamMsg msg = AlgorithmManager::generateMsgByType(AlgoParam::MsgUnity::ZoomMode);
    if (ptr == ui->pushButton_zoom_none) {
        msg.mutable_zoom_mode()->set_zoom_mode(AlgoParam::ZoomMode::ZOOMMODE_NONE);
    } else if (ptr == ui->pushButton_zoom_auto) {
        msg.mutable_zoom_mode()->set_zoom_mode(AlgoParam::ZoomMode::ZOOMMODE_AUTO);
        msg.mutable_zoom_mode()->set_auto_zmspd(auto_zmspd);
        msg.mutable_zoom_mode()->set_auto_scale(auto_scale);
    } else if (ptr == ui->pushButton_zoom_gap) {
        msg.mutable_zoom_mode()->set_zoom_mode(AlgoParam::ZoomMode::ZOOMMODE_GAP);
        msg.mutable_zoom_mode()->set_gap_lowerbound(gap_lowerbound);
        msg.mutable_zoom_mode()->set_gap_upperbound(gap_upperbound);
        msg.mutable_zoom_mode()->set_gap_zmspd(gap_zmspd);
    } else if (ptr == ui->pushButton_zoom_cycle) {
        msg.mutable_zoom_mode()->set_zoom_mode(AlgoParam::ZoomMode::ZOOMMODE_CYCLE);
        msg.mutable_zoom_mode()->set_cycle_lowerbound(cycle_lowerbound);
        msg.mutable_zoom_mode()->set_cycle_upperbound(cycle_upperbound);
        msg.mutable_zoom_mode()->set_cycle_zmspd(cycle_zmspd);
    } else {
    }

    asyncSendMsg(msg);
}

void AlgorithmDialog::updateZoomModeUi() {
    AlgoParam::ZoomMode_Type type = manager->status.mutable_zoom_mode()->zoom_mode();
    ui->pushButton_zoom_none->setStyleSheet("background-color:;");
    ui->pushButton_zoom_auto->setStyleSheet("background-color:;");
    ui->pushButton_zoom_gap->setStyleSheet("background-color:;");
    ui->pushButton_zoom_cycle->setStyleSheet("background-color:;");

    if (AlgoParam::ZoomMode::ZOOMMODE_NONE == type) {
        ui->pushButton_zoom_none->setStyleSheet("background-color:red;");
    } else if (AlgoParam::ZoomMode::ZOOMMODE_AUTO == type) {
        ui->pushButton_zoom_auto->setStyleSheet("background-color:red;");
        double auto_scale = manager->status.mutable_zoom_mode()->auto_scale();
        double auto_zmspd = manager->status.mutable_zoom_mode()->auto_zmspd();
        ui->lineEdit_auto_scale->setText(QString::number(auto_scale));
        ui->lineEdit_auto_zmspd->setText(QString::number(auto_zmspd));
    } else if (AlgoParam::ZoomMode::ZOOMMODE_GAP == type) {
        ui->pushButton_zoom_gap->setStyleSheet("background-color:red;");
        double gap_lowerbound = manager->status.mutable_zoom_mode()->gap_lowerbound();
        double gap_upperbound = manager->status.mutable_zoom_mode()->gap_upperbound();
        double gap_zmspd = manager->status.mutable_zoom_mode()->gap_zmspd();
        ui->lineEdit_gap_lowerbound->setText(QString::number(gap_lowerbound));
        ui->lineEdit_gap_upperbound->setText(QString::number(gap_upperbound));
        ui->lineEdit_gap_zmspd->setText(QString::number(gap_zmspd));
    } else if (AlgoParam::ZoomMode::ZOOMMODE_CYCLE == type) {
        ui->pushButton_zoom_cycle->setStyleSheet("background-color:red;");
        double cycle_lowerbound = manager->status.mutable_zoom_mode()->cycle_lowerbound();
        double cycle_upperbound = manager->status.mutable_zoom_mode()->cycle_upperbound();
        double cycle_zmspd = manager->status.mutable_zoom_mode()->cycle_zmspd();
        ui->lineEdit_cycle_lowerbound->setText(QString::number(cycle_lowerbound));
        ui->lineEdit_cycle_upperbound->setText(QString::number(cycle_upperbound));
        ui->lineEdit_cycle_zmspd->setText(QString::number(cycle_zmspd));
    } else {
    }

    ui->pushButton_zoom_cycle->setEnabled(false);
    ui->pushButton_zoom_gap->setEnabled(false);
    ui->pushButton_zoom_auto->setEnabled(false);
    ui->pushButton_zoom_none->setEnabled(false);
    for (int i = 0; i < manager->status.mutable_zoom_mode()->zoom_mode_supported_size(); ++i) {
        AlgoParam::ZoomMode_Type mode = manager->status.mutable_zoom_mode()->zoom_mode_supported(i);
        if (AlgoParam::ZoomMode::ZOOMMODE_NONE == mode) {
            ui->pushButton_zoom_none->setEnabled(true);
        } else if (AlgoParam::ZoomMode::ZOOMMODE_AUTO == mode) {
            ui->pushButton_zoom_auto->setEnabled(true);
        } else if (AlgoParam::ZoomMode::ZOOMMODE_GAP == mode) {
            ui->pushButton_zoom_gap->setEnabled(true);
        } else if (AlgoParam::ZoomMode::ZOOMMODE_CYCLE == mode) {
            ui->pushButton_zoom_cycle->setEnabled(true);
        }
    }
}

void AlgorithmDialog::pushButton_specialShot_clicked() {
    QPushButton *ptr = dynamic_cast<QPushButton*>(sender());
    if (!ptr) {
        return;
    }

    double slowzoom_zmspd = 0.0;
    double slowzoom_time = 0.0;
    double oncezoom_scale = 0.0;
    double oncezoom_zmspd = 0.0;
    double scan_scale = 0.0;
    double scan_zmspd = 0.0;
    double scan_speed = 0.0;
    double scan_time = 0.0;
    double scan_time_shot = 0.0;
    double shot_scale = 0.0;
    double shot_zmspd = 0.0;
    double shot_time = 0.0;
    double shot_focus_upper = 0.0;
    try {
        slowzoom_zmspd = boost::lexical_cast<double>(ui->lineEdit_slowzoom_zmspd->text().toStdString());
        slowzoom_time = boost::lexical_cast<double>(ui->lineEdit_slowzoom_time->text().toStdString());
        oncezoom_scale = boost::lexical_cast<double>(ui->lineEdit_oncezoom_scale->text().toStdString());
        oncezoom_zmspd = boost::lexical_cast<double>(ui->lineEdit_oncezoom_zmspd->text().toStdString());
        scan_scale = boost::lexical_cast<double>(ui->lineEdit_scan_scale->text().toStdString());
        scan_zmspd = boost::lexical_cast<double>(ui->lineEdit_scan_zmspd->text().toStdString());
        scan_speed = boost::lexical_cast<double>(ui->lineEdit_scan_speed->text().toStdString());
        scan_time = boost::lexical_cast<double>(ui->lineEdit_scan_time->text().toStdString());
        scan_time_shot = boost::lexical_cast<double>(ui->lineEdit_scan_time_shot->text().toStdString());
        shot_scale = boost::lexical_cast<double>(ui->lineEdit_shot_scale->text().toStdString());
        shot_zmspd = boost::lexical_cast<double>(ui->lineEdit_shot_zmspd->text().toStdString());
        shot_time = boost::lexical_cast<double>(ui->lineEdit_shot_time->text().toStdString());
        shot_focus_upper = boost::lexical_cast<double>(ui->lineEdit_shot_focus_upper->text().toStdString());
    } catch (std::exception &e) {
        LOG(INFO) << "AlgorithmDialog::pushButton_specialShot_clicked lexical_cast error";
        return;
    }


    //处理rollswing
    std::vector<double> rollswing_period;
    std::vector<double> rollswing_speed;
    QStringList rollswing_speed_strList = ui->lineEdit_rollswing_speed->text().split(',', QString::SkipEmptyParts);
    for (auto it : rollswing_speed_strList) {
        it = it.trimmed();
        try {
            rollswing_speed.push_back(boost::lexical_cast<double>(it.toStdString()));
         } catch (std::exception &e) {
            LOG(INFO) << "AlgorithmDialog::pushButton_specialShot_clicked lexical_cast error";
            continue;
        }
    }
    QStringList rollswing_period_strList = ui->lineEdit_rollswing_period->text().split(',', QString::SkipEmptyParts);
    for (auto it : rollswing_period_strList) {
        it = it.trimmed();
        try {
            rollswing_period.push_back(boost::lexical_cast<double>(it.toStdString()));
        } catch (std::exception &e) {
            LOG(INFO) << "AlgorithmDialog::pushButton_specialShot_clicked lexical_cast error";
            continue;
        }
    }

    if (rollswing_period.size() != rollswing_speed.size()) {
        LOG(INFO) << "AlgorithmDialog::pushButton_specialShot_clicked rollswing size not not equal";
        return;
    }

    //处理cyclezoom
    std::vector<double> cyclezoom_period;
    std::vector<double> cyclezoom_zmspd;
    QStringList cyclezoom_period_strList = ui->lineEdit_cyclezoom_period->text().split(',', QString::SkipEmptyParts);
    for (auto it : cyclezoom_period_strList) {
        it = it.trimmed();
        try {
            cyclezoom_period.push_back(boost::lexical_cast<double>(it.toStdString()));
        } catch (std::exception &e) {
            LOG(INFO) << "AlgorithmDialog::pushButton_specialShot_clicked cyclezoom lexical_cast error";
            continue;
        }
    }
    QStringList cyclezoom_zmspd_strList = ui->lineEdit_cyclezoom_zmspd->text().split(',', QString::SkipEmptyParts);
    for (auto it : cyclezoom_zmspd_strList) {
        it = it.trimmed();
        try {
            cyclezoom_zmspd.push_back(boost::lexical_cast<double>(it.toStdString()));
        } catch (std::exception &e) {
            LOG(INFO) << "AlgorithmDialog::pushButton_specialShot_clicked cyclezoom lexical_cast error";
            continue;
        }
    }

    if (cyclezoom_period.size() != cyclezoom_zmspd.size()) {
        LOG(INFO) << "AlgorithmDialog::pushButton_specialShot_clicked cyclezoom size not not equal";
        return;
    }

    AlgoParamMsg msg = AlgorithmManager::generateMsgByType(AlgoParam::MsgUnity::SpecialShot);
    if (ptr == ui->pushButton_shot) {
        msg.mutable_special_shot()->set_special_shot(AlgoParam::SpecialShot::SPECIALSHOT_SHOT);
        msg.mutable_special_shot()->set_shot_focus_upper(shot_focus_upper);
        msg.mutable_special_shot()->set_shot_scale(shot_scale);
        msg.mutable_special_shot()->set_shot_time(shot_time);
        msg.mutable_special_shot()->set_shot_zmspd(shot_zmspd);
    } else if (ptr == ui->pushButton_scan) {
        msg.mutable_special_shot()->set_special_shot(AlgoParam::SpecialShot::SPECIALSHOT_SCAN);
        msg.mutable_special_shot()->set_scan_scale(scan_scale);
        msg.mutable_special_shot()->set_scan_speed(scan_speed);
        msg.mutable_special_shot()->set_scan_time(scan_time);
        msg.mutable_special_shot()->set_scan_zmspd(scan_zmspd);
        msg.mutable_special_shot()->set_scan_time_shot(scan_time_shot);
    } else if (ptr == ui->pushButton_roolSwing) {
        msg.mutable_special_shot()->set_special_shot(AlgoParam::SpecialShot::SPECIALSHOT_ROLLSWING);
        for (int i = 0; i < rollswing_period.size(); ++i) {
            msg.mutable_special_shot()->add_rollswing_period(rollswing_period[i]);
            msg.mutable_special_shot()->add_rollswing_period(rollswing_speed[i]);
        }
    } else if (ptr == ui->pushButton_cycleZoom) {
        msg.mutable_special_shot()->set_special_shot(AlgoParam::SpecialShot::SPECIALSHOT_CYCLEZOOM);
        for (int i = 0; i < rollswing_period.size(); ++i) {
            msg.mutable_special_shot()->add_cyclezoom_period(cyclezoom_period[i]);
            msg.mutable_special_shot()->add_cyclezoom_zmspd(cyclezoom_zmspd[i]);
        }
    } else if (ptr == ui->pushButton_onceZoom) {
        msg.mutable_special_shot()->set_special_shot(AlgoParam::SpecialShot::SPECIALSHOT_ONCEZOOM);
        msg.mutable_special_shot()->set_oncezoom_scale(oncezoom_scale);
        msg.mutable_special_shot()->set_oncezoom_zmspd(oncezoom_zmspd);
    } else if (ptr == ui->pushButton_slowzoom) {
        msg.mutable_special_shot()->set_special_shot(AlgoParam::SpecialShot::SPECIALSHOT_SLOWZOOM);
        msg.mutable_special_shot()->set_slowzoom_time(slowzoom_time);
        msg.mutable_special_shot()->set_slowzoom_zmspd(slowzoom_zmspd);
    } else {
    }

    if (isSpecialShotting) {
        msg.mutable_special_shot()->set_special_shot(AlgoParam::SpecialShot::SPECIALSHOT_NONE);
    }
    asyncSendMsg(msg);
}

void AlgorithmDialog::updateSpecialShotUi() {
    AlgoParam::SpecialShot::SpecialShotType type = manager->status.mutable_special_shot()->special_shot();

    if (type == AlgoParam::SpecialShot::SPECIALSHOT_SLOWZOOM) {
        ui->pushButton_slowzoom->setText(QString::fromLocal8Bit("停止"));
        ui->pushButton_slowzoom->setStyleSheet("background-color:red;");
    } else if (type == AlgoParam::SpecialShot::SPECIALSHOT_ONCEZOOM) {
        ui->pushButton_onceZoom->setText(QString::fromLocal8Bit("停止"));
        ui->pushButton_onceZoom->setStyleSheet("background-color:red;");
    } else if (type == AlgoParam::SpecialShot::SPECIALSHOT_CYCLEZOOM) {
        ui->pushButton_cycleZoom->setText(QString::fromLocal8Bit("停止"));
        ui->pushButton_cycleZoom->setStyleSheet("background-color:red;");
    } else if (type == AlgoParam::SpecialShot::SPECIALSHOT_ROLLSWING) {
        ui->pushButton_roolSwing->setText(QString::fromLocal8Bit("停止"));
        ui->pushButton_roolSwing->setStyleSheet("background-color:red;");
    } else if (type == AlgoParam::SpecialShot::SPECIALSHOT_SHOT) {
        ui->pushButton_shot->setText(QString::fromLocal8Bit("停止"));
        ui->pushButton_shot->setStyleSheet("background-color:red;");
    } else if (type == AlgoParam::SpecialShot::SPECIALSHOT_SCAN) {
        ui->pushButton_scan->setText(QString::fromLocal8Bit("停止"));
        ui->pushButton_scan->setStyleSheet("background-color:red;");
    } else if (type == AlgoParam::SpecialShot::SPECIALSHOT_NONE) {
        ui->pushButton_cycleZoom->setText(QString::fromLocal8Bit("开始"));
        ui->pushButton_slowzoom->setText(QString::fromLocal8Bit("开始"));
        ui->pushButton_roolSwing->setText(QString::fromLocal8Bit("开始"));
        ui->pushButton_onceZoom->setText(QString::fromLocal8Bit("开始"));
        ui->pushButton_scan->setText(QString::fromLocal8Bit("开始"));
        ui->pushButton_shot->setText(QString::fromLocal8Bit("开始"));
        ui->pushButton_slowzoom->setStyleSheet("background-color:;");
        ui->pushButton_roolSwing->setStyleSheet("background-color:;");
        ui->pushButton_onceZoom->setStyleSheet("background-color:;");
        ui->pushButton_scan->setStyleSheet("background-color:;");
        ui->pushButton_shot->setStyleSheet("background-color:;");
        ui->pushButton_cycleZoom->setStyleSheet("background-color:;");
    }

    ui->pushButton_slowzoom->setEnabled(false);
    ui->pushButton_cycleZoom->setEnabled(false);
    ui->pushButton_roolSwing->setEnabled(false);
    ui->pushButton_onceZoom->setEnabled(false);
    ui->pushButton_shot->setEnabled(false);
    ui->pushButton_scan->setEnabled(false);
    for (int i = 0; i < manager->status.mutable_special_shot()->special_shot_supported_size(); ++i) {
        AlgoParam::SpecialShot::SpecialShotType supportedType =
                manager->status.mutable_special_shot()->special_shot_supported(i);
        if (supportedType == AlgoParam::SpecialShot::SPECIALSHOT_SLOWZOOM) {
            ui->pushButton_slowzoom->setEnabled(true);
        } else if (supportedType == AlgoParam::SpecialShot::SPECIALSHOT_SCAN) {
            ui->pushButton_scan->setEnabled(true);
        } else if (supportedType == AlgoParam::SpecialShot::SPECIALSHOT_SHOT) {
            ui->pushButton_shot->setEnabled(true);
        } else if (supportedType == AlgoParam::SpecialShot::SPECIALSHOT_ONCEZOOM) {
            ui->pushButton_onceZoom->setEnabled(true);
        } else if (supportedType == AlgoParam::SpecialShot::SPECIALSHOT_ROLLSWING) {
            ui->pushButton_roolSwing->setEnabled(true);
        } else if (supportedType == AlgoParam::SpecialShot::SPECIALSHOT_CYCLEZOOM) {
            ui->pushButton_cycleZoom->setEnabled(true);
        }
    }
}
