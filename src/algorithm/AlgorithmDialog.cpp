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
    } else {
        LOG(INFO) << "AlgorithmDialog::onMsgGot unknown msg type";
    }
}

void AlgorithmDialog::updateSelectionUi() {
//    ui->groupBox_person_chose->setChecked(manager->status.mutable_selection_set()->selecting());

    AlgoParam::SelectionSet::SelectMode mode = manager->status.mutable_selection_set()->select_mode();
    if (AlgoParam::SelectionSet::SELECTION_USERPICK == mode) {
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
        ui->radioButton_person_chose_master->setChecked(false);
        ui->radioButton_person_chose_multiple->setChecked(false);
        ui->radioButton_person_chose_auto->setChecked(false);
    } else if (AlgoParam::SelectionSet::SELECTION_AUTO == mode) {
        ui->radioButton_person_chose_auto->setChecked(true);
    } else if (AlgoParam::SelectionSet::SELECTION_MASTER == mode) {
        ui->radioButton_person_chose_master->setChecked(true);
        ui->lineEdit_master_speed->setText(QString::number(manager->status.mutable_selection_set()->master_speed()));
        ui->lineEdit_master_time->setText(QString::number(manager->status.mutable_selection_set()->master_time()));
    } else if (AlgoParam::SelectionSet::SELECTION_MULTIPLE == mode) {
        ui->radioButton_person_chose_multiple->setChecked(true);
    }

    if (!manager->status.mutable_selection_set()->is_selecting()) {
        ui->radioButton_person_chose_auto->setStyleSheet("background-color:;");
        ui->radioButton_person_chose_master->setStyleSheet("background-color:;");
        ui->radioButton_person_chose_multiple->setStyleSheet("background-color:;");
    }


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
    AlgoParamMsg msg =  AlgorithmManager::generateMsgByType(AlgoParam::MsgUnity::SelectionSet);
    msg.mutable_selection_set()->set_is_selecting(checked);
    asyncSendMsg(msg);

    ui->workSpaceWidget->setManuallyChosingPerson(checked);
}

void AlgorithmDialog::radioButton_person_chose_clicked(bool checked) {
    QRadioButton *ptr = dynamic_cast<QRadioButton*>(sender());
    if (ptr) {
        AlgoParam::SelectionSet::SelectMode mode;
        AlgoParamMsg msg =  AlgorithmManager::generateMsgByType(AlgoParam::MsgUnity::SelectionSet);
        if (ptr == ui->radioButton_person_chose_multiple) {
            ui->radioButton_person_chose_auto->setStyleSheet("background-color:;");
            ui->radioButton_person_chose_master->setStyleSheet("background-color:;");
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
            ui->radioButton_person_chose_master->setStyleSheet("background-color:;");
            ui->radioButton_person_chose_multiple->setStyleSheet("background-color:;");
        } else if (ptr == ui->radioButton_person_chose_master) {
            mode = AlgoParam::SelectionSet::SELECTION_MASTER;
            ui->radioButton_person_chose_auto->setStyleSheet("background-color:;");
            ui->radioButton_person_chose_multiple->setStyleSheet("background-color:;");
        }

        ptr->setStyleSheet("background-color:red;");

        msg.mutable_selection_set()->set_select_mode(mode);
        msg.mutable_selection_set()->set_is_selecting(ui->groupBox_person_chose->isChecked());
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
//    if (!manager->tcpClientConnect()) {
//        QMessageBox::warning(nullptr, "警告", "网络错误", QMessageBox::Ok);
//        return false;
//    }
//
//    //检查是否初始化
//    int checkTimes = 1000;
//    bool algorithmInitialized = false;
//    while (!algorithmInitialized && checkTimes--) {
//        algorithmInitialized = manager->status.contain_allget();
//        boost::this_thread::sleep_for(boost::chrono::milliseconds(10));
//    }
//    if (!algorithmInitialized) {
//        QMessageBox::warning(nullptr, "警告", "算法未初始化", QMessageBox::Ok);
//        return false;
//    }

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

    ui->lineEdit_master_time->setValidator(new QDoubleValidator(0, 100000, 5));
    ui->lineEdit_master_speed->setValidator(new QDoubleValidator(0, 100000, 5));

    initialized = true;
    LOG(INFO) << "AlgorithmDialog::init completed";
    return true;
}

void AlgorithmDialog::on_radioButton_person_chose_master_toggled(bool checked) {
    ui->lineEdit_master_time->setEnabled(checked);
    ui->lineEdit_master_speed->setEnabled(checked);
}

void AlgorithmDialog::sendManuallyChosing(double x, double y) {
    AlgoParamMsg msg =  AlgorithmManager::generateMsgByType(AlgoParam::MsgUnity::SelectionSet);
    msg.mutable_selection_set()->set_select_mode(AlgoParam::SelectionSet::SELECTION_USERPICK);
    msg.mutable_selection_set()->add_userpick_point(x);
    msg.mutable_selection_set()->add_userpick_point(y);
    manager->asyncSendMsg(msg);
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

void AlgorithmDialog::on_pushButton_zoom_clicked() {
    if (zoomSlider) {}
    zoomSlider->show();
}

void AlgorithmDialog::on_pushButton_music_play_clicked() {
    player->setMedia(QUrl::fromLocalFile("/home/jianghualuo/work/data/audio/mariah_carey-my_all.flac"));
    player->setVolume(50);
    player->play();
}
