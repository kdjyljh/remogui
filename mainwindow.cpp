#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_photoandvideo.h"

#include <QPixmap>
#include <QHBoxLayout>
#include <QDesktopWidget>
#include <QtConcurrent/QtConcurrent>


const unsigned DEFAULT_WINDOW_WIDTH = 1000;
const unsigned DEFAULT_WINDOW_HEIGHT = 500;
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

    imagProc->init();
    QtConcurrent::run(imagProc, &ImageStreamProc::play);

    connect(imagProc, SIGNAL(imageGot(const QImage&)), this, SLOT(setLabelPix(const QImage&)));

    ui->action_photoAndVideo->triggered();
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

void MainWindow::on_action_photoAndVideo_triggered()
{
    qDebug() << "pandv triggered!!!";
    QDialog *pAndVDialog = new QDialog;
    Ui::PhotoAndVideoDialog *ui_pAndVDialog = new Ui::PhotoAndVideoDialog;
    ui_pAndVDialog->setupUi(pAndVDialog);
//    pAndVDialog->show();
}


