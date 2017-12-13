#ifndef PROTOFIELDDEF_H
#define PROTOFIELDDEF_H

#include <stdint.h>

/*************工作模式控制命令族**************/
/*
 * cmdset:0x1
 * cmdid:
 * * * * *0x00:获取相机工作模式
 * * * * *0x01:设置相机工作模式
 * * * * *0x02:获取相机支持的工作模式列表
 * * * * * * * TYPE=SINGLE 1; PayloadLength = 2
 * * * * *0x03:启动/停止拍照命令
 * * * * *0x04:启动/停止录像命令
 */
typedef struct Remo_Camera_WorkMode_S
{
    uint8_t MainWorkMode;   // 主工作模式
    uint8_t SubWorkMode;    // 从工作模式
}Remo_Camera_WorkMode_s;
typedef enum Remo_Camera_MainWorkMode_E
{
    MainWorkMode_Photo = 0,
    MainWorKMode_MultiPhoto,
    MainWorKMode_Record,
    MainWorKMode_PlayBack,
    MainWorKMode_Bott
}Remo_Camera_MainWorkMode_e;

typedef enum Remo_Camera_SubWorkMode_Photo_E
{
    SubWorkMode_Photo_Single = 0,
    SubWorkMode_Photo_Delay,
    SubWorkMode_Photo_Bott
}Remo_Camera_SubWorkMode_Photo_e;
typedef enum Remo_Camera_WorkMode_MultiPhoto_E
{
    SubWorkMode_MultiPhoto_Burst = 0,
    SubWorkMode_MultiPhoto_Lapse,
    SubWorkMode_MultiPhoto_Continue,
    SubWorkMode_MultiPhoto_Panorama,
    SubWorkMode_MultiPhoto_AEB,
    SubWorkMode_MultiPhoto_Bott

}Remo_Camera_SubWorkMode_MultiPhoto_e;
typedef enum Remo_Camera_WorkMode_Recode_E
{
    SubWorkMode_Recode_Normal = 0,
    SubWorkMode_Recode_Loop,
    SubWcorkMode_Recode_Collapses,
    SubWcorkMode_Recode_Photo,
    SubWcorkMode_Recode_SlowMotion,
    SubWcorkMode_Recode_Bott
}Remo_Camera_SubWorkMode_Recode_e;


typedef struct Remo_Camera_CapOperation_S
{
    uint8_t Operation;   // 操作类型
}Remo_Camera_CapOperation_s;
typedef enum Remo_Camera_CapOperation_E
{
    CapOperation_Start = 0,
    CapOperation_Stop,
    CapOperation_Bott
}Remo_Camera_CapOperation_e;


typedef struct Remo_Camera_RecOperation_S
{
    uint8_t Operation;   // 操作类型
}Remo_Camera_RecOperation_s;
typedef enum Remo_Camera_RecOperation_E
{
    RecOperation_Start = 0,
    RecOperation_Stop,
    RecOperation_Bott
}Remo_Camera_RecOperation_e;
/*************工作模式控制命令族**************/


/*************单拍模式设置命令族*************/
/*
 * cmdSet:0x1
 * cmdId:
 * * * * *0x10:获取当前拍照分辨率/比例
 * * * * *0x11:设置当前拍照分辨率/比例
 * * * * *0x12:获取拍照分辨率/比例支持的范围
 * * * * * * * *PayloadLength = 1; TYPE=SINGLE  TYPE = RANGE_1的组合
 * * * * *0x13:获取延时拍照的延时时间
 * * * * *0x14:设置延时拍照的延时时间
 * * * * *0x15:获取延时拍照的延时时间支持范围
 * * * * * * * *PayloadLength = 1; TYPE=SINGLE  TYPE = RANGE_1的组合
 * * * * *0x16:获取拍照的场景模式
 * * * * *0x17:设置拍照的场景模式
 * * * * *0x18:获取拍照的场景模式支持范围
 *  * * * * * *PayloadLength = 1; TYPE=SINGLE  TYPE = RANGE_1的组合
 * * * * *0x19:获取拍照HDR模式
 *  * * * 0x1a:设置拍照HDR模式
 */

typedef struct Remo_Camera_ImageResolution_S
{
    uint8_t ImageResolution;   // 分辨率
}Remo_Camera_ImageResolution_s ;
typedef enum Remo_Camera_ImageResolution_E
{
    ImageResolution_3840_2160 = 0,
    ImageResolution_4000_3000,
}Remo_Camera_ImageResolution_e;


typedef struct Remo_Camera_CapDelayTime_S
{
    uint8_t CapDelayTime;   // 延时时间
}Remo_Camera_CapDelayTime_s ;
typedef enum Remo_Camera_CapDelayTime_E
{
    CapDelayTime_3S = 0,
    CapDelayTime_6S,
    CapDelayTime_10S,
    CapDelayTime_15S,
    CapDelayTime_20S,
    CapDelayTime_30S,
    CapDelayTime_Bott
}Remo_Camera_CapDelayTime_e;


