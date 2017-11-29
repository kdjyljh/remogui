#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "imagestreamproc.h"

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

private slots:
    void setLabelPix(const QImage & image);

private slots:
    void on_action_photoAndVideo_triggered();

};

#endif // MAINWINDOW_H
