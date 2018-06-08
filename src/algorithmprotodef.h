#ifndef REMO_GUI_ALGORITHMPROTODEF_H
#define REMO_GUI_ALGORITHMPROTODEF_H

#include <stdint.h>

#define REMOPROTOCOL_VERSION      0x00    // Version
#define PACKET_SYNC               0xAA    // Sync
// Device
#define DEVICE_CAMERA             0x01    // 相机
#define DEVICE_GIMBAL             0x02    // 云台
#define DEVICE_AI                 0x03    // SDK
#define DEVICE_BATTERY            0x04    // 电池
#define DEVICE_APP                0x05    // APP
#define DEVICE_PC                 0x06    // PC
#define DEVICE_BROADCAST          0x0F    // 广播
// Command Set
#define COMMAND_UNIVERSAL         0x00
#define COMMAND_CAMERA            0x01
#define COMMAND_GIMBAL            0x02
#define COMMAND_AI                0x03
#define COMMAND_BATTERY           0x04
#define COMMAND_INTERACT          0x05
#define COMMAND_FACTORY           0x06
// Packet Type
#define PACKET_REQUEST            0x12    // Request
#define PACKET_REQUEST_NOACK      0x10    // Request with No ack
#define PACKET_ACK                0x11    // Ack
// Error Type
#define ERROR_OK                  0x00    // 成功
#define ERROR_VERSION_INVALID     0x01    // 版本号不匹配
#define ERROR_SENDER_ERROR        0x02    // 发送者错误 (不支持该发送者发送该命令)
#define ERROR_RECIEVER_ERROR      0x03    // 接收者错误
#define ERROR_COMMANDSET_ERROR    0x04    // 命令集错误
#define ERROR_COMMAND_INVALID     0x05    // 命令不能识别
#define ERROR_DATALENGTH_INVALID  0x06    // 数据字段长度不匹配
#define ERROR_DATA_INVALID        0x07    // 数据非法
#define ERROR_NO_AUTHORITY        0x08    // 命令无权限
#define ERROR_COMMAND_NOT_SUPPORT 0x09    // 命令不支持
#define ERROR_COMMAND_DATA_ERROR  0x0A    // 命令参数不支持
#define ERROR_USRLIB_FACECHECK_FAILED     0x0B    // 人脸照片校验失败
#define ERROR_USRLIB_FACE_DUPFOUND        0x0C    // 人脸照片重复
#define ERROR_USRLIB_ID_INVALID           0x0D
#define ERROR_GIMBAL_ABNORMAL     0x0E    // 云台设备异常
#define ERROR_UNKNOWN             0xFF    // 未知错误

// APP -> SDK
#define APP_FUNC_SET_SDK_CFG                                  0x0051
#define APP_FUNC_SET_XOFFSET                                  0x0053
#define APP_FUNC_SET_SPEEDMODE                                0x0055
#define APP_FUNC_SET_ZOOMMODE                                 0x0057
// CB
#define APP_CB_ZOOMCTL_ACTION                                 0x0059
#define APP_CB_ROLLCTL_ACTION                                 0x005A
#define APP_CB_TRIGGER_ACTION                                 0x005C
#define APP_CB_SETARGS                                        0x005E
// Target Selection
#define APP_SELTARGET_SET_SELECTION                           0x0061
#define APP_SELTARGET_SETBY_POINT                             0x0063
#define APP_SELTARGET_SETBY_BBOX                              0x0065
#define APP_SELTARGET_SETBY_DEFAULT                           0x0067
// Status
#define SDKSTATUS_PUSH_APP                                    0x006A
#define APP_FETCH_SDK_STATUS                                  0x006C
// Battery -> SDK
#define KEY_TOGGLE_TARGETS                                    0x0007    /* 電池發送的快捷設置報文 */
// 云台设备外部操作指令
#define SDK_GIMBAL_TRIGGER                                    0x0021
#define SDK_GIMBAL_SET_ANGLE_OR_SPEED                         0x0023
#define SDK_GIMBAL_SET_SPEED_TIME                             0x0025
#define SDK_GIMBAL_SET_SPEED_DEGREE                           0x0027
#define SDK_GIMBAL_CFG                                        0x0029
// ZOOM设备外部操作指令
#define SDK_ZOOM_SET_ZOOM_SPD                                 0x0041
#define SDK_ZOOM_SET_ZOOM_SPDTIME                             0x0043
#define SDK_ZOOM_SET_ZOOM_STEPS                               0x0045
#define SDK_ZOOM_SET_ZOOM_RATIO                               0x0047
#define SDK_ZOOM_STOP_ZOOM                                    0x0049



