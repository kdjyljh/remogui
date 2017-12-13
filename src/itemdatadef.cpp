#include "itemdatadef.h"

//void iniItemData()
//{

//}

std::vector<ItemData> itemData =
        {
            {
                CmdSet : Remo_CmdSet_Camera,
                CmdId_GetData : Remo_CmdId_Camera_Get_CapDelayTime,
                CmdId_SetData : Remo_CmdId_Camera_Set_CapDelayTime,
                CmdId_GetRange : Remo_CmdId_Camera_Get_CapDelayTime_Range,
                ShowStr : QString::fromUtf8("延时时间"),
                subItemData:
                    {
                        {
                            {Index : CapDelayTime_3S , ShowStr : QString::fromUtf8("3秒")},
                            {Index : CapDelayTime_6S , ShowStr : QString::fromUtf8("6秒")},
                        }
                    }
            },
        };
