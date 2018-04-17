//
// Created by jianghualuo on 18-4-12.
//

#include "FaceTemplateItem.h"
#include <glog/logging.h>
#include <QFileDialog>

FaceTemplateItem::FaceTemplateItem(AlgoParam::FaceTemplate ft, QWidget *parent) :
        faceData(ft),
        mainLayout(new QHBoxLayout(this)),
        imageWidget(new AlgorithmImageWidget(this)),
        lineEditFaceName(new QLineEdit(this)),
        checkBoxIsMaster(new QCheckBox(QString::fromLocal8Bit("是否主人"), this)),
        pushButtonUp(new QPushButton(QString::fromLocal8Bit("上移"), this)),
        pushButtonDown(new QPushButton(QString::fromLocal8Bit("下移"), this)),
        pushButtonChoseFaceTemp(new QPushButton(QString::fromLocal8Bit("重拍"), this)),
        pushButtonDeleteFaceTemp(new QPushButton(QString::fromLocal8Bit("删除"), this)),
        QWidget(parent) {
    setLayout(mainLayout);
    mainLayout->addWidget(imageWidget);
    QWidget *vWidget1 = new QWidget(this);
    vWidget1->setLayout(new QVBoxLayout(vWidget1));
    mainLayout->addWidget(vWidget1);
    vWidget1->setMaximumWidth(130);
    vWidget1->layout()->addWidget(checkBoxIsMaster);
    vWidget1->layout()->addWidget(lineEditFaceName);
    QWidget *hWidget1 = new QWidget(this);
    hWidget1->setLayout(new QHBoxLayout(hWidget1));
    hWidget1->layout()->addWidget(pushButtonUp);
    hWidget1->layout()->addWidget(pushButtonDown);
    vWidget1->layout()->addWidget(hWidget1);
    QWidget *hWidget2 = new QWidget(this);
    hWidget2->setLayout(new QHBoxLayout(hWidget2));
    hWidget2->layout()->addWidget(pushButtonChoseFaceTemp);
    hWidget2->layout()->addWidget(pushButtonDeleteFaceTemp);
    vWidget1->layout()->addWidget(hWidget2);

    hWidget1->setContentsMargins(0,0,0,0);
    hWidget1->layout()->setContentsMargins(0,0,0,0);
    hWidget2->setContentsMargins(0,0,0,0);
    hWidget2->layout()->setContentsMargins(0,0,0,0);

    connect(pushButtonDeleteFaceTemp, SIGNAL(clicked()), this, SLOT(deleteFaceTemp_clicked()));
    connect(pushButtonChoseFaceTemp, SIGNAL(clicked()), this, SLOT(pushButtonChoseFaceTemp_clicked()));
    connect(lineEditFaceName, SIGNAL(returnPressed()), this, SLOT(lineEditFaceName_returnPressed()));
    connect(checkBoxIsMaster, SIGNAL(clicked(bool)), this, SLOT(checkBoxIsMaster_clicked(bool)));
    connect(pushButtonUp, SIGNAL(clicked()), this, SLOT(pushButtonUpDown_clicked()));
    connect(pushButtonDown, SIGNAL(clicked()), this, SLOT(pushButtonUpDown_clicked()));
}

FaceTemplateItem::~FaceTemplateItem() {

}

void FaceTemplateItem::deleteFaceTemp_clicked() {
    AlgoParam::MsgUnity msg = AlgorithmManager::generateMsgByType(AlgoParam::MsgUnity::FaceTemplLibSet);
    msg.mutable_face_templlib_set()->set_operation(AlgoParam::FaceTemplLibSet::OP_DELETE);
    msg.mutable_face_templlib_set()->mutable_face_templ()->set_face_name(faceData.face_name());
//    AlgorithmDialog::getInstance()->asyncSendMsg(msg);
    FaceTemplateDialog::syncFaceTempHandle(msg);
}

void FaceTemplateItem::pushButtonChoseFaceTemp_clicked() {
    QString fileName = QFileDialog::getOpenFileName(NULL, QString::fromLocal8Bit("选择人脸"), ".", "*.jpg *.jpeg");
    if (!fileName.isEmpty()) {
        LOG(INFO) << "FaceTemplateItem::pushButtonChoseFaceTemp_clicked fileName:" << fileName.toStdString();
        AlgoParamMsg msg = AlgorithmManager::generateMsgByType(AlgoParam::MsgUnity::FaceTemplLibSet);
        QImage image(fileName);
        msg.mutable_face_templlib_set()->set_operation(AlgoParam::FaceTemplLibSet::OP_UPDATE);
        msg.mutable_face_templlib_set()->mutable_face_templ()->set_face_name(faceData.face_name());
        msg.mutable_face_templlib_set()->mutable_face_templ()->set_face_image(reinterpret_cast<char*>(image.bits()));
        FaceTemplateDialog::syncFaceTempHandle(msg);
    }
}

void FaceTemplateItem::lineEditFaceName_returnPressed() {
    AlgoParamMsg msg = AlgorithmManager::generateMsgByType(AlgoParam::MsgUnity::FaceTemplLibModify);
    msg.mutable_face_templlib_modify()->add_change_names(faceData.face_name());
    msg.mutable_face_templlib_modify()->add_change_names(lineEditFaceName->text().toStdString());
    FaceTemplateDialog::syncFaceTempHandle(msg);
}

void FaceTemplateItem::checkBoxIsMaster_clicked(bool checked) {
    AlgoParamMsg msg = AlgorithmManager::generateMsgByType(AlgoParam::MsgUnity::FaceTemplLibModify);
    msg.mutable_face_templlib_modify()->set_masterfunc_name(faceData.face_name());
    msg.mutable_face_templlib_modify()->set_masterfunc_enabled(checked);
    FaceTemplateDialog::syncFaceTempHandle(msg);
}

void FaceTemplateItem::pushButtonUpDown_clicked() {
    QPushButton *ptr = dynamic_cast<QPushButton*>(sender());
    int step = 1;
    if (!ptr) {
        return;
    } else if (ptr == pushButtonDown) {
        step = -1;
    }

    AlgoParamMsg msg = AlgorithmManager::generateMsgByType(AlgoParam::MsgUnity::FaceTemplLibModify);
    msg.mutable_face_templlib_modify()->set_masterfunc_name(faceData.face_name());
    msg.mutable_face_templlib_modify()->set_movefunc_relative_pos(step);
    FaceTemplateDialog::syncFaceTempHandle(msg);
}