typedef struct APP_FUNC_SET_SDK_CFG_s {
    uint16_t APP_FUNC_SET_SDK_CFG_Cfg;
    uint8_t APP_FUNC_SET_SDK_CFG_Cmd;
}APP_FUNC_SET_SDK_CFG_S;
typedef enum APP_FUNC_SET_SDK_CFG_Cmd_e {
    APP_FUNC_SET_SDK_CFG_Cmd_Off                                    = 0,
    APP_FUNC_SET_SDK_CFG_Cmd_On                                     = 1,
}APP_FUNC_SET_SDK_CFG_Cmd_E;
typedef enum APP_FUNC_SET_SDK_CFG_Cfg_e {
    APP_FUNC_SET_SDK_CFG_Cfg_SDKEnable                              = 0,
    APP_FUNC_SET_SDK_CFG_Cfg_Capture                                = 1,
    APP_FUNC_SET_SDK_CFG_Cfg_CapSF                                  = 2,
    APP_FUNC_SET_SDK_CFG_Cfg_CapSA                                  = 3,
    APP_FUNC_SET_SDK_CFG_Cfg_CapSL                                  = 4,
    APP_FUNC_SET_SDK_CFG_Cfg_Handpose                               = 5,
    APP_FUNC_SET_SDK_CFG_Cfg_FindMasterInit                         = 6,
    APP_FUNC_SET_SDK_CFG_Cfg_GimbalLock                             = 7,
}APP_FUNC_SET_SDK_CFG_Cfg_E;


typedef struct APP_FUNC_SET_XOFFSET_s {
    uint8_t APP_FUNC_SET_XOFFSET_Offset;
}APP_FUNC_SET_XOFFSET_S;
typedef enum APP_FUNC_SET_XOFFSET_Offset_e {
    APP_FUNC_SET_XOFFSET_Offset_1Divided3                            = -2,
    APP_FUNC_SET_XOFFSET_Offset_5Divided12                           = -1,
    APP_FUNC_SET_XOFFSET_Offset_1Divided2                            = -0,
    APP_FUNC_SET_XOFFSET_Offset_7Divided12                           = 1,
    APP_FUNC_SET_XOFFSET_Offset_2Divided3                            = 2,
}APP_FUNC_SET_XOFFSET_Offset_E;


typedef struct APP_FUNC_SET_SPEEDMODE_s {
    uint8_t APP_FUNC_SET_SPEEDMODE_Mode;
}APP_FUNC_SET_SPEEDMODE_S;
typedef enum APP_FUNC_SET_SPEEDMODE_Mode_e {
    APP_FUNC_SET_SPEEDMODE_Mode_SLOW                                  = 0,
    APP_FUNC_SET_SPEEDMODE_Mode_NORMAL                                = 1,
    APP_FUNC_SET_SPEEDMODE_Mode_FAST                                  = 2,
}APP_FUNC_SET_SPEEDMODE_Mode_E;


