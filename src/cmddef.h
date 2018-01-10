#ifndef CMDEF_H
#define CMDEF_H
typedef enum Remo_CmdSet_E {
    Remo_CmdSet_Universal     = 0x0,
    Remo_CmdSet_Camera        = 0x1,
    Remo_CmdSet_Gimbal        = 0x2,
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
    CmdId_Type_Control          = 0x3
}Remo_CmdId_Type_e;

//typedef enum Remo_CmdId_Battery_Type_E
//{
//    Battery_Info = 0x00,
//    Charger_Info = 0x01
//}Remo_CmdId_Battery_Type_e;

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
 * * * * *0xe6:获取相机对焦调试结果
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
typedef enum Remo_CmdId_Camera_E {
    Remo_CmdId_Camera_Get_WorkMode                                      = CmdId_Type_Get << 9       | (0x0   & 0x1ff),
    Remo_CmdId_Camera_Set_WorkMode                                      = CmdId_Type_Set << 9       | (0x0   & 0x1ff),
    Remo_CmdId_Camera_Get_WorkMode_Range                                = CmdId_Type_GetRange << 9  | (0x0   & 0x1ff),
    Remo_CmdId_Camera_Set_CapOperation                                  = CmdId_Type_Control << 9   | (0x1   & 0x1ff),
    Remo_CmdId_Camera_Set_RecOperation                                  = CmdId_Type_Control << 9   | (0x2   & 0x1ff),
    Remo_CmdId_Camera_Get_ImageResolution                               = CmdId_Type_Get << 9       | (0x10  & 0x1ff),
    Remo_CmdId_Camera_Set_ImageResolution                               = CmdId_Type_Set << 9       | (0x10  & 0x1ff),
    Remo_CmdId_Camera_Get_ImageResolution_Range                         = CmdId_Type_GetRange << 9  | (0x10  & 0x1ff),
    Remo_CmdId_Camera_Get_CapDelayTime                                  = CmdId_Type_Get << 9       | (0x11  & 0x1ff),
    Remo_CmdId_Camera_Set_CapDelayTime                                  = CmdId_Type_Set << 9       | (0x11  & 0x1ff),
    Remo_CmdId_Camera_Get_CapDelayTime_Range                            = CmdId_Type_GetRange << 9  | (0x11  & 0x1ff),
    Remo_CmdId_Camera_Get_CapScene                                      = CmdId_Type_Get << 9       | (0x12  & 0x1ff),
    Remo_CmdId_Camera_Set_CapScene                                      = CmdId_Type_Set << 9       | (0x12  & 0x1ff),
    Remo_CmdId_Camera_Get_CapScene_Range                                = CmdId_Type_GetRange << 9  | (0x12  & 0x1ff),
    Remo_CmdId_Camera_Get_HDRMode                                       = CmdId_Type_Get << 9       | (0x13  & 0x1ff),
    Remo_CmdId_Camera_Set_HDRMode                                       = CmdId_Type_Set << 9       | (0x13  & 0x1ff),
    Remo_CmdId_Camera_Get_BurstCap                                      = CmdId_Type_Get << 9       | (0x20  & 0x1ff),
    Remo_CmdId_Camera_Set_BurstCap                                      = CmdId_Type_Set << 9       | (0x20  & 0x1ff),
    Remo_CmdId_Camera_Get_BurstCap_Range                                = CmdId_Type_GetRange << 9  | (0x20  & 0x1ff),
    Remo_CmdId_Camera_Get_ContiCap                                      = CmdId_Type_Get << 9       | (0x21  & 0x1ff),
    Remo_CmdId_Camera_Set_ContiCap                                      = CmdId_Type_Set << 9       | (0x21  & 0x1ff),
    Remo_CmdId_Camera_Get_ContiCap_Range                                = CmdId_Type_GetRange << 9  | (0x21  & 0x1ff),
    Remo_CmdId_Camera_Get_LapseCap_Interval                             = CmdId_Type_Get << 9       | (0x22  & 0x1ff),
    Remo_CmdId_Camera_Set_LapseCap_Interval                             = CmdId_Type_Set << 9       | (0x22  & 0x1ff),
    Remo_CmdId_Camera_Get_LapseCap_Interval_Range                       = CmdId_Type_GetRange << 9  | (0x22  & 0x1ff),
    Remo_CmdId_Camera_Get_LapseCap_TotalTime                            = CmdId_Type_Get << 9       | (0x23  & 0x1ff),
    Remo_CmdId_Camera_Set_LapseCap_TotalTime                            = CmdId_Type_Set << 9       | (0x23  & 0x1ff),
    Remo_CmdId_Camera_Get_LapseCap_TotalTime_Range                      = CmdId_Type_GetRange << 9  | (0x23  & 0x1ff),
    Remo_CmdId_Camera_Get_AebCap                                        = CmdId_Type_Get << 9       | (0x24  & 0x1ff),
    Remo_CmdId_Camera_Set_AebCap                                        = CmdId_Type_Set << 9       | (0x24  & 0x1ff),
    Remo_CmdId_Camera_Get_AebCap_Range                                  = CmdId_Type_GetRange << 9  | (0x24  & 0x1ff),
    Remo_CmdId_Camera_Get_PanoMode                                      = CmdId_Type_Get << 9       | (0x25  & 0x1ff),
    Remo_CmdId_Camera_Set_PanoMode                                      = CmdId_Type_Set << 9       | (0x25  & 0x1ff),
    Remo_CmdId_Camera_Get_PanoMode_Range                                = CmdId_Type_GetRange << 9  | (0x25  & 0x1ff),
    Remo_CmdId_Camera_Get_MainVideo_Resolution                          = CmdId_Type_Get << 9       | (0x40  & 0x1ff),
    Remo_CmdId_Camera_Set_MainVideo_Resolution                          = CmdId_Type_Set << 9       | (0x40  & 0x1ff),
    Remo_CmdId_Camera_Get_MainVideo_Resolution_Range                    = CmdId_Type_GetRange << 9  | (0x40  & 0x1ff),
    Remo_CmdId_Camera_Get_SubVideo_Resolution                           = CmdId_Type_Get << 9       | (0x41  & 0x1ff),
    Remo_CmdId_Camera_Set_SubVideo_Resolution                           = CmdId_Type_Set << 9       | (0x41  & 0x1ff),
    Remo_CmdId_Camera_Get_SubVideo_Resolution_Range                     = CmdId_Type_GetRange << 9  | (0x41  & 0x1ff),
    Remo_CmdId_Camera_Get_Thumbnail_Resolution                          = CmdId_Type_Get << 9       | (0x42  & 0x1ff),
    Remo_CmdId_Camera_Set_Thumbnail_Resolution                          = CmdId_Type_Set << 9       | (0x42  & 0x1ff),
    Remo_CmdId_Camera_Get_Thumbnail_Resolution_Range                    = CmdId_Type_GetRange << 9  | (0x42  & 0x1ff),
    Remo_CmdId_Camera_Get_LoopRec_Interval                              = CmdId_Type_Get << 9       | (0x43  & 0x1ff),
    Remo_CmdId_Camera_Set_LoopRec_Interval                              = CmdId_Type_Set << 9       | (0x43  & 0x1ff),
    Remo_CmdId_Camera_Get_LoopRec_Interval_Range                        = CmdId_Type_GetRange << 9  | (0x43  & 0x1ff),
    Remo_CmdId_Camera_Get_LapseRec_Interval                             = CmdId_Type_Get << 9       | (0x44  & 0x1ff),
    Remo_CmdId_Camera_Set_LapseRec_Interval                             = CmdId_Type_Set << 9       | (0x44  & 0x1ff),
    Remo_CmdId_Camera_Get_LapseRec_Interval_Range                       = CmdId_Type_GetRange << 9  | (0x44  & 0x1ff),
    Remo_CmdId_Camera_Get_LapseRec_TotalTime                            = CmdId_Type_Get << 9       | (0x45  & 0x1ff),
    Remo_CmdId_Camera_Set_LapseRec_TotalTime                            = CmdId_Type_Set << 9       | (0x45  & 0x1ff),
    Remo_CmdId_Camera_Get_LapseRec_TotalTime_Range                      = CmdId_Type_GetRange << 9  | (0x45  & 0x1ff),
    Remo_CmdId_Camera_Get_SlowMotion_Resolution                         = CmdId_Type_Get << 9       | (0x46  & 0x1ff),
    Remo_CmdId_Camera_Set_SlowMotion_Resolution                         = CmdId_Type_Set << 9       | (0x46  & 0x1ff),
    Remo_CmdId_Camera_Get_SlowMotion_Resolution_Range                   = CmdId_Type_GetRange << 9  | (0x46  & 0x1ff),
    Remo_CmdId_Camera_Get_CapStorageType                                = CmdId_Type_Get << 9       | (0x60  & 0x1ff),
    Remo_CmdId_Camera_Set_CapStorageType                                = CmdId_Type_Set << 9       | (0x60  & 0x1ff),
    Remo_CmdId_Camera_Get_CapStorageType_Range                          = CmdId_Type_GetRange << 9  | (0x60  & 0x1ff),
    Remo_CmdId_Camera_Get_CapStorageQuality                             = CmdId_Type_Get << 9       | (0x61  & 0x1ff),
    Remo_CmdId_Camera_Set_CapStorageQuality                             = CmdId_Type_Set << 9       | (0x61  & 0x1ff),
    Remo_CmdId_Camera_Get_CapStorageQuality_Range                       = CmdId_Type_GetRange << 9  | (0x61  & 0x1ff),
    Remo_CmdId_Camera_Get_VideoMuxerType                                = CmdId_Type_Get << 9       | (0x62  & 0x1ff),
    Remo_CmdId_Camera_Set_VideoMuxerType                                = CmdId_Type_Set << 9       | (0x62  & 0x1ff),
    Remo_CmdId_Camera_Get_VideoMuxerType_Range                          = CmdId_Type_GetRange << 9  | (0x62  & 0x1ff),
    Remo_CmdId_Camera_Get_VideoFormat                                   = CmdId_Type_Get << 9       | (0x63  & 0x1ff),
    Remo_CmdId_Camera_Set_VideoFormat                                   = CmdId_Type_Set << 9       | (0x63  & 0x1ff),
    Remo_CmdId_Camera_Get_PhotoColorType                                = CmdId_Type_Get << 9       | (0x64  & 0x1ff),
    Remo_CmdId_Camera_Set_PhotoColorType                                = CmdId_Type_Set << 9       | (0x64  & 0x1ff),
    Remo_CmdId_Camera_Get_PhotoColorType_Range                          = CmdId_Type_GetRange << 9  | (0x64  & 0x1ff),
    Remo_CmdId_Camera_Get_WhiteBalance                                  = CmdId_Type_Get << 9       | (0x65  & 0x1ff),
    Remo_CmdId_Camera_Set_WhiteBalance                                  = CmdId_Type_Set << 9       | (0x65  & 0x1ff),
    Remo_CmdId_Camera_Get_WhiteBalance_Range                            = CmdId_Type_GetRange << 9  | (0x65  & 0x1ff),
    Remo_CmdId_Camera_Get_CustomWB_ColorTemp                            = CmdId_Type_Get << 9       | (0x66  & 0x1ff),
    Remo_CmdId_Camera_Set_CustomWB_ColorTemp                            = CmdId_Type_Set << 9       | (0x66  & 0x1ff),
    Remo_CmdId_Camera_Get_CustomWB_ColorTemp_Range                      = CmdId_Type_GetRange << 9  | (0x66  & 0x1ff),
    Remo_CmdId_Camera_Get_AeMode                                        = CmdId_Type_Get << 9       | (0x67  & 0x1ff),
    Remo_CmdId_Camera_Set_AeMode                                        = CmdId_Type_Set << 9       | (0x67  & 0x1ff),
    Remo_CmdId_Camera_Get_AeMode_Range                                  = CmdId_Type_GetRange << 9  | (0x67  & 0x1ff),
    Remo_CmdId_Camera_Get_P_Evbias                                      = CmdId_Type_Get << 9       | (0x68  & 0x1ff),
    Remo_CmdId_Camera_Set_P_Evbias                                      = CmdId_Type_Set << 9       | (0x68  & 0x1ff),
    Remo_CmdId_Camera_Get_S_Shutter                                     = CmdId_Type_Get << 9       | (0x69  & 0x1ff),
    Remo_CmdId_Camera_Set_S_Shutter                                     = CmdId_Type_Set << 9       | (0x69  & 0x1ff),
    Remo_CmdId_Camera_Get_S_Evbias                                      = CmdId_Type_Get << 9       | (0x6a  & 0x1ff),
    Remo_CmdId_Camera_Set_S_Evbias                                      = CmdId_Type_Set << 9       | (0x6a  & 0x1ff),
    Remo_CmdId_Camera_Get_A_Aperture                                    = CmdId_Type_Get << 9       | (0x6b  & 0x1ff),
    Remo_CmdId_Camera_Set_A_Aperture                                    = CmdId_Type_Set << 9       | (0x6b  & 0x1ff),
    Remo_CmdId_Camera_Get_A_Evbias                                      = CmdId_Type_Get << 9       | (0x6c  & 0x1ff),
    Remo_CmdId_Camera_Set_A_Evbias                                      = CmdId_Type_Set << 9       | (0x6c  & 0x1ff),
    Remo_CmdId_Camera_Get_M_Aperture_Range                              = CmdId_Type_Get << 9       | (0x6d  & 0x1ff),
    Remo_CmdId_Camera_Get_M_Shutter_Range                               = CmdId_Type_Get << 9       | (0x6e  & 0x1ff),
    Remo_CmdId_Camera_Get_M_ISO_Range                                   = CmdId_Type_Get << 9       | (0x6f  & 0x1ff),
    Remo_CmdId_Camera_Get_Current_Aperture                              = CmdId_Type_Get << 9       | (0x70  & 0x1ff),
    Remo_CmdId_Camera_Get_Current_Shutter                               = CmdId_Type_Get << 9       | (0x71  & 0x1ff),
    Remo_CmdId_Camera_Get_Current_ISO                                   = CmdId_Type_Get << 9       | (0x72  & 0x1ff),
    Remo_CmdId_Camera_Get_Current_Evbias                                = CmdId_Type_Get << 9       | (0x73  & 0x1ff),
    Remo_CmdId_Camera_Get_Current_Evbias_Range                          = CmdId_Type_Get << 9       | (0x74  & 0x1ff),
    Remo_CmdId_Camera_Get_Current_Shutter_Range                         = CmdId_Type_Get << 9       | (0x75  & 0x1ff),
    Remo_CmdId_Camera_Get_Current_ISO_Range                             = CmdId_Type_Get << 9       | (0x76  & 0x1ff),
    Remo_CmdId_Camera_Get_Current_Aperture_Range                        = CmdId_Type_Get << 9       | (0x77  & 0x1ff),
    Remo_CmdId_Camera_Get_MeterMode                                     = CmdId_Type_Get << 9       | (0x78  & 0x1ff),
    Remo_CmdId_Camera_Set_MeterMode                                     = CmdId_Type_Set << 9       | (0x78  & 0x1ff),
    Remo_CmdId_Camera_Get_MeterMode_Range                               = CmdId_Type_GetRange << 9  | (0x78  & 0x1ff),
    Remo_CmdId_Camera_Get_Antiflick                                     = CmdId_Type_Get << 9       | (0x79  & 0x1ff),
    Remo_CmdId_Camera_Set_Antiflick                                     = CmdId_Type_Set << 9       | (0x79  & 0x1ff),
    Remo_CmdId_Camera_Get_Antiflick_Range                               = CmdId_Type_GetRange << 9  | (0x79  & 0x1ff),
    Remo_CmdId_Camera_Get_AELockStatus                                  = CmdId_Type_Get << 9       | (0x7a  & 0x1ff),
    Remo_CmdId_Camera_Set_AELockStatus                                  = CmdId_Type_Set << 9       | (0x7a  & 0x1ff),
    Remo_CmdId_Camera_Get_AFMode                                        = CmdId_Type_Get << 9       | (0x7b  & 0x1ff),
    Remo_CmdId_Camera_Set_AFMode                                        = CmdId_Type_Set << 9       | (0x7b  & 0x1ff),
    Remo_CmdId_Camera_Get_AFMode_Range                                  = CmdId_Type_GetRange << 9  | (0x7b  & 0x1ff),
    Remo_CmdId_Camera_Get_PosArea                                       = CmdId_Type_Get << 9       | (0x7c  & 0x1ff),
    Remo_CmdId_Camera_Set_PosArea                                       = CmdId_Type_Get << 9       | (0x7c  & 0x1ff),
    Remo_CmdId_Camera_Get_AFStatus                                      = CmdId_Type_Get << 9       | (0x7d  & 0x1ff),
    Remo_CmdId_Camera_Get_AFResult                                      = CmdId_Type_Get << 9       | (0x7e  & 0x1ff),
    Remo_CmdId_Camera_Get_FocalLengthInfo                               = CmdId_Type_Get << 9       | (0x80  & 0x1ff),
    Remo_CmdId_Camera_Get_ZoomControlStatus                             = CmdId_Type_Get << 9       | (0x81  & 0x1ff),
    Remo_CmdId_Camera_Set_ZoomControlParam                              = CmdId_Type_Set << 9       | (0x81  & 0x1ff),
    Remo_CmdId_Camera_Set_Stop_AF                                       = CmdId_Type_Get << 9       | (0x82  & 0x1ff),
    Remo_CmdId_Camera_Get_Sharpness                                     = CmdId_Type_Get << 9       | (0x85  & 0x1ff),
    Remo_CmdId_Camera_Set_Sharpness                                     = CmdId_Type_Set << 9       | (0x85  & 0x1ff),
    Remo_CmdId_Camera_Get_Rotation                                      = CmdId_Type_Get << 9       | (0x86  & 0x1ff),
    Remo_CmdId_Camera_Set_Rotation                                      = CmdId_Type_Set << 9       | (0x86  & 0x1ff),
    Remo_CmdId_Camera_Get_Rotation_Range                                = CmdId_Type_GetRange << 9  | (0x86  & 0x1ff),
    Remo_CmdId_Camera_Get_Histogram                                     = CmdId_Type_Get << 9       | (0x90  & 0x1ff),
    Remo_CmdId_Camera_Get_SDInfo                                        = CmdId_Type_Get << 9       | (0xf0  & 0x1ff),
    Remo_CmdId_Camera_Get_SDStatus                                      = CmdId_Type_Get << 9       | (0xf1  & 0x1ff),
    Remo_CmdId_Camera_Start_SDFormat                                    = CmdId_Type_Control << 9   | (0xf2  & 0x1ff),
    Remo_CmdId_Camera_Get_WifiModuleInfo                                = CmdId_Type_Get << 9       | (0x150 & 0x1ff),
    Remo_CmdId_Camera_Get_WifiWorkStatus                                = CmdId_Type_Get << 9       | (0x151 & 0x1ff),
    Remo_CmdId_Camera_Set_WifiWorkStatus                                = CmdId_Type_Control << 9   | (0x151 & 0x1ff),
    Remo_CmdId_Camera_Switch_WifiWorkMode                               = CmdId_Type_Control << 9   | (0x152 & 0x1ff),
    Remo_CmdId_Camera_FactoryReset                                      = CmdId_Type_Control << 9   | (0x156 & 0x1ff),
    Remo_CmdId_Camera_Bott,
    Remo_CmdId_Camera_PlaceHolder
}Remo_CmdId_Camera_e;



