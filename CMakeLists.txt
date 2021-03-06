cmake_minimum_required(VERSION 3.9.6 FATAL_ERROR)
project(remo_gui)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_SOURCE_DIR}/ui)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(UNIX)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -std=c99")
	#不进行编译优化，方便调试
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g  -O0 -Wall -std=c++11 -Werror=return-type")

	set(CMAKE_PREFIX_PATH
			$ENV{HOME}/Qt5.10.0/5.10.0/gcc_64
			${CMAKE_PREFIX_PATH})

	set(remoguiroot $ENV{HOME}/remogui)
endif()

if(WIN32)
	find_package(glog REQUIRED) 
	find_package(protobuf REQUIRED)
	find_package(Qt5 COMPONENTS Core Gui Concurrent Widgets Network Multimedia REQUIRED)
	find_package(Boost COMPONENTS system thread chrono date_time REQUIRED )
	find_package(BZip2)
	link_directories(
			C:/remogui/usr/ffmpeg/lib
			${link_directionies})
	include_directories(
			C:/remogui/usr/ffmpeg/include
			${include_directories})
elseif(UNIX)
	find_package(Qt5 COMPONENTS Core Gui Concurrent Widgets Network REQUIRED)
	find_package(Qt5 COMPONENTS Multimedia) # Multimedia not REQUIRED
	find_package(Protobuf REQUIRED)
	find_package(Boost REQUIRED)
endif()

set(SOURCE_FILES
        src/photoandvideodialog.cpp
        src/imagestreamproc.cpp
        src/main.cpp
        src/mainwindow.cpp
		remoprotocol/CommAsyncUDP.cpp
		remoprotocol/CommGlobal.cpp
		remoprotocol/Protocol.cpp
		remoprotocol/TimedTask.cpp
		remoprotocol/EndpointEnum.cpp
		remoprotocol/Mix.cpp
		remoprotocol/MutexWrap.cpp
		remoprotocol/CommDeviceEnum.cpp
		remoprotocol/CfgVar.cpp
		remoprotocol/CommCmdSetEnum.cpp
		remoprotocol/CommCmdIDEnum.cpp
		remoprotocol/shareddata.cpp
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
		remoprotocol/commlog.cpp
        src/playbackdialog.cpp
        src/mediaview/waterfallscrollarea.cpp
        src/mediaview/waterfallwidget.cpp
        src/mediaview/waterfallitem.cpp
        src/mediaview/HttpProxy.cpp
        src/mediaview/WaterFallImageItem.cpp
        src/mediaview/WaterFallVideoItem.cpp
        src/mediaview/VideoStreamControl.cpp
        src/algorithm/AlgorithmDialog.cpp
        src/algorithm/WorkSpaceWidget.cpp
        src/algorithm/NetImage.cpp
        src/mediaview/ProgressBarSlider.cpp
        src/CameraImageWidget.cpp
        src/MediaStreamProc.cpp
        src/algorithm/AlgorithmPkt.cpp
        src/algorithm/AlgorithmTcpMsgClient.cpp
        src/algorithm/AlgorithmManager.cpp
        src/algorithm/AlgorithmImageWidget.cpp
        src/algorithm/FaceTemplateDialog.cpp
        src/algorithm/FaceTemplateItem.cpp
        src/algorithm/AlgorithmGimbalDialog.cpp
        src/algorithm/AlgorithmZoomSlider.cpp
        src/MainWorkSpaceWidget.cpp
        src/MainImageWidget.cpp
		src/AlgorithmProtoTest.cpp)

#set(HEADERS_FILES
#        src/photoandvideodialog.h
#        src/imagestreamproc.h
#        src/mainwindow.h
#        remoprotocol/shareddata.h
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
#        remoprotocol/commlog.h
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

if(WIN32)
	set(CUSTOM_LIBS
			avdevice avfilter avformat avcodec swresample swscale avutil
			BZip2::BZip2
			glog::glog
			protobuf::libprotobuf Boost::system Boost::thread
			Qt5::Gui Qt5::Concurrent Qt5::Core Qt5::Widgets Qt5::Network Qt5::Multimedia)
elseif(UNIX)
	set(CUSTOM_LIBS
			avdevice avfilter avformat avcodec swresample swscale avutil
			vdpau va X11 va-drm va-x11
			dl pthread m
			boost_system boost_thread boost_filesystem boost_regex boost_chrono boost_date_time boost_atomic
			glog
			protobuf
			Qt5::Gui Qt5::Concurrent Qt5::Core Qt5::Widgets Qt5::Network Qt5::Multimedia)

        if(${Qt5Multimedia_FOUND})
            set(CUSTOM_LIBS ${CUSTOM_LIBS} Qt5::Multimedia)
        endif()
endif()

set(PROTO_FILES
        proto/test.proto
        proto/AlgoParam.proto)

#if(UNIX)
##demo演示笔记本需要
#link_directories(/home/demo/remogui/usr/lib
#        /home/usrlocal/lib
#        ${remoguiroot}/usr/ffmpeg/lib/
#        ${link_directionies})
#include_directories(/home/demo/remogui/usr/include
#        /home/usrlocal/include
#        ${remoguiroot}/usr/ffmpeg/include
#        ${include_directories})
#endif()
include_directories(
		src
		src/algorithm
		src/mediaview
		remoprotocol
		${include_directories})

PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})

add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES} ${RESOURCES_FILES} ${PROTO_SRCS} ${PROTO_HDRS})

target_link_libraries(${CMAKE_PROJECT_NAME} ${CUSTOM_LIBS})

file(COPY json/remo_gui.json DESTINATION ${CMAKE_BINARY_DIR})