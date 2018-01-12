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

    ImageStreamProc *imageProc = new ImageStreamProc;
    boost::thread t1([imageProc](){
        for (;;) {
            imageProc->play();
            usleep(1000);
        }
    });
    boost::thread t2([imageProc](){
        for (;;) {
            imageProc->readStream_1S();
            usleep(1000);
        }
    });

//    MainWindow::getWindInstace()->show();

    pause();

    return a.exec();
}
