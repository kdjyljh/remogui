#ifndef PHOTOANDVIDEODIALOG_H
#define PHOTOANDVIDEODIALOG_H
#include "ui_photoandvideo.h"
#include <QDialog>

enum CameraWorkMode // 拍照/录像的工作模式
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

class CameraSetting // 拍照/录像的工作模式对应的参数
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

class PhotoAndVideoDialog : public QDialog // 和Ui::PhotoAndVideoDialog不同类
{
Q_OBJECT
public:
    PhotoAndVideoDialog(QWidget *parent = 0);
    ~PhotoAndVideoDialog();

    void closeEvent(QCloseEvent *event) override;

signals:
    void cameraSettingChanged(const CameraSetting &cs);

private:
    Ui::PhotoAndVideoDialog *ui; // 来自uic生成的header中的类（photoandvideodialog.ui）
    CameraSetting cameraSetting; // 对话框中的所有设置

private slots:
    void getSettingFromUi();
    void initConnect();
};

#endif // PHOTOANDVIDEODIALOG_H