typedef struct Remo_Camera_CapScene_S
{
    uint8_t CapScene;   // 场景模式
}Remo_Camera_CapScene_s;
typedef enum Remo_Camera_CapScene_E
{
    CapScene_Night = 0,
    CapScene_Dusk,
    CapScene_Bar,
    CapScene_TrainRoom,
    CapScene_Bott
}Remo_Camera_CapScene_e;


typedef struct Remo_Camera_HDRMode_S
{
    uint8_t HDRMode;   // HDR模式
}Remo_Camera_HDRMode_s ;
typedef enum Remo_Camera_HDRMode_E
{
    HDRMode_Off = 0,
    HDRMode_3,
    HDRMode_5,
    HDRMode_Auto,
    HDRMode_Bott
}Remo_Camera_HDRMode_e;
/*************单拍模式设置命令族*************/


/*************多拍模式设置命令族*************/
/*
 * cmdSet:0x1
 * cmdId:
 * * * * *0x30:获取BURST拍照张数
 * * * * *0x31:设置BURST拍照张数
 * * * * *0x32:获取BURST拍照张数支持范围
 * * * * *0x33:获取定时拍照周期
 * * * * *0x34:设置定时拍照周期
 * * * * *0x35:获取定时拍照周期支持范围
 * * * * *0x36:获取LAPSE拍照周期
 * * * * *0x37:设置LAPSE拍照周期
 * * * * *0x38:获取LAPSE拍照周期支持范围
 * * * * *0x39:获取LAPSE拍照持续时间
 * * * * *0x3a:设置LAPSE拍照持续时间
 * * * * *0x3b:获取LAPSE拍照持续时间支持范围
 * * * * *0x3c:获取AEB拍照张数
 * * * * *0x3d:设置AEB拍照张数
 * * * * *0x3e:获取AEB拍照张数支持范围
 * * * * *0xef:获取全景拍照模式
 * * * * *0x40:设置全景拍照模式
 * * * * *0x41:获取全景拍照模式支持的范围
 */

typedef struct Remo_Camera_BurstCap_S
{
    uint8_t BurstCapNum;   // 连拍张数
}Remo_Camera_BurstCap_s;
typedef enum Remo_Camera_BurstCapNum_E
{
    BurstCapNum_3 = 0,
    BurstCapNum_5,
    BurstCapNum_7,
    BurstCapNum_Bott
}Remo_Camera_BurstCapNum_e;

//定时拍照
typedef struct Remo_Camera_ContiCap_S
{
    uint8_t ContiCapInterval;   // 连拍间隔时间
}Remo_Camera_ContiCap_s;
typedef enum Remo_Camera_ContiCapInterval_E
{
    ContiCapInterval_1S = 0,
    ContiCapInterval_3S,
    ContiCapInterval_5S,
    ContiCapInterval_10S,
    ContiCapInterval_20S,
    ContiCapInterval_30S,
    ContiCapInterval_Bott
}Remo_Camera_ContiCapInterval_e;


typedef struct Remo_Camera_LapseCap_Interval_S
{
    uint8_t LapseCapInterval;   // Lapse拍照间隔时间
}Remo_Camera_Lapse_Interval_s ;
typedef enum Remo_Camera_LapseCapInterval_E
{
    LapseCapInterval_0_5S = 0,
    LapseCapInterval_1S,
    LapseCapInterval_2S,
    LapseCapInterval_3S,
    LapseCapInterval_5S,
    LapseCapInterval_8S,
    LapseCapInterval_10S,
    LapseCapInterval_20S,
    LapseCapInterval_30S,
    LapseCapInterval_1MIN,
    LapseCapInterval_2MIN,
    LapseCapInterval_5MIN,
    LapseCapInterval_Bott
}Remo_Camera_LapseCapInterval_e;


typedef struct Remo_Camera_LapseCap_TotalTime_S
{
    uint8_t LapseCapTotalTime;   // Lapse拍照总时间
}Remo_Camera_LapseInterval_TotalTime_s;
typedef enum Remo_Camera_LapseCapTotalTime_E
{
    LapseCapTotalTime_30S = 0,
    LapseCapTotalTime_1MIN,
    LapseCapTotalTime_2MIN,
    LapseCapTotalTime_3MIN,
    LapseCapTotalTime_5MIN,
    LapseCapTotalTime_8MIN,
    LapseCapTotalTime_10MIN,
    LapseCapTotalTime_20MIN,
    LapseCapTotalTime_30MIN,
    LapseCapTotalTime_1H,
    LapseCapTotalTime_2H,
    LapseCapTotalTime_5H,
    LapseCapTotalTime_Infinity,
    LapseCapTotalTime_Bott
}Remo_Camera_LapseCapTotalTime_e;



