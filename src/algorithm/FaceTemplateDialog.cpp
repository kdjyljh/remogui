//
// Created by jianghualuo on 18-4-12.
//

#include "FaceTemplateDialog.h"

#include <QFileDialog>
#include <QInputDialog>
#include <QProgressBar>
#include <QProgressDialog>
#include <QDesktopWidget>
#include <QMessageBox>
#include <glog/logging.h>

FaceTemplateDialog::FaceTemplateDialog(QWidget *parent) :
        contentWidget(new QWidget(this)),
        faceItemWidget(new QWidget(this)),
        pushButtonCreate(new QPushButton(QString::fromLocal8Bit("新增"), this)),
        faceTemplateItemHeight(150),
        QScrollArea(parent) {
    setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, QSize(300, 600),
                                                qApp->desktop()->availableGeometry()));
    setWidget(contentWidget);
    contentWidget->resize(size());
    contentWidget->setLayout(new QVBoxLayout(contentWidget));
    contentWidget->layout()->addWidget(pushButtonCreate);
    contentWidget->layout()->addWidget(faceItemWidget);
    faceItemWidget->setLayout(new QVBoxLayout(faceItemWidget));

    setFixedSize(size());

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    faceItemList.push_back(AlgoParam::FaceTemplate());
    faceItemList.push_back(AlgoParam::FaceTemplate());
    faceItemList.push_back(AlgoParam::FaceTemplate());
    faceItemList.push_back(AlgoParam::FaceTemplate());
    faceItemList.push_back(AlgoParam::FaceTemplate());
    faceItemList.push_back(AlgoParam::FaceTemplate());
    faceItemList.push_back(AlgoParam::FaceTemplate());
    faceItemList.push_back(AlgoParam::FaceTemplate());
    refreshItem();

    setWindowModality(Qt::ApplicationModal);

    connect(pushButtonCreate, SIGNAL(clicked()), this, SLOT(pushButtonCreateClicked()));
}

FaceTemplateDialog::~FaceTemplateDialog() {
    delete contentWidget;
}

void FaceTemplateDialog::refreshItem() {
    //先全部删除
    for (auto it : faceItemUiList) {
        contentWidget->layout()->removeWidget(it);
    }

    std::list<FaceTemplateItem*> newUiList;
    //根据faceItemList修改itemUi
    for (auto it : faceItemList) {
        if (it.use_cached_face_image()) {
            //需要保留界面里的face image
            bool foundItem = false;
            for (auto uiIt : faceItemUiList) {
                if (uiIt->faceData.id_inlib() == it.id_inlib()) {
                    newUiList.push_back(uiIt);
                    foundItem = true;
                    break;
                }
            }
            if (!foundItem) {
                LOG(INFO) << "FaceTemplateDialog::refreshItem can't find item in ui id=" << it.id_inlib();
            }
        } else {
            //否则插入新的itemUi
            newUiList.push_back(new FaceTemplateItem(it));
        }
    }

    for (auto it : faceItemUiList) {
        delete it;
    }
    faceItemUiList.assign(newUiList.begin(), newUiList.end());

    //重新插入判序后到item
    faceItemUiList.sort(std::greater<FaceTemplateItem*>());
    for (auto it : faceItemUiList) {
        if (it == faceItemUiList.front()) {
            it->setPushButtonDownEnable(true);
            it->setPushButtonUpEnable(false);
        } else if (it == faceItemUiList.back()) {
            it->setPushButtonUpEnable(true);
            it->setPushButtonDownEnable(false);
        } else {
            it->setPushButtonUpEnable(true);
            it->setPushButtonDownEnable(true);
        }
        faceItemWidget->layout()->addWidget(it);
    }

    contentWidget->setFixedSize(width(), faceTemplateItemHeight * faceItemUiList.size() + 50);
}

void FaceTemplateDialog::pushButtonCreateClicked() {
    QString fileName = QFileDialog::getOpenFileName(NULL, "选择人脸", ".", "*.jpg *.jpeg");
    if (!fileName.isEmpty()) {
        QString faceName = QInputDialog::getText(this, QString::fromLocal8Bit("输入名称"),
                                                 tr("名称:"), QLineEdit::Normal,
                                                 QDir::home().dirName());
        LOG(INFO) << "FaceTemplateDialog::pushButtonCreateClicked fileName:" << fileName.toStdString();
        if (!faceName.isEmpty()) {
            AlgoParamMsg msg = AlgorithmManager::generateMsgByType(AlgoParam::MsgUnity::FaceTemplLibSet);
            QImage image(fileName);
            msg.mutable_face_templlib_set()->set_operation(AlgoParam::FaceTemplLibSet::OP_CREATE);
            msg.mutable_face_templlib_set()->mutable_face_templ()->set_face_name(faceName.toStdString());
            msg.mutable_face_templlib_set()->mutable_face_templ()->set_face_image((char*)image.bits());
            syncFaceTempHandle(msg);
            LOG(INFO) << "FaceTemplateDialog::pushButtonCreateClicked faceName:" << faceName.toStdString();
        }
    }
}

