#include "photoandvideodialog.h"

#include <QDebug>

unsigned DEFAULT_WITH = 500;
unsigned DEFAULT_HEIGHT = 500;

PhotoAndVideoDialog::PhotoAndVideoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PhotoAndVideoDialog)
{
    ui->setupUi(this);
    setFixedSize(DEFAULT_WITH, DEFAULT_HEIGHT);

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
    emit cameraSettingChanged(cameraSetting);
}

void PhotoAndVideoDialog::getSettingFromUi()
{
    cameraSetting.photoContinusTime = ui->LineEdit_photo_continus->text().toInt();
    cameraSetting.photoDelayedTime = ui->LineEdit_photo_delayed->text().toInt();
    cameraSetting.photoLapsecontinueTime = ui->LineEdit_photo_lapse_continueTime->text().toInt();
    cameraSetting.photoLapseIntervalTime = ui->LineEdit_photo_lapse_intervalTime->text().toDouble();
    cameraSetting.photoMultiTime = ui->LineEdit_photo_multi->text().toInt();
    cameraSetting.photoPanoramaFps = ui->LineEdit_photo_panorama->text().toInt();

    cameraSetting.videoCircleTime = ui->LineEdit_video_circle->text().toInt();
    cameraSetting.videoDelayTime = ui->LineEdit_video_delay->text().toInt();
    cameraSetting.videoPhoto_photoTime = ui->LineEdit_video_photo->text().toInt();

    cameraSetting.photoOrvideo = ui->radioButton_videoMode->isChecked();

    if (ui->radioButton_photo_continus->isChecked()) {
        cameraSetting.workMode = photoContinus;
    }
    else if (ui->radioButton_photo_delayed->isChecked()) {
        cameraSetting.workMode = photoDelayed;
    }
    else if (ui->radioButton_photo_lapse->isChecked()) {
        cameraSetting.workMode = photoLapse;
    }
    else if (ui->radioButton_photo_multi->isChecked()) {
        cameraSetting.workMode = photoMulti;
    }
    else if (ui->radioButton_photo_panorama->isChecked()){
        cameraSetting.workMode = photoPanorama;
    }
    else if (ui->radioButton_photo_singel->isChecked()){
        cameraSetting.workMode = photoSingel;
    }
    else if (ui->radioButton_video_circle->isChecked()) {
        cameraSetting.workMode = videoCircle;
    }
    else if (ui->radioButton_video_delay->isChecked()) {
        cameraSetting.workMode = videoDelay;
    }
    else if (ui->radioButton_video_normal->isChecked()) {
        cameraSetting.workMode = videoNormal;
    }
    else if (ui->radioButton_video_photo->isChecked()) {
        cameraSetting.workMode = videoPhoto;
    }
    else if (ui->radioButton_video_slowMotion->isChecked()) {
        cameraSetting.workMode = videoSlowMotion;
    }

    emit cameraSettingChanged(cameraSetting);
}

void PhotoAndVideoDialog::initConnect()
{
//    connect(ui->LineEdit_photo_continus, SIGNAL(ui->LineEdit_photo_continus->editingFinished()), this, SLOT(getSettingFromUi()));
    connect(ui->pushButton_updateSetting, SIGNAL(clicked()), this, SLOT(getSettingFromUi()));
}
