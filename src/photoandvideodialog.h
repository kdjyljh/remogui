#ifndef PHOTOANDVIDEODIALOG_H
#define PHOTOANDVIDEODIALOG_H
#include "ui_photoandvideo.h"
//#include "camerasetting.h"
#include "protocoldataInterfaceimpl.h"

#include <QDialog>
#include <QMessageBox>
#include <map>

class PhotoAndVideoDialog : public QDialog, public ProtocolDataInterfaceImpl
{
Q_OBJECT
public:
    PhotoAndVideoDialog(QWidget *parent = 0);
    ~PhotoAndVideoDialog();

    void closeEvent(QCloseEvent *event) override;

//    void setPhotoAndvideoUiBySetting(const PhotoAndVideoSetting & pvSetting);

public slots:
    void readVideoStreamDoneSlot(bool gotStream);

protected:
    virtual void workModeGot(const Remo_Camera_WorkMode_s & workmode) override;
    virtual void settingGot(const std::vector<uint8_t> & data, Remo_CmdId_Camera_e cmdId) override;
    virtual void surportRangeGot(std::set<SubItemData> rangeSet, Remo_CmdId_Camera_e cmdId) override;
    virtual void retProcess(CmdContent cc);

signals:
    void getVideoStreamAgain();
    void photoDelayTickTack(QString showStr);

private:
    Ui::PhotoAndVideoDialog *ui; // 来自uic生成的header中的类（photoandvideodialog.ui
    bool isRecording; //是否正在录影
    bool isPhotoing; //是否正在拍照
    bool recordOrCapture; //目前前是拍照还算录影模式  : true为录影模式
    bool initCompleted;
    bool stopCapRec_Reverse;
    Remo_Camera_WorkMode_s currentWorkMode;

//    PhotoAndVideoSetting photoAndVideoSetting;
    void initSurportRange();
    void setRecVideoByWorkMode(Remo_Camera_WorkMode_s workMode);

    QMessageBox *photoMsgDialog;
    QMessageBox *recordeMsgDialog;
    QAbstractButton *button_stop_recod;
    QAbstractButton *button_stop_photo;
    QMessageBox *waitMsgBox;



//    void getWorkModeFromCamera();
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
    void on_radioButton_SubWcorkMode_Recode_LapseRec_clicked();
//    void on_radioButton_SubWcorkMode_Recode_Photo_clicked();
    void on_radioButton_SubWcorkMode_Recode_SlowMotion_clicked();

//    void on_pushButton_Start_clicked();
//    void on_pushButton_Stop_clicked();
    void on_pushButton_Photo_clicked();
    void on_pushButton_Record_clicked();

    void comboBox_activated(int index);
    void stop_recorde();
    void stop_photo();

    void set_photoMsgDialog_text(QString showStr);
};

#endif // PHOTOANDVIDEODIALOG_H
