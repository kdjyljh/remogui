#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "imagestreamproc.h"
#include "photoandvideodialog.h"
#include "camerasetting.h"

#include <QMainWindow>
#include <QLabel>
#include <QImage>
#include <QActionGroup>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QLabel *viewLable;
    QLayout *mainLayout;
    PhotoAndVideoDialog *photoAndVideoDialog;

    QActionGroup *actionGroupResolution;
    QActionGroup *actionGroupVideoStandard;
    QActionGroup *actionGroupWhiteBalance;
    QActionGroup *actionGroupExposure;

private:
    ImageStreamProc *imagProc;
    CameraSetting *cameraSetting;

private:
    void setupAction();

private slots:
    void setLabelPix(const QImage & image);
    void updatePhotoAndVideoSetting(const PhotoAndVideoSetting & vcSetting);

private slots:
    //auto connect slots
    void on_action_photoAndVideo_triggered();

    //slots need connect by code
    void actionGroup_resolution_triggered(QAction *action);
    void actionGroup_videoStandard_triggered(QAction *action);
    void actionGroup_WhiteBalance_triggered(QAction *action);
    void actionGroup_exposure_triggered(QAction *action);
};

#endif // MAINWINDOW_H
