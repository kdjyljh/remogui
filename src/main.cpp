#include "mainwindow.h"
#include <QApplication>

#include <unordered_map>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include "../thirdparty/Protocol.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow * instance = MainWindow::getWindInstace();
    instance->show();

    return a.exec();
}
