#include "mainwindow.h"
#include <QApplication>
//#include <boost/shared_ptr.hpp>
#include "../thirdparty/Protocol.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow::getWindInstace()->show();

    return a.exec();
}
