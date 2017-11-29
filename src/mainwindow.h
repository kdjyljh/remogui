#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "imagestreamproc.h"
#include "photoandvideodialog.h"

#include <QMainWindow>
#include <QLabel>
#include <QImage>

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

    ImageStreamProc *imagProc;

    QSize resolution;

private slots:
    void setLabelPix(const QImage & image);
    void updateCameraSetting(const CameraSetting &cs);

private slots:
    void on_action_photoAndVideo_triggered();
    void on_all_action_resolution_triggered(bool isChecked);
};

#endif // MAINWINDOW_H
