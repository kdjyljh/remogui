#ifndef PHOTOANDVIDEODIALOG_H
#define PHOTOANDVIDEODIALOG_H
#include "ui_photoandvideo.h"
#include "camerasetting.h"

#include <QDialog>

class PhotoAndVideoDialog : public QDialog
{
Q_OBJECT
public:
    PhotoAndVideoDialog(QWidget *parent = 0);
    ~PhotoAndVideoDialog();

    void closeEvent(QCloseEvent *event) override;

    void setPhotoAndvideoUiBySetting(const PhotoAndVideoSetting & pvSetting);

public slots:
    PhotoAndVideoSetting getSettingFromUi();

signals:
    void cameraSettingChanged(const PhotoAndVideoSetting &pvSetting);

private:
    Ui::PhotoAndVideoDialog *ui; // 来自uic生成的header中的类（photoandvideodialog.ui）
    PhotoAndVideoSetting photoAndVideoSetting;

private slots:
    void initConnect();
};

#endif // PHOTOANDVIDEODIALOG_H
