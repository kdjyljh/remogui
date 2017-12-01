#include "mainwindow.h"
#include <QApplication>

#include <unordered_map>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include "../thirdparty/Protocol.hpp"

static void threadfunc();
int main(int argc, char *argv[])
{
//    boost::thread threadlocal(threadfunc);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

static void threadfunc() {
//    std::unordered_map<std::string, boost::function<void (void)>> mapName2Func;
//    mapName2Func["CAMERA_PING"] = boost::bind(&CommProtoVariables::request_ping, CommProtoVariables::Get(), COMMDEVICE_CAMERA);
//    mapName2Func["TRIOPHEAD_PING"] = boost::bind(&CommProtoVariables::request_ping, CommProtoVariables::Get(), COMMDEVICE_TRIOPHEAD);
//    mapName2Func["PC_PING"] = boost::bind(&CommProtoVariables::request_ping, CommProtoVariables::Get(), COMMDEVICE_PC);
//    mapName2Func["CAMERA_VERSION"] = boost::bind(&CommProtoVariables::request_version, CommProtoVariables::Get(), COMMDEVICE_CAMERA);
//    mapName2Func["TRIOPHEAD_VERSION"] = boost::bind(&CommProtoVariables::request_version, CommProtoVariables::Get(), COMMDEVICE_TRIOPHEAD);
//    mapName2Func["PC_VERSION"] = boost::bind(&CommProtoVariables::request_version, CommProtoVariables::Get(), COMMDEVICE_PC);
//    mapName2Func["CAMERA_STATUS"] = boost::bind(&CommProtoVariables::request_status, CommProtoVariables::Get(), COMMDEVICE_CAMERA);
//    mapName2Func["TRIOPHEAD_STATUS"] = boost::bind(&CommProtoVariables::request_status, CommProtoVariables::Get(), COMMDEVICE_TRIOPHEAD);
//    mapName2Func["PC_STATUS"] = boost::bind(&CommProtoVariables::request_status, CommProtoVariables::Get(), COMMDEVICE_PC);
//    for (auto &&pair : mapName2Func) {
//      int intervalUS = 1e6; // 1s
//      TimedTask::Get()->work_irrevocable(pair.first, pair.second, intervalUS*mapName2Func.size());
//      boost::this_thread::sleep_for(boost::chrono::microseconds(intervalUS));
//    }
}