void FaceTemplateDialog::syncFaceTempHandle(const AlgoParamMsg &msg) {
    QDialog dialog;
    QProgressBar *progressBar = new QProgressBar(&dialog);
    progressBar->setMinimum(0);
    progressBar->setMaximum(0);
    dialog.setLayout(new QHBoxLayout(&dialog));
    dialog.layout()->addWidget(progressBar);
    dialog.setWindowFlags(Qt::Widget | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    dialog.setWindowOpacity(0.9);
    dialog.setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, QSize(200, 10), qApp->desktop()->availableGeometry()));
    dialog.setContentsMargins(0,0,0,0);
    dialog.layout()->setContentsMargins(0,0,0,0);
    AlgoParamMsg retMsg = AlgorithmManager::generateMsgByType(AlgoParam::MsgUnity::FaceTemplLibSet);
    bool ret = false;
    boost::thread([&](){
        ret = AlgorithmDialog::getInstance()->syncSendMsg(msg, retMsg);
        dialog.close();
    });
    dialog.setWindowFlags(dialog.windowFlags() | Qt::WindowStaysOnTopHint);
    dialog.exec();

    if (!ret) {
        QMessageBox::warning(nullptr, "错误", "网络错误", QMessageBox::Ok);
        return;
    }

    int type = retMsg.type(0);
    AlgoParam::ErrorCode ec;
    QString faceName;
    if (type == AlgoParam::MsgUnity::FaceTemplLibSet) {
        if (retMsg.has_face_templlib_set()) {
            ec = retMsg.mutable_face_templlib_set()->error_code();
            faceName = QString::fromLocal8Bit(retMsg.mutable_face_templlib_set()->mutable_face_templ()->face_name().data());
        } else {
            LOG(INFO) << "FaceTemplateDialog::faceTempHandle msg has no face_templlib";
            return;
        }
    } else if (type == AlgoParam::MsgUnity::FaceTemplLibModify) {
        if (retMsg.has_face_templlib_modify()) {
            ec = retMsg.mutable_face_templlib_modify()->error_code();
            faceName = QString::fromLocal8Bit(retMsg.mutable_face_templlib_modify()->mutable_change_names(0)->data());
        } else {
            LOG(INFO) << "FaceTemplateDialog::faceTempHandle msg has no face_templlib";
            return;
        }
    }

    if (AlgoParam::ERR_FACE_NAME_INVALID == ec) {
        QString errorStr = QString::fromLocal8Bit("名称非法:") + faceName;
        QMessageBox::warning(nullptr, QString::fromLocal8Bit("错误"), errorStr, QMessageBox::Ok);
    } else if (AlgoParam::ERR_FACE_NAME_ALREADY_EXIST == ec) {
        QString errorStr = QString::fromLocal8Bit("人名已存在:") + faceName;
        QMessageBox::warning(nullptr, QString::fromLocal8Bit("错误"), errorStr, QMessageBox::Ok);
    } else if (AlgoParam::ERR_FACE_WRONG_IMAGE == ec) {
        QString errorStr = QString::fromLocal8Bit("人脸非法:") + faceName;
        QMessageBox::warning(nullptr, QString::fromLocal8Bit("错误"), errorStr, QMessageBox::Ok);
    } else if (AlgoParam::ERR_FACE_IMAGE_ALREADY_EXIST == ec) {
        QString errorStr = QString::fromLocal8Bit("人脸非法:") + faceName;
        if (QMessageBox::Ok == QMessageBox::warning(nullptr, QString::fromLocal8Bit("错误"), errorStr,
                                                    QMessageBox::Ok | QMessageBox::Cancel)) {
            AlgoParamMsg updateMsg = msg;
            updateMsg.mutable_face_templlib_set()->set_operation(AlgoParam::FaceTemplLibSet::OP_DELETE);
            syncFaceTempHandle(updateMsg);
        }
    } else if (AlgoParam::SUCCEED == ec) {
        QMessageBox::warning(nullptr, "通知", "添加成功", QMessageBox::Ok);
    } else {
        QString errorStr = QString::fromLocal8Bit("未知错误:") + faceName;
        QMessageBox::warning(nullptr, QString::fromLocal8Bit("错误"), errorStr, QMessageBox::Ok);
    }
}

void FaceTemplateDialog::updateFaceTemplateUi(const AlgoParamMsg &msg) {
    if (AlgoParam::SUCCEED == msg.face_templlib_get().error_code()) {
        faceItemList.clear();
        for (int i = 0; i < msg.face_templlib_get().face_templ_all_size(); ++i) {
            faceItemList.push_back(msg.face_templlib_get().face_templ_all(i));
        }
    }
}
