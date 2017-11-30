#-------------------------------------------------
#
# Project created by QtCreator 2017-11-22T14:33:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = remo_gui
TEMPLATE = app

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


SOURCES += \
    src/photoandvideodialog.cpp \
    src/imagestreamproc.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/camerasetting.cpp

HEADERS += \
    src/photoandvideodialog.h \
    src/imagestreamproc.h \
    src/mainwindow.h \
    src/camerasetting.h

FORMS += \
    ui/mainwindow.ui \
    ui/photoandvideo.ui

RESOURCES += \
    resource/image.qrc
