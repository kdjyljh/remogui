#-------------------------------------------------
#
# Project created by QtCreator 2017-11-22T14:33:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): {
    QT += widgets
    CONFIG += c++11
}

TARGET = remo_gui
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

unix:LIBS += -lavdevice -lavfilter -lavformat  -lavcodec  -lswresample -lswscale -lavutil

unix:LIBS += -lvdpau -lva -lX11 -lva-drm -lva-x11

unix:LIBS += -ldl -pthread -lm

unix:LIBS += -lboost_system -lboost_thread -lboost_filesystem -lboost_regex -lboost_chrono -lboost_date_time -lboost_atomic

unix:LIBS += -lglog

SOURCES += \
    src/photoandvideodialog.cpp \
    src/imagestreamproc.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/camerasetting.cpp \
    thirdparty/CommAsyncUDP.cpp \
    thirdparty/CommGlobal.cpp \
    thirdparty/Protocol.cpp \
    thirdparty/TimedTask.cpp \
    thirdparty/EndpointEnum.cpp \
    thirdparty/Mix.cpp \
    thirdparty/MutexWrap.cpp \
    thirdparty/CommDeviceEnum.cpp \
    thirdparty/CfgVar.cpp \
    thirdparty/CommCmdSetEnum.cpp \
    thirdparty/CommCmdIDEnum.cpp \
    thirdparty/shareddata.cpp \
    src/receivedataproc.cpp \
    src/cmdsender.cpp \
    src/receivedatahandler.cpp \
    src/protocaldatainterface.cpp \
    src/cameraworkmode.cpp

HEADERS += \
    src/photoandvideodialog.h \
    src/imagestreamproc.h \
    src/mainwindow.h \
    src/camerasetting.h \
    thirdparty/shareddata.h \
    src/receivedataproc.h \
    src/camerafielddef.h \
    src/cmdsender.h \
    src/receivedatahandler.h \
    src/protocaldatainterface.h \
    src/cameraworkmode.h \
    src/cmddef.h

FORMS += \
    ui/mainwindow.ui \
    ui/photoandvideo.ui

RESOURCES += \
    resource/image.qrc