typedef struct Remo_Camera_AebCap_S
{
    uint8_t AebCapNum;   // Aeb拍照张数
}Remo_Camera_AebCap_s;
typedef enum Remo_Camera_AebCapNum_E
{
    AebCapNum_3 = 0,
    AebCapNum_5,
    AebCapNum_Bott
}Remo_Camera_AebCapNum_e;


typedef struct Remo_Camera_Pano_S
{
    uint8_t PanoMode;   // 全景模式
}Remo_Camera_Pano_s;
typedef enum Remo_Camera_PanoMode_E
{
    PanoMode_180D = 0,
    PanoMode_Selfie_180D,
    PanoMode_360D,
    PanoMode_Manual,
    PanoMode_SuperWideAngle,
    PanoMode_Bott
}Remo_Camera_PanoMode_e;
/*************多拍模式设置命令族*************/



/*************录像模式设置命令族*************/
/*
 * cmdSet:0x1
 * cmdId:
 * * * * *0x60:获取录像主码流分辨率
 * * * * *0x61:设置录像主码流分辨率
 * * * * *0x62:获取录像主码流分辨率支持的范围
 * * * * *0x63:获取实时预览分辨率
 * * * * *0x64:设置实时预览分辨率
 * * * * *0x65:获取实时预览分辨率支持的范围
 * * * * *0x66:获取录像thumbnail分辨率
 * * * * *0x67:设置录像thumbnail分辨率
 * * * * *0x68:获取录像thumbnail分辨率支持的范围
 * * * * *0x69:获取循环录像的周期时间
 * * * * *0x6a:设置循环录像的周期时间
 * * * * *0x6b:获取循环录像的周期时间支持的范围
 * * * * *0x6c:获取缩时摄影的拍摄周期
 * * * * *0x6d:设置缩时摄影的拍摄周期
 * * * * *0x6e:获取缩时摄影的拍摄周期支持范围
 * * * * *0x6f:获取缩时摄影的持续时间
 * * * * *0x70:设置缩时摄影的持续时间
 * * * * *0x71:获取摄影的持续时间支持范围
 * * * * *0x72:获取慢动作回放分辨率
 * * * * *0x73:设置慢动作回放分辨率
 * * * * *0x74:获取慢动作回放分辨率支持的范围
 */
typedef struct Remo_Camera_MainVideo_Resolution_S
{
    uint8_t MainVideo_Resolution;   //分辨率
}Remo_Camera_MainVideo_Resolution_s ;
//帧率根据NTSC/PAL格式选择是/前还是/后的帧率
typedef enum Remo_Camera_MainVideo_Resolution_E
{
    MainVideo_Resolution_3840_2160P__30_25 = 0,
    MainVideo_Resolution_3840_2160P__24,
    MainVideo_Resolution_2704_1520P__30_25,
    MainVideo_Resolution_2704_1520P__24,
    MainVideo_Resolution_1920_1080P__120_100,
    MainVideo_Resolution_1920_1080P__60_50,
    MainVideo_Resolution_1920_1080P__48,
    MainVideo_Resolution_1920_1080P__30_25,
    MainVideo_Resolution_1920_1080P__24,
    MainVideo_Resolution_1280_720P__240_200,
    MainVideo_Resolution_1280_720P__120_100,
    MainVideo_Resolution_1280_720P__60_50,
    MainVideo_Resolution_1280_720P__48,
    MainVideo_Resolution_1280_720P__30_25,
    MainVideo_Resolution_1280_720P__24,
    MainVideo_Resolution_Bott
}Remo_Camera_MainVideo_Resolution_e;


typedef struct Remo_Camera_SubVideo_Resolution_S
{
    uint8_t SubVideo_Resolution;   // 分辨率
} Remo_Camera_SubVideo_Resolution_s;
typedef enum Remo_Camera_SubVideo_Resolution_E
{
    SubVideo_Resolution_1024_576P__30_25,
    SubVideo_Resolution_1024_576P__60_50,
    SubVideo_Resolution_1280_720P__30_25,
    SubVideo_Resolution_1280_720P__60_50,
    SubVideo_Resolution_1920_1080P__30_25,
    SubVideo_Resolution_Bott
}Remo_Camera_SubVideo_Resolution_E;


//相机缩略图预览分辨率
typedef struct Remo_Camera_Thumbnail_Resolution_S
{
    uint8_t ThumbnailResolution;   //分辨率
}Remo_Camera_Thumbnail_Resolution_s;
typedef enum Remo_Camera_Thumbnail_Resolution_E
{
    Thumbnail_Resolution_320_240,
    Thumbnail_Resolution_Bott
}Remo_Camera_Thumbnail_Resolution_e;


