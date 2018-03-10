cmake_minimum_required(VERSION 3.9.6 FATAL_ERROR)
project(remo_gui)

set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_SOURCE_DIR}/ui)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -std=c99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11 -Werror=return-type")

find_package(
        Qt5Widgets REQUIRED
        Qt5Gui REQUIRED
        Qt5Concurrent REQUIRED
        Qt5Core REQUIRED
        Boost REQUIRED
)

set(SOURCE_FILES
        src/photoandvideodialog.cpp
        src/imagestreamproc.cpp
        src/main.cpp
        src/mainwindow.cpp
        thirdparty/CommAsyncUDP.cpp
        thirdparty/CommGlobal.cpp
        thirdparty/Protocol.cpp
        thirdparty/TimedTask.cpp
        thirdparty/EndpointEnum.cpp
        thirdparty/Mix.cpp
        thirdparty/MutexWrap.cpp
        thirdparty/CommDeviceEnum.cpp
        thirdparty/CfgVar.cpp
        thirdparty/CommCmdSetEnum.cpp
        thirdparty/CommCmdIDEnum.cpp
        thirdparty/shareddata.cpp
        src/receivedataproc.cpp
        src/receivedatahandler.cpp
        src/protocaldatainterface.cpp
        src/workmodedialog.cpp
        src/itemdatadef.cpp
        src/aemodedialog.cpp
        src/protocoldataInterfaceimpl.cpp
        src/focusdialog.cpp
        src/viewlable.cpp
        src/gimbaldialog.cpp
        src/deviceinfodialog.cpp
        src/batteryinfo.cpp
        src/deviceinfo.cpp
        src/receivedatadispatcher.cpp
        thirdparty/commlog.cpp
        src/playbackdialog.cpp
        src/mediaview/waterfallscrollarea.cpp
        src/mediaview/waterfallwidget.cpp
        src/mediaview/waterfallitem.cpp
        )

#set(HEADERS_FILES
#        src/photoandvideodialog.h
#        src/imagestreamproc.h
#        src/mainwindow.h
#        thirdparty/shareddata.h
#        src/receivedataproc.h
#        src/camerafielddef.h
#        src/receivedatahandler.h
#        src/protocaldatainterface.h
#        src/cmddef.h
#        src/itemdatadef.h
#        src/workmodedialog.h
#        src/aemodedialog.h
#        src/protocoldataInterfaceimpl.h
#        src/focusdialog.h
#        thirdparty/commlog.h
#        src/viewlable.h
#        src/gimbaldialog.h
#        src/deviceinfodialog.h
#        src/batteryinfo.h
#        src/deviceinfo.h
#        src/receivedatadispatcher.h
#        src/playbackdialog.h
#        src/mediaview/waterfallscrollarea.h
#        src/mediaview/waterfallwidget.h
#        src/mediaview/waterfallitem.h
#        )

set(RESOURCES_FILES
        resource/image.qrc)

set(CUSTOM_LIBS
        avdevice avfilter avformat avcodec swresample swscale avutil
        vdpau va X11 va-drm va-x11
        dl pthread m
        boost_system boost_thread boost_filesystem boost_regex boost_chrono boost_date_time boost_atomic
        glog
        Qt5::Gui Qt5Concurrent Qt5::Core Qt5::Widgets)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES} ${RESOURCES_FILES})

target_link_libraries(${CMAKE_PROJECT_NAME} ${CUSTOM_LIBS})

file(COPY remo_gui.json DESTINATION ${CMAKE_BINARY_DIR})