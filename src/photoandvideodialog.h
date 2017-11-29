#ifndef PHOTOANDVIDEODIALOG_H
#define PHOTOANDVIDEODIALOG_H
#include "ui_photoandvideo.h"
#include <QDialog>

enum CameraWorkMode
{
    photoSingel,
    photoMulti,
    photoDelayed,
    photoContinus,
    photoPanorama,
    photoLapse,
    videoNormal,
    videoCircle,
    videoPhoto,
    videoDelay,
    videoSlowMotion
};

class CameraSetting
{
public:
    bool photoOrvideo;//true: video
    enum CameraWorkMode workMode;
    int photoMultiTime;
    int photoDelayedTime;
    int photoContinusTime;
    int photoPanoramaFps;
    double photoLapseIntervalTime;
    int photoLapsecontinueTime;
    int videoCircleTime;
    int videoPhoto_photoTime;
    int videoDelayTime;
};

class PhotoAndVideoDialog : public QDialog
{
Q_OBJECT
public:
    PhotoAndVideoDialog(QWidget *parent = 0);
    ~PhotoAndVideoDialog();

    void closeEvent(QCloseEvent *event) override;

signals:
    void cameraSettingChanged(const CameraSetting &cs);

private:
    Ui::PhotoAndVideoDialog *ui;
    CameraSetting cameraSetting;

private slots:
    void getSettingFromUi();
    void initConnect();
};

#endif // PHOTOANDVIDEODIALOG_H