typedef struct Remo_Camera_LoopRec_Interval_S
{
    uint8_t LoopRecInterval;   //录像周期
}Remo_Camera_LoopRec_Interval_s;
typedef enum Remo_Camera_LoopRec_Interval_E
{
    LoopRec_Interval_2MIN = 10,
    LoopRec_Interval_5MIN,
    LoopRec_Interval_10MIN,
    LoopRec_Interval_25MIN,
    LoopRec_Interval_Bott
}Remo_Camera_LoopRec_Interval_e;


typedef struct Remo_Camera_LapseRec_Interval_S
{
    uint8_t LapseRecInterval;   //缩时录像的时间间隔
} Remo_Camera_LapseRec_Interval_s;
typedef enum Remo_Camera_LapseRec_Interval_E
{
    LapseRec_Interval_0_5S = 0,
    LapseRec_Interval_1S,
    LapseRec_Interval_2S,
    LapseRec_Interval_3S,
    LapseRec_Interval_5S,
    LapseRec_Interval_8S,
    LapseRec_Interval_10S,
    LapseRec_Interval_20S,
    LapseRec_Interval_30S,
    LapseRec_Interval_1MIN,
    LapseRec_Interval_2MIN,
    LapseRec_Interval_5MIN,
    LapseRec_Interval_Bott
}Remo_Camera_LapseRec_Interval_e;


typedef struct Remo_Camera_LapseRec_TotalTime_S
{
    uint8_t LapseRecTotalTime;   // 缩时录像总时间
}Remo_Camera_LapseRec_TotalTime_s;
typedef enum Remo_Camera_LapseRec_TotalTime_E
{
    LapseRec_TotalTime_30S = 0,
    LapseRec_TotalTime_1MIN,
    LapseRec_TotalTime_2MIN,
    LapseRec_TotalTime_3MIN,
    LapseRec_TotalTime_5MIN,
    LapseRec_TotalTime_8MIN,
    LapseRec_TotalTime_10MIN,
    LapseRec_TotalTime_20MIN,
    LapseRec_TotalTime_30MIN,
    LapseRec_TotalTime_1H,
    LapseRec_TotalTime_2H,
    LapseRec_TotalTime_5H,
    LapseRec_TotalTime_Infinity,
    LapseRec_TotalTime_Bott
}Remo_Camera_LapseRec_TotalTime_e;


//慢动作回放模式分辨率
typedef struct Remo_Camera_SlowMotion_Resolution_S
{
    uint8_t SlowMotion_Resolution;   //分辨率
} Remo_Camera_SlowMotion_Resolution_s;
typedef enum Remo_Camera_SlowMotion_Resolution_E
{
    SlowMotion_Resolution_1280_720P__240_200 = 0,
    SlowMotion_Resolution_1280_720P__120_100,
    SlowMotion_Resolution_19201080P__120_100,
    SlowMotion_Resolution_Bott,
}Remo_Camera_SlowMotion_Resolution_e;
/*************录像模式设置命令族*************/




