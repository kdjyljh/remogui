#include "mainwindow.h"
#include <QApplication>
#include "../thirdparty/Protocol.hpp"
#include "itemdatadef.h"

#include <glog/logging.h>

int main(int argc, char *argv[])
{
    google::InitGoogleLogging("remo_gui");
    FLAGS_log_dir = "/tmp/";
    FLAGS_logtostderr = 1;

    init_itemData();

    QApplication a(argc, argv);

    MainWindow::getWindInstace()->show();

    return a.exec();
}
