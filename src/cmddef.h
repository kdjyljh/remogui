#ifndef CMDEF_H
#define CMDEF_H
typedef enum Remo_CmdSet_E {
    Remo_CmdSet_Universal     = 0x0,
    Remo_CmdSet_Camera        = 0x1,
    Remo_CmdSet_TriopHead     = 0x2,
    Remo_CmdSet_Algorithm     = 0x3,
    Remo_CmdSet_Battery       = 0x4,
    Remo_CmdSet_Interaction   = 0x5,
    Remo_CmdSet_Factory       = 0x6,
    Remo_CmdSet_Bott,
    Remo_CmdSet_PlaceHolder
}Remo_CmdSet_e;


//set类指令返回值
/*
 * 0: 命令执行正确，没有出现错误
 * -1: 命令执行失败，无特殊场景下的通用用返回值
 * -2: 输入的参数值错误
*/

typedef enum Remo_CmdId_SetCmd_ReturnValue_E{
    Return_OK                       = 0,
    Return_NG                       = -1,
    Return_ParamError               = -2,
    Return_Bott,
    Return_PlaceHolder
}Remo_CmdId_SetCmd_ReturnValue_e;


typedef enum Remo_CmdId_Type_E
{
    CmdId_Type_Get              = 0x0,
    CmdId_Type_Set              = 0x1,
    CmdId_Type_GetRange         = 0x2,
}Remo_CmdId_Type_e;
/*
 * cmdset:0x1
 * cmdid:
 * * * * *0x00:获取相机工作模式
 * * * * *0x01:设置相机工作模式
 * * * * *0x02:获取相机支持的工作模式列表
 * * * * *0x03:启动/停止拍照命令
 * * * * *0x04:启动/停止录像命令
 * * * * * * * TYPE=SINGLE 1; PayloadLength = 2
 *
 * * * * *0x10:获取当前拍照分辨率/比例
 * * * * *0x11:设置当前拍照分辨率/比例
 * * * * *0x12:获取拍照分辨率/比例支持的范围
 * * * * *0x13:获取延时拍照的延时时间
 * * * * *0x14:设置延时拍照的延时时间
 * * * * *0x15:获取延时拍照的延时时间支持范围
 * * * * *0x16:获取拍照的场景模式
 * * * * *0x17:设置拍照的场景模式
 * * * * *0x18:获取拍照的场景模式支持范围
 * * * * *0x19:获取拍照HDR模式
 *  * * * 0x1a:设置拍照HDR模式
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
 * * * * *0x200:获取存储卡基本信息
 * * * * *0x201:获取存储卡当前状态
 * * * * *0x202:格式化SD卡
 */