typedef struct APP_FUNC_SET_ZOOMMODE_s {
    uint8_t APP_FUNC_SET_ZOOMMODE_Mode;
    uint8_t APP_FUNC_SET_ZOOMMODE_Scale;
}APP_FUNC_SET_ZOOMMODE_S;
typedef enum APP_FUNC_SET_ZOOMMODE_Mode_e {
    APP_FUNC_SET_ZOOMMODE_Mode_NONE                                         = 0,
    APP_FUNC_SET_ZOOMMODE_Mode_NORMAL                                       = 1,
    APP_FUNC_SET_ZOOMMODE_Mode_RANDOM                                       = 2,
    APP_FUNC_SET_ZOOMMODE_Mode_AUTO                                         = 3
}APP_FUNC_SET_ZOOMMODE_Mode_E;
typedef enum APP_FUNC_SET_ZOOMMODE_Scale_e {
    APP_FUNC_SET_ZOOMMODE_Scale_3                                           = 3,
    APP_FUNC_SET_ZOOMMODE_Scale_5                                           = 5,
    APP_FUNC_SET_ZOOMMODE_Scale_7                                           = 7,
    APP_FUNC_SET_ZOOMMODE_Scale_10                                          = 10,
    APP_FUNC_SET_ZOOMMODE_Scale_12                                          = 12,
    APP_FUNC_SET_ZOOMMODE_Scale_15                                          = 15,
}APP_FUNC_SET_ZOOMMODE_Scale_E;


typedef enum APP_SELTARGET_SETBY_DEFAULT_e {
    APP_SELTARGET_SETBY_DEFAULT_Default                                     = 0,
    APP_SELTARGET_SETBY_DEFAULT_Multiple                                    = 1,
    APP_SELTARGET_SETBY_DEFAULT_Master                                      = 2,
}APP_SELTARGET_SETBY_DEFAULT_E;


typedef struct SDKSTATUS_PUSH_APP_s {
    int SDKSTATUS_PUSH_APP_Enable;
    int SDKSTATUS_PUSH_APP_Selection;
    int SDKSTATUS_PUSH_APP_GimbalLock;
    int SDKSTATUS_PUSH_APP_Capture;
    int SDKSTATUS_PUSH_APP_Capture_sf;
    int SDKSTATUS_PUSH_APP_Capture_sa;
    int SDKSTATUS_PUSH_APP_Capture_sl;
    int SDKSTATUS_PUSH_APP_Handpose;
    int SDKSTATUS_PUSH_APP_FindMasterInit;
    int SDKSTATUS_PUSH_APP_SpeedMode;
    int SDKSTATUS_PUSH_APP_XOffset;
    int SDKSTATUS_PUSH_APP_ZoomMode;
    int SDKSTATUS_PUSH_APP_AutoScale;
    int SDKSTATUS_PUSH_APP_ActionType;
    int SDKSTATUS_PUSH_APP_ZoomCtrlType;
    int SDKSTATUS_PUSH_APP_RollCtrlType;
    int SDKSTATUS_PUSH_APP_OnceZoomScale;
    int SDKSTATUS_PUSH_APP_CbZoomSpeed;
    int SDKSTATUS_PUSH_APP_CbCycleZoomPrd;
    int SDKSTATUS_PUSH_APP_CbShotScale;
    int SDKSTATUS_PUSH_APP_CbBiasRollDegree;
    int SDKSTATUS_PUSH_APP_CbCycleBiasRollPrd;
    int SDKSTATUS_PUSH_APP_CbScanScale;
    int SDKSTATUS_PUSH_APP_CbScanSpeed;
    int SDKSTATUS_PUSH_APP_CbSwitchSpeed;
    int SDKSTATUS_PUSH_APP_XOffsetAllowed;
    int SDKSTATUS_PUSH_APP_ZoomModeAllowedRange[10];
    int SDKSTATUS_PUSH_APP_AutoScaleAllowedRange[10];
    int SDKSTATUS_PUSH_APP_ActionTypeAllowedRange[10];
    int SDKSTATUS_PUSH_APP_ZoomCtrlTypeAllowedRange[10];
    int SDKSTATUS_PUSH_APP_RollCtrlTypeAllowedRange[10];
    int SDKSTATUS_PUSH_APP_OnceZoomScaleAllowedRange[10];
}SDKSTATUS_PUSH_APP_S;

#endif //REMO_GUI_ALGORITHMPROTODEF_H
