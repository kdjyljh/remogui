#include "mainwindow.h"
#include <QApplication>
#include <glog/logging.h>

int main(int argc, char *argv[])
{
    google::InitGoogleLogging("remo_gui");
    FLAGS_log_dir = "/tmp/";
    FLAGS_logtostderr = 1;

    init_itemData();

    QApplication a(argc, argv);

    if (MainWindow::getWindInstace()) {
        MainWindow::getWindInstace()->show();
    } else {
        return -1;
    }

    return a.exec();
}
