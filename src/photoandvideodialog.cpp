#include "photoandvideodialog.h"

#include <QDebug>

static const unsigned DEFAULT_DIALOG_WITH = 500;
static const unsigned DEFAULT_DIALOG_HEIGHT = 500;

PhotoAndVideoDialog::PhotoAndVideoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PhotoAndVideoDialog)
{
    ui->setupUi(this);
    setFixedSize(DEFAULT_DIALOG_WITH, DEFAULT_DIALOG_HEIGHT);

    initConnect();
    getSettingFromUi();
}

PhotoAndVideoDialog::~PhotoAndVideoDialog()
{
    qDebug() << "PhotoAndVideoDialog has been deleted!!";
    delete ui;
}

void PhotoAndVideoDialog::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    emit cameraSettingChanged(photoAndVideoSetting); // 关闭对话框前需要更新设置 ## emit到哪里？
}

void PhotoAndVideoDialog::setPhotoAndvideoUiBySetting(const PhotoAndVideoSetting &pvSetting)
{

}

PhotoAndVideoSetting PhotoAndVideoDialog::getSettingFromUi()
{
    photoAndVideoSetting.photoContinusTime = ui->LineEdit_photo_continus->text().toInt();
    photoAndVideoSetting.photoDelayedTime = ui->LineEdit_photo_delayed->text().toInt();
    photoAndVideoSetting.photoLapsecontinueTime = ui->LineEdit_photo_lapse_continueTime->text().toInt();
    photoAndVideoSetting.photoLapseIntervalTime = ui->LineEdit_photo_lapse_intervalTime->text().toDouble();
    photoAndVideoSetting.photoMultiTime = ui->LineEdit_photo_multi->text().toInt();
    photoAndVideoSetting.photoPanoramaFps = ui->LineEdit_photo_panorama->text().toInt();

    photoAndVideoSetting.videoCircleTime = ui->LineEdit_video_circle->text().toInt();
    photoAndVideoSetting.videoDelayTime = ui->LineEdit_video_delay->text().toDouble();
    photoAndVideoSetting.videoPhoto_photoTime = ui->LineEdit_video_photo->text().toInt();

    photoAndVideoSetting.photoOrvideo = ui->radioButton_videoMode->isChecked();
    if (photoAndVideoSetting.photoOrvideo) {
        if (ui->radioButton_video_circle->isChecked()) {
            photoAndVideoSetting.workMode = videoCircle;
        }
        else if (ui->radioButton_video_delay->isChecked()) {
            photoAndVideoSetting.workMode = videoDelay;
        }
        else if (ui->radioButton_video_normal->isChecked()) {
            photoAndVideoSetting.workMode = videoNormal;
        }
        else if (ui->radioButton_video_photo->isChecked()) {
            photoAndVideoSetting.workMode = videoPhoto;
        }
        else if (ui->radioButton_video_slowMotion->isChecked()) {
            photoAndVideoSetting.workMode = videoSlowMotion;
        }
        else {
            photoAndVideoSetting.workMode = videoNormal;
        }
    }
    else {
        if (ui->radioButton_photo_continus->isChecked()) {
            photoAndVideoSetting.workMode = photoContinus;
        }
        else if (ui->radioButton_photo_delayed->isChecked()) {
            photoAndVideoSetting.workMode = photoDelayed;
        }
        else if (ui->radioButton_photo_lapse->isChecked()) {
            photoAndVideoSetting.workMode = photoLapse;
        }
        else if (ui->radioButton_photo_multi->isChecked()) {
            photoAndVideoSetting.workMode = photoMulti;
        }
        else if (ui->radioButton_photo_panorama->isChecked()){
            photoAndVideoSetting.workMode = photoPanorama;
        }
        else if (ui->radioButton_photo_singel->isChecked()){
            photoAndVideoSetting.workMode = photoSingel;
        }
        else {
            photoAndVideoSetting.workMode = photoSingel;
        }
    }
    emit cameraSettingChanged(photoAndVideoSetting);

    return photoAndVideoSetting;
}

void PhotoAndVideoDialog::initConnect()
{
//    connect(ui->LineEdit_photo_continus, SIGNAL(ui->LineEdit_photo_continus->editingFinished()), this, SLOT(getSettingFromUi()));
    connect(ui->pushButton_updateSetting, SIGNAL(clicked()), this, SLOT(getSettingFromUi()));
}