/*************相机媒体高级设置命令族*************/
/*
 * cmdSet:0x1
 * cmdId:
 * * * * *0xa0:获取相机照片存储格式设置
 * * * * *0xa1:设置相机照片存储格式
 * * * * *0xa2:获取相机照片存储格式支持范围
 * * * * *0xa3:获取相机照片存储质量
 * * * * *0xa4:设置相机照片存储质量
 * * * * *0xa5:获取相机照片存储质量支持范围
 * * * * *0xa6:获取相机视频存储格式设置
 * * * * *0xa7:设置相机视频存储格式
 * * * * *0xa8:获取相机视频存储格式支持范围
 * * * * *0xa9:获取PAL/NTSC相机工作制式设置
 * * * * *0xaa:设置PAL/NTSC相机工作制式
 * * * * *0xab:获取照片色彩模式设置
 * * * * *0xac:设置照片色彩模式
 * * * * *0xad:获取照片色彩模式设置支持范围
 * * * * *0xb0:获取白平衡模式设置
 * * * * *0xb1:设置白平衡模式
 * * * * *0xb2:获取白平衡模式设置支持的范围
 * * * * *0xb3:获取自定义白平衡色温设定值
 * * * * *0xb4:设置自定义白平衡色温设定值
 * * * * *0xb5:获取自定义白平衡色温设定值支持的范围
 * * * * *0xb6:获取曝光模式设置(PSMA)
 * * * * *0xb7:设置曝光模式
 * * * * *0xb8:获取曝光模式设置支持范围
 * * * * *0xb9:获取P档曝光补偿值
 * * * * *0xba:设置P档曝光补偿值
 * * * * *0xbb:获取S档快门设定值
 * * * * *0xbc:设置S档快门设定值
 * * * * *0xbd:获取S档曝光补偿值
 * * * * *0xbe:设置S档曝光补偿值
 * * * * *0xbf:获取A档光圈设定值
 * * * * *0xc0:设置A档光圈设定值
 * * * * *0xc1:获取A档曝光补偿值
 * * * * *0xc2:设置A档曝光补偿值
 * * * * *0xc3:获取当前的光圈设定值
 * * * * *0xc4:获取当前的快门设定值
 * * * * *0xc5:获取当前的ISO设定值
 * * * * *0xc6:获取M档光圈设定值支持的范围
 * * * * *0xc7:获取M档快门设定值支持的范围
 * * * * *0xc8:获取M档ISO设定值支持的范围
 * * * * *0xc9:获取当前档下当前的EV BIAS
 * * * * *0xca:获取曝光补偿设定值支持的范围
 * * * * *0xcb:获取拍照模式下快门设定值支持的范围
 * * * * *0xcc:获取录像模式下快门设定值支持的范围
 * * * * *0xcd:获取拍照模式下ISO设定值支持的范围
 * * * * *0xce:获取录像模式下ISO设定值支持的范围
 * * * * *0xcf:获取光圈设定值支持的范围
 * * * * *0xd0:获取测光模式
 * * * * *0xd1:设置测光模式
 * * * * *0xd2:获取测光模式支持的范围
 * * * * *0xd3:获取抗闪烁模式设置
 * * * * *0xd4:设置抗闪烁模式
 * * * * *0xd5:获取抗闪烁模式支持的范围
 * * * * *0xd6:获取AE LOCK 状态
 * * * * *0xd7:设置 AE LOCK状态
 * * * * *0xe0:获取相机的对焦模式
 * * * * *0xe1:设置相机的对焦模式
 * * * * *0xe2:获取相机的对焦模式支持的范围
 * * * * *0xe3:获取相机对焦位置参数,用于交互上显示对焦窗大小
 * * * * *0xe4:设置相机对焦位置参数启动对焦,设置相机的对焦点中心位置,这个操作会启动单次对焦
 * * * * *0xe5:获取相机对焦状态,获取相机的对焦状态,用于交互提示对焦.这个命令大部分情况下不会使用,一般通过相机推送获得这个状态
 * * * * *0xe6:获取相机对焦结果
 * * * * *0xf0:获取当前相机焦段信息
 * * * * *0xf1:设置相机变焦参数启动变焦
 * * * * *0xf2:获取相机变焦状态信息
 * * * * *0xf3:立即停止相机变焦
 * * * * *0xf8:获取相机锐度参数
 * * * * *0xf9:设置相机的锐度参数
 * * * * *0xfa:获取相机旋转参数
 * * * * *0xfb:设置相机旋转参数命令
 * * * * *0xfc:获取相机旋转参数支持的范围
 * * * * *0x100:获取直方图数据
 */

typedef struct Remo_Camera_CapStorageType_S
{
    uint8_t CapStorageType;
}Remo_Camera_CapSotrageType_s;
typedef enum Remo_Camera_CapStorageType_E
{
    CapStorageType_JPEG = 0,
    CapStorageType_RAW,
    CapStorageType_JPEG_RAW,
    CapStorageType_Bott
}Remo_Camera_CapStorageType_e;


typedef struct Remo_Camera_CapStorageQuality_S
{
    uint8_t QualityLevel;   //照片质量级别
}Remo_Camera_CapStorageQuality_s;
typedef enum Remo_Camera_CapStorageQuality_E
{
    CapStorageQuality_High = 0,
    CapStorageQuality_Medium,
    CapStorageQuality_Low,
    CapStorageQuality_Bott
}Remo_Camera_CapStorageQuality_e;


typedef struct Remo_Camera_VideoMuxerType_S
{
    uint8_t MuxerType;   //视频存储格式类型
}Remo_Camera_VideoMuxerType_s;
typedef enum Remo_Camera_VideoMuxerType_E
{
    VideoMuxerType_MP4 = 0,
    VideoMuxerType_MOV,
    VideoMuxerType_FLV,
    VideoMuxerType_Bott
}Remo_Camera_VideoMuxerType_e;


typedef struct Remo_Camera_VideoFormat_S
{
    uint8_t VideoFormat;   //视频制式类型
} Remo_Camera_VideoFormat_s;
typedef enum Remo_Camera_VideoFormat_E
{
    VideoFormat_PAL,
    VideoFormat_NTSC,
    VideoFormat_Bott
}Remo_Camera_VideoFormat_e;

typedef struct Remo_Camera_PhotoColorType_S
{
    uint8_t ColorType;   //照片色彩类型
} Remo_Camera_PhotoColorType_s;
typedef enum Remo_Camera_PhotoColorType_E
{
    PhotoColorType_Normal = 0,
    PhotoColorType_Binary,
    PhotoColorType_Portrait,
    PhotoColorType_Art,
    PhotoColorType_Dream,
    PhotoColorType_Classical,
    PhotoColorType_Nostalgic,
    PhotoColorType_Bott
}Remo_Camera_PhotoColorType_e;

