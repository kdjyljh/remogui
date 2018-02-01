#include "mainwindow.h"
#include <QApplication>
//#include "../thirdparty/Protocol.hpp"
//#include "itemdatadef.h"

#include <glog/logging.h>
//#include <QObject>
//#include <boost/thread.hpp>
//#include <boost/shared_ptr.hpp>

int main(int argc, char *argv[])
{
    google::InitGoogleLogging("remo_gui");
    FLAGS_log_dir = "/tmp/";
    FLAGS_logtostderr = 1;

    init_itemData();

    QApplication a(argc, argv);

//    MainWindow::getWindInstace()->registerSelf2Handler();
    MainWindow::getWindInstace()->show();

    return a.exec();
}
