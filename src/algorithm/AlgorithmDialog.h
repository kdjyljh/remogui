//
// Created by jianghualuo on 18-3-20.
//

#ifndef REMO_GUI_ALGORITHMDIALOG_H
#define REMO_GUI_ALGORITHMDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QLabel>
#include <QPixmap>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <remo_gui_autogen/include/ui_gimbal.h>
#include "WorkSpaceWidget.h"
#include "AlgorithmManager.h"
#include "FaceTemplateDialog.h"
#include "AlgorithmGimbalDialog.h"
#include "AlgorithmZoomSlider.h"
#include "ui_algorithm.h"
#include <QMediaPlayer>

class FaceTemplateDialog;

class AlgorithmDialog : public QDialog {
    Q_OBJECT
public:
    ~AlgorithmDialog();
    bool isValid() {return initialized;}
    bool init();
    static boost::shared_ptr<AlgorithmDialog> getInstance();
    bool asyncSendMsg(const AlgoParamMsg &msg);
    bool syncSendMsg(const AlgoParamMsg &msgSend, AlgoParamMsg &msgRet, int timeOut = 5000);
    bool syncSendMsg(const AlgoParamMsg &msgSend,
                     boost::function<void (const AlgoParamMsg &msgSend, void*)> callback, int timeOut = 5000);

protected:
    virtual void resizeEvent(QResizeEvent *event) override;

private slots:
//    void on_pushButton_person_selection_action_clicked();
//    void on_comboBox_person_selection_activated(int index);
    void on_groupBox_person_chose_clicked(bool checked);
    void on_radioButton_person_chose_master_toggled(bool checked);
    void on_pushButton_faceTemplate_clicked();
    void on_pushButton_gimbal_clicked();
//    void on_pushButton_zoom_clicked();
    void on_pushButton_music_play_clicked();

private slots:
    void onMsgGot(int msgType);
    void checkBox_capture_stateChanged(int state);
    void radioButton_person_chose_clicked(bool checked);
    void sendManuallyChosing(double x, double y);
    void pushButton_zoom_clicked();
    void pushButton_specialShot_clicked();

private:
    void updateSelectionUi();
    void updateVersionUi();
    void updateCaptureUi();
    void updateControl();
    void updateFaceTemplateUi();
    void updateZoomModeUi();
    void updateSpecialShotUi();
    AlgorithmDialog(QWidget *parent = nullptr);

private:
    Ui::Algorithm *ui;
    bool initialized;
    bool isSpecialShotting;
    boost::shared_ptr<AlgorithmManager> manager;
    boost::shared_ptr<FaceTemplateDialog> faceTemplateDialog;
    boost::shared_ptr<AlgorithmGimbalDialog> gimbalDialog;
    boost::shared_ptr<AlgorithmZoomSlider> zoomSlider;
    boost::shared_ptr<QMediaPlayer> player;
};


#endif //REMO_GUI_ALGORITHMDIALOG_H
