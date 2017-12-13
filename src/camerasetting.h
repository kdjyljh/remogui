//#ifndef CAMERASETTING_H
//#define CAMERASETTING_H
//#include <QSize>
//#include <map>
//#include <vector>
//#include <stdint.h>

//enum CameraWorkMode
//{
//    photoSingel,
//    photoMulti,
//    photoDelayed,
//    photoContinus,
//    photoPanorama,
//    photoLapse,
//    videoNormal,
//    videoCircle,
//    videoPhoto,
//    videoDelay,
//    videoSlowMotion
//};

//enum VideoStanderd
//{
//    NTSC,
//    PAL
//};

//enum WhiteBalance
//{
//    whiteBalance_auto,
//    whiteBalance_incandesctLamp,
//    whiteBalance_fluoresctLamp,
//    whiteBalance_daylight,
//    whiteBalance_overcast,
//    whiteBalance_shadow,
//    whiteBalance_dusk
//};

//struct Resolution
//{
//    QSize NTSCSize;
//    int NTSCFps;
//    QSize PALSize;
//    int PALFps;
//};

//struct PhotoAndVideoSetting
//{
//    bool photoOrvideo;//true: video
//    enum CameraWorkMode workMode;
//    int photoMultiTime;
//    int photoDelayedTime;
//    int photoContinusTime;
//    int photoPanoramaFps;
//    double photoLapseIntervalTime;
//    int photoLapsecontinueTime;
//    int videoCircleTime;
//    int videoPhoto_photoTime;
//    double videoDelayTime;
//};

//class CameraSetting
//{
//public:
//    PhotoAndVideoSetting photoAndVideoSetting;
//    Resolution resolution;
//    int resolutionWidth;
//    int resolutionHeight;
//    VideoStanderd videoStanderd;
//    WhiteBalance whiteBalance;
//};

//enum CameraSetDataType
//{
//    CameraSetDataType_setWorkMode,
//    CameraSetDataType_getWorkMode,
//    CameraSetDataType_setValue,
//    CameraSetDataType_setGetValue,
//    CameraSetDataType_custom,
//};

//typedef void*(*CustomDataHandler)(void*);
//struct CameraSetData
//{
//    void * activeItem;
//    void * textItem;

//    std::vector<uint8_t> data;
//    CameraSetDataType dataType;

//    CustomDataHandler customDataHandler;
//};

//extern std::map<uint64_t, CameraSetData> cameraSetMap;

//#endif // CAMERASETTING_H
