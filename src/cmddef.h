#ifndef CMDEF_H
#define CMDEF_H
typedef enum Remo_CmdSet_E {
  Remo_CmdSet_Universal     = 0x0,
  Remo_CmdSet_Camera        = 0x1,
  Remo_CmdSet_TriopHead     = 0x2,
  Remo_CmdSet_Battery       = 0x3,
  Remo_CmdSet_Interaction   = 0x4,
  Remo_CmdSet_Factory       = 0x5,
}Remo_CmdSet_e;


/*
 * cmdset:0x1
 * cmdid:
 * * * * *0x00:获取相机工作模式
 * * * * *0x01:设置相机工作模式
 * * * * *0x02:获取相机支持的工作模式列表
 * * * * *0x03:启动/停止拍照命令
 * * * * *0x04:启动/停止录像命令
 * * * * * * * TYPE=SINGLE 1; PayloadLength = 2
 */
typedef enum Remo_CmdId_E {
  Remo_CmdId_Camera_Get_WorkMode               = 0x00,
  Remo_CmdId_Camera_Set_WorkMode               = 0x01,
  Remo_CmdId_Camera_Set_WorkMode_List          = 0x02,
}Remo_CmdId_e;

#endif // CMDEF_H