/*云台命令字段定义
 * 0x00 获取云台姿态角（地球坐标系下）
 * 0x01 获取云台角速度（地球坐标系下）
 * 0x02 设置云台绝对姿态角参考值（地球坐标系下）
 * 0x03 设置云台相对姿态角参考值（地球坐标系下）
 * 0x04 设置云台角速度参考值（地球坐标系下）
 * 0x05 设置云台角速度-时间参考值（地球坐标系下）
 * 0x20 Roll轴工作模式（横拍or竖拍）
 * 0x21 Roll轴微调控制
 * 0x22 Yaw，Pitch或Roll轴锁定
 * 0x23 复位至默认值
 * 0x24 速度响应灵敏度控制
 * 0x25 速度响应插值方式
 * 0x26 自动校准模式
 * 0x27 恢复出厂设置
 * 0x40 云台设备信息获取
 */
typedef enum Remo_CmdId__Gimbal_E
{
    Remo_CmdId_Gimbal_Get_AttiAngle                                 = 0x00,
    Remo_CmdId_Gimbal_Get_AnguVelo                                  = 0x01,
    Remo_CmdId_Gimbal_Set_AbsAttiAngle                              = 0x02,
    Remo_CmdId_Gimbal_Set_RelaAttiAngle                             = 0x03,
    Remo_CmdId_Gimbal_Set_AnguVelo                                  = 0x04,
    Remo_CmdId_Gimbal_Set_AnguVeloInter                             = 0x05,
    Remo_CmdId_Gimbal_Set_Roll_WorkMode                             = 0x20,
    Remo_CmdId_Gimbal_Set_RollFineTuning                            = 0x21,
    Remo_CmdId_Gimbal_Set_LockAxis                                  = 0x22,
    Remo_CmdId_Gimbal_Reset                                         = 0x23,
    Remo_CmdId_Gimbal_Set_VeloSlope                                 = 0x24,
    Remo_CmdId_Gimbal_Set_InterPolation                             = 0x25,
    Remo_CmdId_Gimbal_Start_Calibrate                               = 0x26,
    Remo_CmdId_Gimbal_FactoryReset                                  = 0x27,
    Remo_CmdId_Gimbal_Get_DeviceInfo                                = 0x40,
    Remo_CmdId_Gimbal_Bott,
    Remo_CmdId_Gimbal_PlaceHolder
}Remo_CmdId_Gimbal_e;




//电源系统指令
typedef enum Remo_CmdId_Battery_E
{
    Remo_CmdId_Battery_Get_Battery_Info     = 0x01,
    Remo_CmdId_Battery_Get_Charger_Info     = 0x02,
    Remo_CmdId_Battery_Bott,
    Remo_CmdId_Battery_PlaceHolder
}Remo_CmdId_Battery_e;


#endif // CMDEF_H