typedef struct Remo_Camera_WhiteBalance_S
{
    uint8_t WhiteBalanceMode;   //白平衡类型
}Remo_Camera_WhiteBalance_s;
typedef enum Remo_Camera_WhiteBalance_E
{
    WhiteBalance_Auto = 0,
    WhiteBalance_Overcast,
    WhiteBalance_FluorescentLamp,
    WhiteBalance_DayLight,
    WhiteBalance_IncandescentLamp,
    WhiteBalance_Custom,
    WhiteBalance_Bott
}Remo_Camera_WhiteBalance_e;

//支持 2000-10000色温的设定，按照100为一个档位
typedef struct Remo_Camera_CustomWB_ColorTemp_S
{
    uint8_t ColorTemp;   //色温值
}Remo_Camera_CustomWB_ColorTemp_s;


//P为自动档，S为快门优先档，A为光圈优先档，M为手动档
typedef struct Remo_Camera_AeMode_S
{
    uint8_t Mode;   //曝光模式
}Remo_Camera_AeMode_s ;
typedef enum Remo_Camera_AeMode_E
{
    AeMode_Auto = 0,
    AeMode_S,
    AeMode_A,
    AeMode_M,
    AeMode_Bott
}Remo_Camera_AeMode_e;

//曝光补偿值
typedef struct Remo_Camera_Evbias_S
{
    uint8_t BIAS;   //曝光补偿档位
} Remo_Camera_P_Evbias_s;
typedef enum Remo_Camera_Evbias_E
{
    Evbias_Neg_3 = 0,
    Evbias_Neg_2_7,
    Evbias_Neg_2_3,
    Evbias_Neg_2,
    Evbias_Neg_1_7,
    Evbias_Neg_1_3,
    Evbias_Neg_1,
    Evbias_Neg_0_7,
    Evbias_Neg_0_3,
    Evbias_0,
    Evbias_0_3,
    Evbias_0_7,
    Evbias_1,
    Evbias_1_3,
    Evbias_1_7,
    Evbias_2_0,
    Evbias_2_3,
    Evbias_2_7,
    Evbias_3,
    Evbias_Bott,
}Remo_Camera_Evbias_e;


//快门设定值
typedef struct Remo_Camera_Shutter_S
{
    uint8_t ShutterTime;   //快门设定值
}Remo_Camera_Shutter_s;
typedef enum Remo_Camera_Shutter_E
{
    Shutter_1_DividedBy_8000 = 0,
    Shutter_1_DividedBy_6400,
    Shutter_1_DividedBy_5000,
    Shutter_1_DividedBy_4000,
    Shutter_1_DividedBy_3200,
    Shutter_1_DividedBy_2500,
    Shutter_1_DividedBy_2000,
    Shutter_1_DividedBy_1600,
    Shutter_1_DividedBy_1250,
    Shutter_1_DividedBy_1000,
    Shutter_1_DividedBy_800,
    Shutter_1_DividedBy_640,
    Shutter_1_DividedBy_500,
    Shutter_1_DividedBy_400,
    Shutter_1_DividedBy_320,
    Shutter_1_DividedBy_240,
    Shutter_1_DividedBy_200,
    Shutter_1_DividedBy_160,
    Shutter_1_DividedBy_120,
    Shutter_1_DividedBy_100,
    Shutter_1_DividedBy_80,
    Shutter_1_DividedBy_60,
    Shutter_1_DividedBy_50,
    Shutter_1_DividedBy_40,
    Shutter_1_DividedBy_30,
    Shutter_1_DividedBy_25,
    Shutter_1_DividedBy_20,
    Shutter_1_DividedBy_15,
    Shutter_1_DividedBy_12_5,
    Shutter_1_DividedBy_10,
    Shutter_1_DividedBy_8,
    Shutter_1_DividedBy_6_25,
    Shutter_1_DividedBy_5,
    Shutter_1_DividedBy_4,
    Shutter_1_DividedBy_3,
    Shutter_1_DividedBy_2_5,
    Shutter_1_DividedBy_2,
    Shutter_1_DividedBy_1_67,
    Shutter_1_DividedBy_1_25,
    Shutter_1,
    Shutter_1_3,
    Shutter_1_6,
    Shutter_2,
    Shutter_2_5,
    Shutter_3,
    Shutter_3_2,
    Shutter_4,
    Shutter_5,
    Shutter_6,
    Shutter_7,
    Shutter_8,
    Shutter_9,
    Shutter_10,
    Shutter_13,
    Shutter_15,
    Shutter_20,
    Shutter_25,
    Shutter_30,
    Shutter_Bott,
}Remo_Camera_Shutter_e;


