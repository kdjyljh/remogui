#ifndef PHOTOANDVIDEODIALOG_H
#define PHOTOANDVIDEODIALOG_H
#include "ui_photoandvideo.h"
//#include "camerasetting.h"
#include "cameraworkmode.h"

#include <QDialog>
#include <map>

class PhotoAndVideoDialog : public QDialog, public CameraWorkMode
{
Q_OBJECT
public:
    PhotoAndVideoDialog(QWidget *parent = 0);
    ~PhotoAndVideoDialog();

    void closeEvent(QCloseEvent *event) override;

//    void setPhotoAndvideoUiBySetting(const PhotoAndVideoSetting & pvSetting);

protected:
    virtual void workModeGot(const Remo_Camera_WorkMode_s & workmode) override;
//    virtual void capDelayTimeGot(const int & value) override;
    virtual void cameraSettingGot(const std::vector<uint8_t> & data, Remo_CmdId_e cmdId) override;
    virtual void surportRangeGot(std::set<SubItemData> rangeSet, Remo_CmdId_e cmdId) override;

public slots:
//    PhotoAndVideoSetting getSettingFromUi();

signals:
//    void cameraSettingChanged(const PhotoAndVideoSetting &pvSetting);

private:
    Ui::PhotoAndVideoDialog *ui; // 来自uic生成的header中的类（photoandvideodialog.ui）
//    PhotoAndVideoSetting photoAndVideoSetting;
    void initSurportRange();

    void getWorkModeFromCamera();
private slots:
    void on_radioButton_SubWorkMode_Photo_Single_clicked();
    void on_radioButton_SubWorkMode_Photo_Delay_clicked();
    void on_radioButton_SubWorkMode_MultiPhoto_Burst_clicked();
    void on_radioButton_SubWorkMode_MultiPhoto_Lapse_clicked();
    void on_radioButton_SubWorkMode_MultiPhoto_Continue_clicked();
    void on_radioButton_SubWorkMode_MultiPhoto_Panorama_clicked();
//    void on_radioButton_SubWorkMode_MultiPhoto_AEB_clicked();
    void on_radioButton_SubWorkMode_Recode_Normal_clicked();
    void on_radioButton_SubWorkMode_Recode_Loop_clicked();
    void on_radioButton_SubWcorkMode_Recode_Collapses_clicked();
    void on_radioButton_SubWcorkMode_Recode_Photo_clicked();
    void on_radioButton_SubWcorkMode_Recode_SlowMotion_clicked();

    void on_pushButton_Start_clicked();
    void on_pushButton_Stop_clicked();

    void on_ComboBox_SubWorkMode_Photo_Delay_activated(int index);
};

#endif // PHOTOANDVIDEODIALOG_H
