#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPixmap>
#include <QHBoxLayout>
#include <QDesktopWidget>
#include <QtConcurrent/QtConcurrent>
#include <QDebug>


const unsigned DEFAULT_WINDOW_WIDTH = 1000;
const unsigned DEFAULT_WINDOW_HEIGHT = 500;

static bool checkMutexAction(QMenu *menu, QAction *action)
{
    if (!menu || !action) {
        return false;
    }

    QList<QAction*> actionList = menu->actions();
    if (!actionList.contains(action)) {
        return false;
    }

    for (int i = 0; i < actionList.size(); ++i) {
        if (action == actionList.at(i)) {
            action->setChecked(true);
        }
        else {
            actionList.at(i)->setChecked(false);
        }
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mainLayout(new QHBoxLayout(this)),
    viewLable(new QLabel(this)),
    imagProc(new ImageStreamProc)
{
    ui->setupUi(this);

    QWidget *cw = new QWidget(this);
    setCentralWidget(cw);
    cw->setLayout(mainLayout);
    mainLayout->addWidget(viewLable);

//    setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    QRect screenRect = QApplication::desktop()->screenGeometry();
    move(screenRect.width() / 2 - width() / 2, screenRect.height() / 2 - height() / 2);

//    imagProc->init();
//    QtConcurrent::run(imagProc, &ImageStreamProc::play);

    connect(imagProc, SIGNAL(imageGot(const QImage&)), this, SLOT(setLabelPix(const QImage&)));

    connect(ui->action_resolution_2_7Kp24_2_7Kp24, SIGNAL(triggered(bool)), this, SLOT(on_all_action_resolution_triggered(bool)));
}

MainWindow::~MainWindow()
{
    imagProc->thread()->terminate();
    delete imagProc;
    delete viewLable;
    delete mainLayout;
    delete imagProc;

    delete ui;
}

void MainWindow::setLabelPix(const QImage &image)
{
    QPixmap pix = QPixmap::fromImage(image);
    viewLable->setPixmap(pix);
}

void MainWindow::updateCameraSetting(const CameraSetting &cs)
{
    qDebug() << "get camera setting changed signal cs.photoContinusTime:" << cs.photoContinusTime;
}

void MainWindow::on_action_photoAndVideo_triggered()
{
    PhotoAndVideoDialog *dialog = new PhotoAndVideoDialog(this);
    connect(dialog, SIGNAL(cameraSettingChanged(const CameraSetting &)), this, SLOT(updateCameraSetting(const CameraSetting &)));
    dialog->show();
}

void MainWindow::on_all_action_resolution_triggered(bool isChecked)
{

}