//光圈设定值
typedef struct Remo_Camera_Aperture_S
{
    uint8_t Aperture;   //光圈设定值
}Remo_Camera_Aperture_s;
typedef enum Remo_Camera_Aperture_E
{
    Aperture_FNO_1 = 0,
    Aperture_FNO_1_4,
    Aperture_FNO_2,
    Aperture_FNO_2_8,
    Aperture_FNO_4,
    Aperture_Bott,
}Remo_Camera_Aperture_e;


//ISO设定值
typedef struct Remo_Camera_M_ISO_S
{
    uint8_t ISOValue;   //ISO设定值
}Remo_Camera_M_ISO_s;
typedef enum Remo_Camera_ISO_E
{
    ISO_100,
    ISO_200,
    ISO_400,
    ISO_800,
    ISO_1600,
    ISO_3200,
    ISO_6400,
    ISO_12800,
    ISO_25600,
    ISO_Bott
}Remo_Camera_ISO_e;


typedef struct Remo_Camera_MeterMode_S
{
    uint8_t MeterMode;   //测光模式
}Remo_Camera_MeterMode_s;
typedef enum Remo_Camera_MeterMode_E
{
    MeterMode_Global = 0,
    MeterMode_Point,
    MeterMode_Bott
}Remo_Camera_MeterMode_e;


typedef struct Remo_Camera_Antiflick_S
{
    uint8_t AntiflickMode;   //抗闪模式
}Remo_Camera_Antiflick_s;
typedef enum Remo_Camera_Antiflick_E
{
    Antiflick_Off = 0,
    Antiflick_50HZ,
    Antiflick_60HZ,
    Antiflick_Auto,
    Antiflick_Bott
}Remo_Camera_Antiflick_e;


typedef struct Remo_Camera_AELockStatus_S
{
    uint8_t AeLockStatus;   //AELOCK状态
}Remo_Camera_AELockStatus_s;
typedef enum Remo_Camera_AELockStatus_E
{
    AELockStatus_Off = 0,
    AELockStatus_On,
    AELockStatus_Bott
}Remo_Camera_AELockStatus_e;


typedef struct Remo_Camera_AFMode_S
{
    uint8_t AFMode;   //AF模式
}Remo_Camera_AFMode_s;
typedef enum Remo_Camera_AFMode_E
{
    AFMode_MF,
    AFMode_AFS, //单次对焦模式
    AFMode_AFC, //连续对焦模式
    AFMode_Bott
}Remo_Camera_AFMode_e;


//获取对焦位置参数
//位置都通过归一化的坐标表示x,y 取值范围：(0 ~ 1)
typedef struct Remo_Camera_PosPoint_S
{
    float x;         // 归一化的x坐标
    float y;         // 归一化的y坐标
}Remo_Camera_PosPoint_s;

typedef struct Remo_Camera_PosArea_S
{
    Remo_Camera_PosPoint_s Update_Left;  // 左上角
    Remo_Camera_PosPoint_s Down_Right;   // 右下角
}Remo_Camera_PosArea_s;


//相机对焦状态
typedef struct Remo_Camera_AFStatus_S
{
    uint8_t Afstatus;   // AF对焦状态
}Remo_Camera_AFStatus_s;
typedef enum Remo_Camera_AFStatus_E
{
    AFStatus_IDLE_SUCCESS = 0, //表示上次对焦结束并且成功
    AFStatus_IDLE_IDLE_FAILED, //表示上次对焦结束并且失败
    AFStatus_IDLE_FOCUSING,//表示正在对焦
    AFStatus_IDLE_Bott
}Remo_Camera_AFStatus_e;


//相机对焦调试结果
//获取相机的对焦结果，主要用于调试，表示相机目前对焦成功结果以及出错原因
typedef struct Remo_Camera_AFResult_S
{
    uint8_t AfResult;   // AF对焦结果
}Remo_Camera_AFResult_s;
typedef enum Remo_Camera_AFResult_E
{
    AFResult_SinglePeak_SUCCESS = 0,
    AFResult_MultiPeak_SUCCESS = 1,
    AFResult_Undefined = -1,
    AFResult_MultiPeak_Failed = -2,
    AFResult_NoTrendFound = -3,
    AFResult_Bott
}Remo_Camera_AFResult_e;


//相机的变焦控制信息
typedef struct Remo_Camera_FocalLengthInfo_S
{
    uint16_t SpeedLevelNum;   // 变焦速度有多少等级
    uint16_t FocalLengthMin;  // 最小焦段值
    uint16_t FocalLengthMax;  // 最大焦段值
    uint16_t FocalLengthDivideNum;  // 焦段被划分成的分数

}Remo_Camera_FocusLengthInfo_s;
//Max为 35mm等效焦距 *10倍的放大值；FocalLengthDvideNum 将全焦段等分之后进行控制，一般分为64或者 128 或者 256焦段。
typedef enum Remo_Camera_SpeedLevelNum_E
{
    SpeedLevelNum_Lowest = 0,
    SpeedLevelNum_Lower = 1,
    SpeedLevelNum_Medium = -1,
    SpeedLevelNum_Faster = -2,
    SpeedLevelNum_Fastest = -3,
    SpeedLevelNum_Bott
}Remo_Camera_SpeedLevelNum_e;


