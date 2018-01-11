#include "mainwindow.h"
#include <QApplication>
#include "../thirdparty/Protocol.hpp"
#include "itemdatadef.h"

#include <glog/logging.h>
#include <QObject>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include "imagestreamproc.h"

int main(int argc, char *argv[])
{
    google::InitGoogleLogging("remo_gui");
    FLAGS_log_dir = "/tmp/";
    FLAGS_logtostderr = 1;

    init_itemData();

    QApplication a(argc, argv);


//    boost::thread t1(&ImageStreamProc::play, MainWindow::getWindInstace()->imagProc);
    boost::thread t2([](){for (;;) {sleep(1); MainWindow::getWindInstace()->imagProc->readStream_1S();}});

    MainWindow::getWindInstace()->show();

    return a.exec();
}
