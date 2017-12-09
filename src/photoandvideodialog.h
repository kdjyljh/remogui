#ifndef PHOTOANDVIDEODIALOG_H
#define PHOTOANDVIDEODIALOG_H
#include "ui_photoandvideo.h"
#include "camerasetting.h"
#include "cameraworkmode.h"

#include <QDialog>

class PhotoAndVideoDialog : public QDialog, public CameraWorkMode
{
Q_OBJECT
public:
    PhotoAndVideoDialog(QWidget *parent = 0);
    ~PhotoAndVideoDialog();

    void closeEvent(QCloseEvent *event) override;

//    void setPhotoAndvideoUiBySetting(const PhotoAndVideoSetting & pvSetting);

protected:
    virtual void workModeGot(const Remo_Camera_WorkMode_S & workmode) override;

public slots:
//    PhotoAndVideoSetting getSettingFromUi();

signals:
//    void cameraSettingChanged(const PhotoAndVideoSetting &pvSetting);

private:
    Ui::PhotoAndVideoDialog *ui; // 来自uic生成的header中的类（photoandvideodialog.ui）
//    PhotoAndVideoSetting photoAndVideoSetting;

    void getWorkModeFromCamera();
private slots:
    void initConnect();
};

#endif // PHOTOANDVIDEODIALOG_H