//相机变焦的target位置和速度
typedef struct Remo_Camera_ZoomControlParam_S
{
    uint8_t ZoomControlType;      // 变焦控制类型
    union {
        struct {
            uint16_t Speed;       // 变焦控制速度
            uint16_t TargetPosNo; // 目标焦段的分段值
        };
        struct {
            uint16_t Speed1;       //  变焦控制速度
            uint16_t  Dir;         //  变焦方向
            uint16_t LastTime;    //  变焦操作持续时间
        };

    };
}Remo_Camera_ZoomControlParam_s;
typedef enum Remo_Camera_ZoomControlType_E
{
    ZoomControlType_AbsPos = 0,
    ZoomControlType_Speed_Continue,
    ZoomControlType_Bott
}Remo_Camera_ZoomControlType_e;


//相机变焦状态
typedef struct Remo_Camera_ZoomControlStatus_S
{
    uint16_t SpeedLevel;   // 变焦速度有多少等级
    uint16_t FocalLengthPosNo; // 当前PosNo位置
    uint16_t FocalLengthValue; // 当前焦距值
    uint16_t State;         // 变焦状态信息

}Remo_Camera_ZoomControlStatus_s;


typedef struct Remo_Camera_Sharpness_S
{
    uint8_t Sharpness;   //锐度
}Remo_Camera_Sharpness_s;
typedef enum Remo_Camera_Sharpness_E
{
    Sharpness_Lowest = 0,
    Sharpness_Lower,
    Sharpness_Medium,
    Sharpness_Higher,
    Sharpness_Highest,
    Sharpness_Bott
}Remo_Camera_Sharpness_e;


typedef struct Remo_Camera_Rotation_S
{
    uint8_t Rotation;   //旋转参数
}Remo_Camera_Rotation_s;
typedef enum Remo_Camera_Rotation_E
{
    Rotation_0D = 0,
    Rotation_90D,
    Rotation_180D,
    Rotation_360D,
    Rotation_Mirror,
    Rotation_90D_Mirror,
    Rotation_180D_Mirror,
    Rotation_270D_Mirror,
    Rotation_Bott
}Remo_Camera_Rotation_e;


//将AE的 5段直方图和256段直方图都上传给交互端，方便用户进行曝光和通透度判断
typedef struct Remo_Camera_Histogram_S
{
    uint32_t Histogram5[5];       // 5 段直方图
    uint32_t Histogram256[256];   // 256段直方图
}Remo_Camera_Histogram_s;



/*************相机媒体高级设置命令族*************/



/*************相机存储功能设置命令族*************/
/*
 *cmdSet:0x1
 *cmdId:
 * * * * *0x200:获取存储卡基本信息
 * * * * *0x201:获取存储卡当前状态
 * * * * *0x202:格式化SD卡
 */
typedef struct Remo_Camera_SDFormat_S
{
    uint8_t FormatType;   // SD卡格式化类型
    uint8_t IsDCFCreate;  // 是否自动创建DCIF目录
}Remo_Camera_SDFormat_s;
typedef enum Remo_Camera_SDFormat_FormatType_E
{
    FormatType_Fast = 0,
    FormatType_LowLevel,
    FormatType_Bott
}Remo_Camera_SDFormat_FormatType_e;
typedef enum Remo_Camera_SDFormat_DCFCreate_E
{
    SDFormat_DCFCreate = 0,
    SDFormat_NotDCFCreate,
    SDFormat_DCFCreate_Bott
}Remo_Camera_SDFormat_DCFCreate_e;



/*************相机存储功能设置命令族*************/


/*************相机回放播放控制命令族*************/
/*
 * cmdSet:0x1
 * cmdId:
 * * * * *0x280:
 */
/*************相机回放播放控制命令族*************/


/*************相机镜头参数相关命令族*************/
/*
 * cmdSet:0x1
 * cmdId:
 * * * * *0x300:
 */
/*************相机镜头参数相关命令族*************/


/*************相机状态推送相关命令族*************/
/*
 * cmdSet:0x1
 * cmdId:
 * * * * *0x360:
 * * * * *0x400:获取WIFI模块的信息
 * * * * *0x401:获取WIFI的工作状态
 * * * * *0x402:设置WIFI的工作状态
 */
/*************相机状态推送相关命令族*************/


/*************相机工厂生产相关命令族*************/
/*
* cmdSet:0x1
* cmdId:
* * * * *0x500:
*/
/*************相机工厂生产相关命令族*************/
#endif // PROTOFIELDDEF_H