typedef enum Remo_CmdId_E {
    Remo_CmdId_Camera_Get_WorkMode                                      = 0x00,
    Remo_CmdId_Camera_Set_WorkMode                                      = 0x01,
    Remo_CmdId_Camera_Get_WorkMode_Range                                = 0x02,
    Remo_CmdId_Camera_Set_CapOperation                                  = 0x03,
    Remo_CmdId_Camera_Set_RecOperation                                  = 0x04,
    Remo_CmdId_Camera_Get_ImageResolution                               = 0x10,
    Remo_CmdId_Camera_Set_ImageResolution                               = 0x11,
    Remo_CmdId_Camera_Get_ImageResolution_Range                         = 0x12,
    Remo_CmdId_Camera_Get_CapDelayTime                                  = CmdId_Type_Get << 9       | (0x13  & 0x1ff),
    Remo_CmdId_Camera_Set_CapDelayTime                                  = CmdId_Type_Set << 9       | (0x14  & 0x1ff),
    Remo_CmdId_Camera_Get_CapDelayTime_Range                            = CmdId_Type_GetRange << 9  | (0x15  & 0x1ff),
    Remo_CmdId_Camera_Get_CapScene                                      = 0x16,
    Remo_CmdId_Camera_Set_CapScene                                      = 0x17,
    Remo_CmdId_Camera_Get_CapScene_Range                                = 0x18,
    Remo_CmdId_Camera_Get_HDRMode                                       = 0x19,
    Remo_CmdId_Camera_Set_HDRMode                                       = 0x1a,
    Remo_CmdId_Camera_Get_BurstCap                                      = 0x30,
    Remo_CmdId_Camera_Set_BurstCap                                      = 0x31,
    Remo_CmdId_Camera_Get_BurstCap_Range                                = 0x32,
    Remo_CmdId_Camera_Get_ContiCap                                      = 0x33,
    Remo_CmdId_Camera_Set_ContiCap                                      = 0x34,
    Remo_CmdId_Camera_Get_ContiCap_Range                                = 0x35,
    Remo_CmdId_Camera_Get_LapseCap_Interval                             = 0x36,
    Remo_CmdId_Camera_Set_LapseCap_Interval                             = 0x37,
    Remo_CmdId_Camera_Get_LapseCap_Interval_Range                       = 0x38,
    Remo_CmdId_Camera_Get_LapseCap_TotalTime                            = 0x39,
    Remo_CmdId_Camera_Set_LapseCap_TotalTime                            = 0x3a,
    Remo_CmdId_Camera_Get_LapseCap_TotalTime_Range                      = 0x3b,
    Remo_CmdId_Camera_Get_AebCap                                        = 0x3c,
    Remo_CmdId_Camera_Set_AebCap                                        = 0x3d,
    Remo_CmdId_Camera_Get_AebCap_Range                                  = 0x3e,
    Remo_CmdId_Camera_Get_PanoMode                                      = 0x3f,
    Remo_CmdId_Camera_Set_PanoMode                                      = 0x40,
    Remo_CmdId_Camera_Get_PanoMode_Range                                = 0x41,
    Remo_CmdId_Camera_Get_MainVideo_Resolution                          = 0x60,
    Remo_CmdId_Camera_Set_MainVideo_Resolution                          = 0x61,
    Remo_CmdId_Camera_Get_MainVideo_Resolution_Range                    = 0x62,
    Remo_CmdId_Camera_Get_SubVideo_Resolution                           = 0x63,
    Remo_CmdId_Camera_Set_SubVideo_Resolution                           = 0x64,
    Remo_CmdId_Camera_Get_SubVideo_Resolution_Range                     = 0x65,
    Remo_CmdId_Camera_Get_Thumbnail_Resolution                          = 0x66,
    Remo_CmdId_Camera_Set_Thumbnail_Resolution                          = 0x67,
    Remo_CmdId_Camera_Get_Thumbnail_Resolution_Range                    = 0x68,
    Remo_CmdId_Camera_Get_LoopRec_Interval                              = 0x69,
    Remo_CmdId_Camera_Set_LoopRec_Interval                              = 0x6a,
    Remo_CmdId_Camera_Get_LoopRec_Interval_Range                        = 0x6b,
    Remo_CmdId_Camera_Get_LapseRec_Interval                             = 0x6c,
    Remo_CmdId_Camera_Set_LapseRec_Interval                             = 0x6d,
    Remo_CmdId_Camera_Get_LapseRec_Interval_Range                       = 0x6e,
    Remo_CmdId_Camera_Get_LapseRec_TotalTime                            = 0x6f,
    Remo_CmdId_Camera_Set_LapseRec_TotalTime                            = 0x70,
    Remo_CmdId_Camera_Get_LapseRec_TotalTime_Range                      = 0x71,
    Remo_CmdId_Camera_Get_SlowMotion_Resolution                         = 0x72,
    Remo_CmdId_Camera_Set_SlowMotion_Resolution                         = 0x73,
    Remo_CmdId_Camera_Get_SlowMotion_Resolution_Range                   = 0x74,
    Remo_CmdId_Camera_Get_CapStorageType                                = 0xa0,
    Remo_CmdId_Camera_Set_CapStorageType                                = 0xa1,
    Remo_CmdId_Camera_Get_CapStorageType_Range                          = 0xa2,
    Remo_CmdId_Camera_Get_CapStorageQuality                             = 0xa3,
    Remo_CmdId_Camera_Set_CapStorageQuality                             = 0xa4,
    Remo_CmdId_Camera_Get_CapStorageQuality_Range                       = 0xa5,
    Remo_CmdId_Camera_Get_VideoMuxerType                                = 0xa6,
    Remo_CmdId_Camera_Set_VideoMuxerType                                = 0xa7,
    Remo_CmdId_Camera_Get_VideoMuxerType_Range                          = 0xa8,
    Remo_CmdId_Camera_Get_VideoFormat                                   = 0xa9,
    Remo_CmdId_Camera_Set_VideoFormat                                   = 0xaa,
    Remo_CmdId_Camera_Get_PhotoColorType                                = 0xab,
    Remo_CmdId_Camera_Set_PhotoColorType                                = 0xac,
    Remo_CmdId_Camera_Get_PhotoColorType_Range                          = 0xad,
    Remo_CmdId_Camera_Get_WhiteBalance                                  = 0xaf,
    Remo_CmdId_Camera_Set_WhiteBalance                                  = 0xb1,
    Remo_CmdId_Camera_Get_WhiteBalance_Range                            = 0xb2,
    Remo_CmdId_Camera_Get_CustomWB_ColorTemp                            = 0xb3,
    Remo_CmdId_Camera_Set_CustomWB_ColorTemp                            = 0xb4,
    Remo_CmdId_Camera_Get_CustomWB_ColorTemp_Range                      = 0xb5,
    Remo_CmdId_Camera_Get_AeMode                                        = 0xb6,
    Remo_CmdId_Camera_Set_AeMode                                        = 0xb7,
    Remo_CmdId_Camera_Get_AeMode_Range                                  = 0xb8,
    Remo_CmdId_Camera_Get_P_Evbias                                      = 0xb9,
    Remo_CmdId_Camera_Set_P_Evbias                                      = 0xba,
    Remo_CmdId_Camera_Get_S_Shutter                                     = 0xbb,
    Remo_CmdId_Camera_Set_S_Shutter                                     = 0xbc,
    Remo_CmdId_Camera_Get_S_Evbias                                      = 0xbd,
    Remo_CmdId_Camera_Set_S_Evbias                                      = 0xbe,
    Remo_CmdId_Camera_Get_A_Aperture                                    = 0xbf,
    Remo_CmdId_Camera_Set_A_Aperture                                    = 0xc0,
    Remo_CmdId_Camera_Get_A_Evbias                                      = 0xc1,
    Remo_CmdId_Camera_Set_A_Evbias                                      = 0xc2,
    Remo_CmdId_Camera_Get_Current_Aperture                              = 0xc3,
    Remo_CmdId_Camera_Set_Current_Aperture                              = 0xc4,
    Remo_CmdId_Camera_Get_Current_ISO                                   = 0xc5,
    Remo_CmdId_Camera_Get_M_Aperture_Range                              = 0xc6,
    Remo_CmdId_Camera_Get_M_Shutter_Range                               = 0xc7,
    Remo_CmdId_Camera_Get_M_ISO_Range                                   = 0xc8,
    Remo_CmdId_Camera_Get_Current_Evbias                                = 0xc9,
    Remo_CmdId_Camera_Get_Photo_Shutter_Range                           = 0xca,
    Remo_CmdId_Camera_Get_Record_Shutter_Range                          = 0xcb,
    Remo_CmdId_Camera_Get_Photo_ISO_Range                               = 0xcc,
    Remo_CmdId_Camera_Get_Record_ISO_Range                              = 0xcd,
    Remo_CmdId_Camera_Get_Aperture_Range                                = 0xcf,
    Remo_CmdId_Camera_Get_MeterMode                                     = 0xd0,
    Remo_CmdId_Camera_Set_MeterMode                                     = 0xd1,
    Remo_CmdId_Camera_Get_MeterMode_Range                               = 0xd2,
    Remo_CmdId_Camera_Get_Antiflick                                     = 0xd3,
    Remo_CmdId_Camera_Set_Antiflick                                     = 0xd4,
    Remo_CmdId_Camera_Get_Antiflick_Range                               = 0xd5,
    Remo_CmdId_Camera_Get_AELockStatus                                  = 0xd6,
    Remo_CmdId_Camera_Set_AELockStatus                                  = 0xd7,
    Remo_CmdId_Camera_Get_AFMode                                        = 0xe0,
    Remo_CmdId_Camera_Set_AFMode                                        = 0xe1,
    Remo_CmdId_Camera_Get_AFMode_Range                                  = 0xe2,
    Remo_CmdId_Camera_Get_PosArea                                       = 0xe3,
    Remo_CmdId_Camera_Set_AF_CenterPoint                                = 0xe4,
    Remo_CmdId_Camera_Get_AFStatus                                      = 0xe5,
    Remo_CmdId_Camera_Get_AFResult                                      = 0xe6,
    Remo_CmdId_Camera_Get_FocalLengthInfo                               = 0xf0,
    Remo_CmdId_Camera_Set_ZoomControlParam                              = 0xf1,
    Remo_CmdId_Camera_Get_ZoomControlStatus                             = 0xf2,
    Remo_CmdId_Camera_Set_Stop_AF                                       = 0xf3,
    Remo_CmdId_Camera_Get_Sharpness                                     = 0xf8,
    Remo_CmdId_Camera_Set_Sharpness                                     = 0xf9,
    Remo_CmdId_Camera_Get_Rotation                                      = 0xfa,
    Remo_CmdId_Camera_Set_Rotation                                      = 0xfb,
    Remo_CmdId_Camera_Get_Rotation_Range                                = 0xfc,
    Remo_CmdId_Camera_Get_Histogram                                     = 0x100,
    Remo_CmdId_Camera_Get_SDInfo                                        = 0x200,
    Remo_CmdId_Camera_Get_SDStatus                                      = 0x201,
    Remo_CmdId_Camera_Start_SDFormat                                    = 0x202,
    Remo_CmdId_Camera_Bott,
    Remo_CmdId_Camera_PlaceHolder
}Remo_CmdId_e;

#endif // CMDEF_H
