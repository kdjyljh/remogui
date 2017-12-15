#include "itemdatadef.h"
#include <boost/bind.hpp>

ItemData constructItemData(int cmdSet, int cmdid_get, int cmdid_set, int cmdid_getRange, std::string mainShowStr, std::set<SubItemData> subData)
{
    return ItemData
    {
        CmdSet : cmdSet,
        CmdId_GetData : cmdid_get,
        CmdId_SetData : cmdid_set,
        CmdId_GetRange : cmdid_getRange,
        ShowStr : mainShowStr,
        subItemData: subData
    };
}

//Remo_CmdId_Camera_Bott 表示无数据
auto constructItemData_Camera_ = [](int id_get = Remo_CmdId_Camera_Bott, int id_set = Remo_CmdId_Camera_Bott,
    int id_getRange = Remo_CmdId_Camera_Bott, std::string mainShowStr = "", std::set<SubItemData> subData = std::set<SubItemData>()) {
    return constructItemData(Remo_CmdSet_Camera, id_get, id_set, id_getRange, mainShowStr, subData);
};

auto constructItemData_Camera_CmdId_Set_Required_ = [](int id_set) {
    return constructItemData_Camera_(Remo_CmdId_Camera_Bott, id_set);
};

std::vector<ItemData> itemData;
//        {
//            {
//                CmdSet : Remo_CmdSet_Camera,
//                CmdId_GetData : Remo_CmdId_Camera_Get_CapDelayTime,
//                CmdId_SetData : Remo_CmdId_Camera_Set_CapDelayTime,
//                CmdId_GetRange : Remo_CmdId_Camera_Get_CapDelayTime_Range,
//                ShowStr : QString::fromUtf8("延时时间"),
//                subItemData:
//                    {
//                        {
//                            {Index : CapDelayTime_3S , ShowStr : "3秒"},
//                            {Index : CapDelayTime_6S , ShowStr : "6秒"},
//                        }
//                    }
//            },
//        };

void init_itemData()
{
    itemData.push_back(constructItemData_Camera_(
                           Remo_CmdId_Camera_Get_WorkMode, Remo_CmdId_Camera_Set_WorkMode, Remo_CmdId_Camera_Get_WorkMode_Range, "工作模式"));
    itemData.push_back(constructItemData_Camera_CmdId_Set_Required_(Remo_CmdId_Camera_Set_CapOperation));
    itemData.push_back(constructItemData_Camera_CmdId_Set_Required_(Remo_CmdId_Camera_Set_RecOperation));
    itemData.push_back(constructItemData_Camera_(
                           Remo_CmdId_Camera_Get_ImageResolution, Remo_CmdId_Camera_Set_ImageResolution, Remo_CmdId_Camera_Get_ImageResolution_Range, "图片分辨率",
                           {{Index : ImageResolution_3840_2160 , ShowStr : "3840x2160"},
                            {Index : ImageResolution_4000_3000 , ShowStr : "4000x3000"},}));
    itemData.push_back(constructItemData_Camera_(
                           Remo_CmdId_Camera_Get_CapDelayTime, Remo_CmdId_Camera_Set_CapDelayTime, Remo_CmdId_Camera_Get_CapDelayTime_Range, "延时时间",
                            {{CapDelayTime_3S , "3秒"}, {CapDelayTime_6S , "6秒"},{CapDelayTime_10S, "10秒"},
                            {CapDelayTime_15S, "15秒"}, {CapDelayTime_20S, "20秒"}, {CapDelayTime_30S, "30秒"}}));
    itemData.push_back(constructItemData_Camera_(
                           Remo_CmdId_Camera_Get_CapScene, Remo_CmdId_Camera_Set_CapScene, Remo_CmdId_Camera_Get_CapScene_Range, "照相场景",
                            {{CapScene_Night , "夜景"}, {CapScene_Dusk , "黄昏"}, {CapScene_Bar, "酒吧"}, {CapScene_TrainRoom, "训练室"}}));
    itemData.push_back(constructItemData_Camera_(
                           Remo_CmdId_Camera_Get_HDRMode, Remo_CmdId_Camera_Set_HDRMode, Remo_CmdId_Camera_Bott, "HDR",
                            {{HDRMode_Off , "关闭"}, {HDRMode_3 , "3张合成"}, {HDRMode_5, "5张合成"}, {HDRMode_Auto, "自动"}}));

    itemData.push_back(constructItemData_Camera_(
                           Remo_CmdId_Camera_Get_BurstCap, Remo_CmdId_Camera_Set_BurstCap, Remo_CmdId_Camera_Get_BurstCap_Range, "连拍张数",
                            {{BurstCapNum_3 , "3张"}, {BurstCapNum_5 , "5张"}, {BurstCapNum_7, "7张"}}));
    itemData.push_back(constructItemData_Camera_(
                           Remo_CmdId_Camera_Get_ContiCap, Remo_CmdId_Camera_Set_ContiCap, Remo_CmdId_Camera_Get_ContiCap_Range, "连拍间隔",
                            {{ContiCapInterval_1S , "1秒"}, {ContiCapInterval_3S , "3秒"}, {ContiCapInterval_5S, "5秒"},
                             {ContiCapInterval_10S, "10秒"}, {ContiCapInterval_20S, "20秒"}, {ContiCapInterval_30S, "30秒"}}));
    itemData.push_back(constructItemData_Camera_(
                           Remo_CmdId_Camera_Get_LapseCap_Interval, Remo_CmdId_Camera_Set_LapseCap_Interval, Remo_CmdId_Camera_Get_LapseCap_Interval_Range, "Lapse拍照间隔",
                            {{LapseCapInterval_0_5S , "0.5秒"}, {LapseCapInterval_1S , "1秒"}, {LapseCapInterval_2S, "2秒"},
                             {LapseCapInterval_3S , "3秒"}, {LapseCapInterval_5S , "5秒"}, {LapseCapInterval_8S, "8秒"},
                             {LapseCapInterval_10S , "秒"}, {LapseCapInterval_20S , "秒"}, {LapseCapInterval_30S, "秒"},
                             {LapseCapInterval_1MIN , "1分钟"}, {LapseCapInterval_2MIN , "2分钟"}, {LapseCapInterval_5MIN, "5分钟"}}));
    itemData.push_back(constructItemData_Camera_(
                           Remo_CmdId_Camera_Get_LapseCap_TotalTime, Remo_CmdId_Camera_Set_LapseCap_TotalTime, Remo_CmdId_Camera_Get_LapseCap_TotalTime_Range, "Lapse拍照总时间",
                            {{LapseCapTotalTime_30S , "30秒"}, {LapseCapTotalTime_1MIN , "1分钟"}, {LapseCapTotalTime_2MIN, "2分钟"},
                             {LapseCapTotalTime_3MIN , "3分钟"}, {LapseCapTotalTime_5MIN , "5分钟"}, {LapseCapTotalTime_8MIN, "8分钟"},
                             {LapseCapTotalTime_10MIN , "10分钟"}, {LapseCapTotalTime_20MIN , "20分钟"}, {LapseCapTotalTime_30MIN, "30分钟"},
                             {LapseCapTotalTime_1H , "1小时"}, {LapseCapTotalTime_2H , "2小时"}, {LapseCapTotalTime_5H, "5小时"},
                             {LapseCapTotalTime_Infinity , "无限大"}}));
    itemData.push_back(constructItemData_Camera_(
                           Remo_CmdId_Camera_Get_AebCap, Remo_CmdId_Camera_Set_AebCap, Remo_CmdId_Camera_Get_AebCap_Range, "Aeb拍照张数",
                            {{AebCapNum_3 , "3张"}, {AebCapNum_5 , "5张"}}));



    itemData.push_back(constructItemData_Camera_(
                           Remo_CmdId_Camera_Get_PanoMode, Remo_CmdId_Camera_Set_HDRMode, Remo_CmdId_Camera_Get_PanoMode_Range, "全景拍照角度",
                            {{PanoMode_180D , "180度"}, {PanoMode_Selfie_180D , "180度自拍"}, {PanoMode_360D, "360度"},
                             {PanoMode_Manual, "手动"}, {PanoMode_SuperWideAngle, "超广角"}}));
    itemData.push_back(constructItemData_Camera_(
                           Remo_CmdId_Camera_Get_MainVideo_Resolution, Remo_CmdId_Camera_Set_MainVideo_Resolution, Remo_CmdId_Camera_Get_MainVideo_Resolution_Range, "录像主码流分辨率",
                            {{MainVideo_Resolution_3840_2160P__30_25 , "3840*2160P30/25"}, {MainVideo_Resolution_3840_2160P__24 , "3840*2160P24"},
                             {MainVideo_Resolution_2704_1520P__30_25 , "2704*1520P30/25"}, {MainVideo_Resolution_2704_1520P__24 , "2704*1520P24"},
                             {MainVideo_Resolution_1920_1080P__120_100 , "1920*1080P120/100"}, {MainVideo_Resolution_1920_1080P__60_50 , "1920*1080P60/50"},
                             {MainVideo_Resolution_1920_1080P__48 , "1920*1080P48"}, {MainVideo_Resolution_1920_1080P__30_25 , "1920*1080P30/25"},
                             {MainVideo_Resolution_1920_1080P__24 , "1920*1080P24"}, {MainVideo_Resolution_1280_720P__240_200 , "1280*720P240/200"},
                             {MainVideo_Resolution_1280_720P__120_100 , "1280*720P120/100"}, {MainVideo_Resolution_1280_720P__60_50 , "1280*720P60/50"},
                             {MainVideo_Resolution_1280_720P__48 , "1280*720P48"}, {MainVideo_Resolution_1280_720P__30_25 , "1280*720P30/25"},
                             {MainVideo_Resolution_1280_720P__24 , "1280*720P24"}}));
    itemData.push_back(constructItemData_Camera_(
                           Remo_CmdId_Camera_Get_SubVideo_Resolution, Remo_CmdId_Camera_Set_SubVideo_Resolution, Remo_CmdId_Camera_Get_SubVideo_Resolution_Range, "实时预览分辨率",
                            {{SubVideo_Resolution_1024_576P__30_25 , "1024*576P30/25"}, {SubVideo_Resolution_1024_576P__60_50 , "1024*576P60/50"},
                             {SubVideo_Resolution_1280_720P__30_25, "1280*720P30/25"}, {SubVideo_Resolution_1280_720P__60_50, "1280*720P60/50"},
                             {SubVideo_Resolution_1920_1080P__30_25, "1920*1080P30/25"}}));
    itemData.push_back(constructItemData_Camera_(
                           Remo_CmdId_Camera_Get_Thumbnail_Resolution, Remo_CmdId_Camera_Set_Thumbnail_Resolution, Remo_CmdId_Camera_Get_Thumbnail_Resolution_Range, "录像Thumbnail分辨率",
                            {{Thumbnail_Resolution_320_240 , "320*240"}}));
    itemData.push_back(constructItemData_Camera_(
                           Remo_CmdId_Camera_Get_LoopRec_Interval, Remo_CmdId_Camera_Set_LoopRec_Interval, Remo_CmdId_Camera_Get_LoopRec_Interval_Range, "循环录像周期",
                            {{LoopRec_Interval_2MIN , "2分钟"}, {LoopRec_Interval_5MIN , "5分钟"}, {LoopRec_Interval_10MIN, "10分钟"}, {LoopRec_Interval_25MIN, "25分钟"}}));
    itemData.push_back(constructItemData_Camera_(
                           Remo_CmdId_Camera_Get_LapseRec_Interval, Remo_CmdId_Camera_Set_LapseRec_Interval, Remo_CmdId_Camera_Get_LapseRec_Interval_Range, "缩时录像周期",
                            {{LapseRec_Interval_0_5S , "0.5秒"}, {LapseRec_Interval_1S , "1秒"}, {LapseRec_Interval_2S, "2秒"},
                             {LapseRec_Interval_3S , "3秒"}, {LapseRec_Interval_5S , "5秒"}, {LapseRec_Interval_8S, "8秒"},
                           {LapseRec_Interval_10S , "10秒"}, {LapseRec_Interval_20S , "20秒"}, {LapseRec_Interval_30S, "30秒"},
                           {LapseRec_Interval_1MIN , "1分钟"}, {LapseRec_Interval_2MIN , "2分钟"}, {LapseRec_Interval_5MIN, "5分钟"}}));
    itemData.push_back(constructItemData_Camera_(
                           Remo_CmdId_Camera_Get_LapseRec_TotalTime, Remo_CmdId_Camera_Set_LapseRec_TotalTime, Remo_CmdId_Camera_Get_LapseRec_TotalTime_Range, "缩时录像总时间",
                            {{LapseRec_TotalTime_30S , "30秒"}, {LapseRec_TotalTime_1MIN , "1分钟"}, {LapseRec_TotalTime_2MIN, "2分钟"},
                             {LapseRec_TotalTime_3MIN , "3分钟"}, {LapseRec_TotalTime_5MIN , "5分钟"}, {LapseRec_TotalTime_8MIN, "8分钟"},
                             {LapseRec_TotalTime_10MIN , "10分钟"}, {LapseRec_TotalTime_20MIN , "20分钟"}, {LapseRec_TotalTime_30MIN, "30分钟"},
                             {LapseRec_TotalTime_1H , "1小时"}, {LapseRec_TotalTime_2H , "2小时"}, {LapseRec_TotalTime_5H, "5小时"},
                             {LapseRec_TotalTime_Infinity , "无限大"}}));
    itemData.push_back(constructItemData_Camera_(
                           Remo_CmdId_Camera_Get_SlowMotion_Resolution, Remo_CmdId_Camera_Set_SlowMotion_Resolution, Remo_CmdId_Camera_Get_SlowMotion_Resolution_Range, "慢动作回放分辨率",
                            {{SlowMotion_Resolution_1280_720P__240_200 , "1280*720P240/200"}, {SlowMotion_Resolution_1280_720P__120_100 , "1280*720P120/100"},
                             {SlowMotion_Resolution_19201080P__120_100, "1920*1080P120/100"}}));
    itemData.push_back(constructItemData_Camera_(
                           Remo_CmdId_Camera_Get_CapStorageType, Remo_CmdId_Camera_Set_CapStorageType, Remo_CmdId_Camera_Get_CapStorageType_Range, "照片存储格式",
                            {{CapStorageType_JPEG , "JPEG"}, {CapStorageType_RAW , "RAW"}, {CapStorageType_JPEG_RAW, "JPEG+RAW"}}));
    itemData.push_back(constructItemData_Camera_(
                           Remo_CmdId_Camera_Get_CapStorageQuality, Remo_CmdId_Camera_Set_CapStorageQuality, Remo_CmdId_Camera_Get_CapStorageQuality_Range, "照片存储质量",
                            {{CapStorageQuality_High , "高"}, {CapStorageQuality_Medium , "中"}, {CapStorageQuality_Low, "低"}}));
    itemData.push_back(constructItemData_Camera_(
                           Remo_CmdId_Camera_Get_VideoMuxerType, Remo_CmdId_Camera_Set_VideoMuxerType, Remo_CmdId_Camera_Get_VideoMuxerType_Range, "视频存储格式",
                            {{VideoMuxerType_MP4 , "MP4"}, {VideoMuxerType_MOV , "MOV"}, {VideoMuxerType_FLV, "FLV"}}));
    itemData.push_back(constructItemData_Camera_(
                           Remo_CmdId_Camera_Get_VideoFormat, Remo_CmdId_Camera_Set_VideoFormat, Remo_CmdId_Camera_Bott, "视频制式",
                            {{VideoFormat_PAL , "PAL"}, {VideoFormat_NTSC , "NTSC"}}));
    itemData.push_back(constructItemData_Camera_(
                           Remo_CmdId_Camera_Get_PhotoColorType, Remo_CmdId_Camera_Set_PhotoColorType, Remo_CmdId_Camera_Get_PhotoColorType_Range, "色彩模式",
                            {{PhotoColorType_Normal , "普通"}, {PhotoColorType_Binary , "黑白"}, {PhotoColorType_Portrait, "人像"},
                             {PhotoColorType_Art , "艺术"}, {PhotoColorType_Dream , "梦幻"}, {PhotoColorType_Classical, "经典"},
                             {PhotoColorType_Nostalgic , "怀旧"}}));
    itemData.push_back(constructItemData_Camera_(
                           Remo_CmdId_Camera_Get_WhiteBalance, Remo_CmdId_Camera_Set_WhiteBalance, Remo_CmdId_Camera_Get_WhiteBalance_Range, "白平衡",
                            {{WhiteBalance_Auto , "自动"}, {WhiteBalance_Overcast , "阴天"}, {WhiteBalance_FluorescentLamp, "荧光灯"},
                             {WhiteBalance_DayLight , "晴天"}, {WhiteBalance_IncandescentLamp , "白炽灯"}, {WhiteBalance_Custom, "自定义"}}));
    itemData.push_back(constructItemData_Camera_(
                           Remo_CmdId_Camera_Get_CustomWB_ColorTemp, Remo_CmdId_Camera_Set_CustomWB_ColorTemp, Remo_CmdId_Camera_Get_CustomWB_ColorTemp_Range, "自定义白平衡色温"));
    itemData.push_back(constructItemData_Camera_(
                           Remo_CmdId_Camera_Get_AeMode, Remo_CmdId_Camera_Set_AeMode, Remo_CmdId_Camera_Get_AeMode_Range, "曝光模式",
                            {{HDRMode_Off , "自动"}, {AeMode_S , "S档"}, {AeMode_A, "A档"}, {AeMode_M, "M档"}}));
    itemData.push_back(constructItemData_Camera_(
                           Remo_CmdId_Camera_Get_P_Evbias, Remo_CmdId_Camera_Set_P_Evbias, Remo_CmdId_Camera_Bott, "曝光补偿",
                            {{Evbias_Neg_3 , "-3.0"}, {Evbias_Neg_2_7 , "-2.7"}, {Evbias_Neg_2_3, "-2.3"},
                             {Evbias_Neg_2 , "-2.0"}, {Evbias_Neg_1_7 , "-1.7"}, {Evbias_Neg_1_3, "-1.3"},
                             {Evbias_Neg_1 , "-1.0"}, {Evbias_Neg_0_7 , "-0.7"}, {Evbias_Neg_0_3, "-0.3"},
                             {Evbias_0 , "0"}, {Evbias_0_3 , "0.3"}, {Evbias_0_7, "0.7"},
                             {Evbias_1 , "1.0"}, {Evbias_1_3 , "1.3"}, {Evbias_1_7, "1.7"},
                             {Evbias_2_0 , "2.0"}, {Evbias_2_3 , "2.3"}, {Evbias_2_7, "2.7"},
                             {Evbias_3 , "3.0"}}));
    itemData.push_back(constructItemData_Camera_(
                           Remo_CmdId_Camera_Get_S_Shutter, Remo_CmdId_Camera_Set_S_Shutter, Remo_CmdId_Camera_Bott, "S档快门",
                            {{Shutter_1_DividedBy_8000, "1/8000"}, {Shutter_1_DividedBy_6400 , "1/6400"}, {Shutter_1_DividedBy_5000, "1/5000"},
                             {Shutter_1_DividedBy_4000, "1/4000"}, {Shutter_1_DividedBy_3200 , "1/3200"}, {Shutter_1_DividedBy_2500, "1/2500"},
                             {Shutter_1_DividedBy_2000, "1/2000"}, {Shutter_1_DividedBy_1600 , "1/1600"}, {Shutter_1_DividedBy_1250, "1/1250"},
                             {Shutter_1_DividedBy_1000, "1/1000"}, {Shutter_1_DividedBy_800 , "1/800"}, {Shutter_1_DividedBy_640, "1/640"},
                             {Shutter_1_DividedBy_500 , "1/500"}, {Shutter_1_DividedBy_400 , "1/400"}, {Shutter_1_DividedBy_320, "1/320"},
                             {Shutter_1_DividedBy_240 , "1/240"}, {Shutter_1_DividedBy_200 , "1/200"}, {Shutter_1_DividedBy_160, "1/160"},
                             {Shutter_1_DividedBy_120 , "1/120"}, {Shutter_1_DividedBy_100 , "1/100"}, {Shutter_1_DividedBy_80, "1/80"},
                             {Shutter_1_DividedBy_60 , "1/60"}, {Shutter_1_DividedBy_50 , "1/50"}, {Shutter_1_DividedBy_40, "1/40"},
                             {Shutter_1_DividedBy_30 , "1/30"}, {Shutter_1_DividedBy_25 , "1/25"}, {Shutter_1_DividedBy_20, "1/20"},
                             {Shutter_1_DividedBy_15 , "1/15"}, {Shutter_1_DividedBy_12_5 , "1/12.5"}, {Shutter_1_DividedBy_10, "1/10"},
                             {Shutter_1_DividedBy_8 , "1/8"}, {Shutter_1_DividedBy_6_25 , "1/6.25"}, {Shutter_1_DividedBy_5, "1/5"},
                             {Shutter_1_DividedBy_4 , "1/4"}, {Shutter_1_DividedBy_3 , "1/3"}, {Shutter_1_DividedBy_2_5, "1/2.5"},
                             {Shutter_1_DividedBy_2 , "1.2"}, {Shutter_1_DividedBy_1_67 , "1/1.67"}, {Shutter_1_DividedBy_1_25, "1/1.25"},
                             {Shutter_1 , "1"}, {Shutter_1_3 , "1.3"}, {Shutter_1_6, "1.6"},
                             {Shutter_2 , "2"}, {Shutter_2_5 , "2.5"}, {Shutter_3, "3"},
                             {Shutter_3_2 , "3.2"}, {Shutter_4 , "4"}, {Shutter_5, "5"},
                             {Shutter_6 , "6"}, {Shutter_7 , "7"}, {Shutter_8, "8"},
                             {Shutter_9 , "9"}, {Shutter_10 , "10"}, {Shutter_13, "13"},
                             {Shutter_15 , "15"}, {Shutter_20 , "20"}, {Shutter_25, "25"},
                             {Shutter_30 , "30"}}));
    itemData.push_back(constructItemData_Camera_(
                           Remo_CmdId_Camera_Get_S_Evbias, Remo_CmdId_Camera_Set_S_Evbias, Remo_CmdId_Camera_Bott, "S档曝光补偿",
                            {{Evbias_Neg_3 , "-3.0"}, {Evbias_Neg_2_7 , "-2.7"}, {Evbias_Neg_2_3, "-2.3"},
                             {Evbias_Neg_2 , "-2.0"}, {Evbias_Neg_1_7 , "-1.7"}, {Evbias_Neg_1_3, "-1.3"},
                             {Evbias_Neg_1 , "-1.0"}, {Evbias_Neg_0_7 , "-0.7"}, {Evbias_Neg_0_3, "-0.3"},
                             {Evbias_0 , "0"}, {Evbias_0_3 , "0.3"}, {Evbias_0_7, "0.7"},
                             {Evbias_1 , "1.0"}, {Evbias_1_3 , "1.3"}, {Evbias_1_7, "1.7"},
                             {Evbias_2_0 , "2.0"}, {Evbias_2_3 , "2.3"}, {Evbias_2_7, "2.7"},
                             {Evbias_3 , "3.0"}}));
    itemData.push_back(constructItemData_Camera_(
                           Remo_CmdId_Camera_Get_A_Aperture, Remo_CmdId_Camera_Set_A_Aperture, Remo_CmdId_Camera_Bott, "HDR",
                            {{Aperture_FNO_1 , "FNO.1"}, {Aperture_FNO_1_4 , "FNO.1.4"}, {Aperture_FNO_2, "FNO.2"}, {Aperture_FNO_2_8, "FNO.2.8"}, {Aperture_FNO_4, "FNO.4"}}));
    itemData.push_back(constructItemData_Camera_(
                           Remo_CmdId_Camera_Get_A_Evbias, Remo_CmdId_Camera_Set_A_Evbias, Remo_CmdId_Camera_Bott, "A档曝光补偿",
                            {{Evbias_Neg_3 , "-3.0"}, {Evbias_Neg_2_7 , "-2.7"}, {Evbias_Neg_2_3, "-2.3"},
                             {Evbias_Neg_2 , "-2.0"}, {Evbias_Neg_1_7 , "-1.7"}, {Evbias_Neg_1_3, "-1.3"},
                             {Evbias_Neg_1 , "-1.0"}, {Evbias_Neg_0_7 , "-0.7"}, {Evbias_Neg_0_3, "-0.3"},
                             {Evbias_0 , "0"}, {Evbias_0_3 , "0.3"}, {Evbias_0_7, "0.7"},
                             {Evbias_1 , "1.0"}, {Evbias_1_3 , "1.3"}, {Evbias_1_7, "1.7"},
                             {Evbias_2_0 , "2.0"}, {Evbias_2_3 , "2.3"}, {Evbias_2_7, "2.7"},
                             {Evbias_3 , "3.0"}}));
    itemData.push_back(constructItemData_Camera_(
                           Remo_CmdId_Camera_Get_Current_Aperture, Remo_CmdId_Camera_Set_Current_Aperture, Remo_CmdId_Camera_Bott, "当前光圈值",
                            {{Aperture_FNO_1 , "FNO.1"}, {Aperture_FNO_1_4 , "FNO.1.4"}, {Aperture_FNO_2, "FNO.2"}, {Aperture_FNO_2_8, "FNO.2.8"}, {Aperture_FNO_4, "FNO.4"}}));
    itemData.push_back(constructItemData_Camera_(
                           Remo_CmdId_Camera_Get_Current_ISO, Remo_CmdId_Camera_Bott, Remo_CmdId_Camera_Bott, "当前ISO值",
                            {{ISO_100 , "关闭"}, {ISO_200 , "3张合成"}, {ISO_400, "5张合成"},
                             {ISO_800 , "关闭"}, {ISO_1600 , "3张合成"}, {ISO_3200, "5张合成"},
                             {ISO_6400 , "关闭"}, {ISO_12800 , "3张合成"}, {ISO_25600, "5张合成"}}));
    itemData.push_back(constructItemData_Camera_(
                           Remo_CmdId_Camera_Bott, Remo_CmdId_Camera_Bott, Remo_CmdId_Camera_Get_M_Aperture_Range, "M档光圈",
                            {{Aperture_FNO_1 , "FNO.1"}, {Aperture_FNO_1_4 , "FNO.1.4"}, {Aperture_FNO_2, "FNO.2"}, {Aperture_FNO_2_8, "FNO.2.8"}, {Aperture_FNO_4, "FNO.4"}}));
    itemData.push_back(constructItemData_Camera_(
                           Remo_CmdId_Camera_Bott, Remo_CmdId_Camera_Bott, Remo_CmdId_Camera_Get_M_Shutter_Range, "M档快门",
                            {{Shutter_1_DividedBy_8000, "1/8000"}, {Shutter_1_DividedBy_6400 , "1/6400"}, {Shutter_1_DividedBy_5000, "1/5000"},
                             {Shutter_1_DividedBy_4000, "1/4000"}, {Shutter_1_DividedBy_3200 , "1/3200"}, {Shutter_1_DividedBy_2500, "1/2500"},
                             {Shutter_1_DividedBy_2000, "1/2000"}, {Shutter_1_DividedBy_1600 , "1/1600"}, {Shutter_1_DividedBy_1250, "1/1250"},
                             {Shutter_1_DividedBy_1000, "1/1000"}, {Shutter_1_DividedBy_800 , "1/800"}, {Shutter_1_DividedBy_640, "1/640"},
                             {Shutter_1_DividedBy_500 , "1/500"}, {Shutter_1_DividedBy_400 , "1/400"}, {Shutter_1_DividedBy_320, "1/320"},
                             {Shutter_1_DividedBy_240 , "1/240"}, {Shutter_1_DividedBy_200 , "1/200"}, {Shutter_1_DividedBy_160, "1/160"},
                             {Shutter_1_DividedBy_120 , "1/120"}, {Shutter_1_DividedBy_100 , "1/100"}, {Shutter_1_DividedBy_80, "1/80"},
                             {Shutter_1_DividedBy_60 , "1/60"}, {Shutter_1_DividedBy_50 , "1/50"}, {Shutter_1_DividedBy_40, "1/40"},
                             {Shutter_1_DividedBy_30 , "1/30"}, {Shutter_1_DividedBy_25 , "1/25"}, {Shutter_1_DividedBy_20, "1/20"},
                             {Shutter_1_DividedBy_15 , "1/15"}, {Shutter_1_DividedBy_12_5 , "1/12.5"}, {Shutter_1_DividedBy_10, "1/10"},
                             {Shutter_1_DividedBy_8 , "1/8"}, {Shutter_1_DividedBy_6_25 , "1/6.25"}, {Shutter_1_DividedBy_5, "1/5"},
                             {Shutter_1_DividedBy_4 , "1/4"}, {Shutter_1_DividedBy_3 , "1/3"}, {Shutter_1_DividedBy_2_5, "1/2.5"},
                             {Shutter_1_DividedBy_2 , "1.2"}, {Shutter_1_DividedBy_1_67 , "1/1.67"}, {Shutter_1_DividedBy_1_25, "1/1.25"},
                             {Shutter_1 , "1"}, {Shutter_1_3 , "1.3"}, {Shutter_1_6, "1.6"},
                             {Shutter_2 , "2"}, {Shutter_2_5 , "2.5"}, {Shutter_3, "3"},
                             {Shutter_3_2 , "3.2"}, {Shutter_4 , "4"}, {Shutter_5, "5"},
                             {Shutter_6 , "6"}, {Shutter_7 , "7"}, {Shutter_8, "8"},
                             {Shutter_9 , "9"}, {Shutter_10 , "10"}, {Shutter_13, "13"},
                             {Shutter_15 , "15"}, {Shutter_20 , "20"}, {Shutter_25, "25"},
                             {Shutter_30 , "30"}}));
    itemData.push_back(constructItemData_Camera_(
                           Remo_CmdId_Camera_Bott, Remo_CmdId_Camera_Bott, Remo_CmdId_Camera_Get_M_ISO_Range, "M档IOS范围",
                            {{ISO_100 , "关闭"}, {ISO_200 , "3张合成"}, {ISO_400, "5张合成"},
                             {ISO_800 , "关闭"}, {ISO_1600 , "3张合成"}, {ISO_3200, "5张合成"},
                             {ISO_6400 , "关闭"}, {ISO_12800 , "3张合成"}, {ISO_25600, "5张合成"}}));
    itemData.push_back(constructItemData_Camera_(
                           Remo_CmdId_Camera_Get_Current_Evbias, Remo_CmdId_Camera_Bott, Remo_CmdId_Camera_Bott, "当前曝光补偿"));
    itemData.push_back(constructItemData_Camera_(
                           Remo_CmdId_Camera_Bott, Remo_CmdId_Camera_Bott, Remo_CmdId_Camera_Get_Photo_Shutter_Range, "照相模式快门范围",
                            {{Shutter_1_DividedBy_8000, "1/8000"}, {Shutter_1_DividedBy_6400 , "1/6400"}, {Shutter_1_DividedBy_5000, "1/5000"},
                             {Shutter_1_DividedBy_4000, "1/4000"}, {Shutter_1_DividedBy_3200 , "1/3200"}, {Shutter_1_DividedBy_2500, "1/2500"},
                             {Shutter_1_DividedBy_2000, "1/2000"}, {Shutter_1_DividedBy_1600 , "1/1600"}, {Shutter_1_DividedBy_1250, "1/1250"},
                             {Shutter_1_DividedBy_1000, "1/1000"}, {Shutter_1_DividedBy_800 , "1/800"}, {Shutter_1_DividedBy_640, "1/640"},
                             {Shutter_1_DividedBy_500 , "1/500"}, {Shutter_1_DividedBy_400 , "1/400"}, {Shutter_1_DividedBy_320, "1/320"},
                             {Shutter_1_DividedBy_240 , "1/240"}, {Shutter_1_DividedBy_200 , "1/200"}, {Shutter_1_DividedBy_160, "1/160"},
                             {Shutter_1_DividedBy_120 , "1/120"}, {Shutter_1_DividedBy_100 , "1/100"}, {Shutter_1_DividedBy_80, "1/80"},
                             {Shutter_1_DividedBy_60 , "1/60"}, {Shutter_1_DividedBy_50 , "1/50"}, {Shutter_1_DividedBy_40, "1/40"},
                             {Shutter_1_DividedBy_30 , "1/30"}, {Shutter_1_DividedBy_25 , "1/25"}, {Shutter_1_DividedBy_20, "1/20"},
                             {Shutter_1_DividedBy_15 , "1/15"}, {Shutter_1_DividedBy_12_5 , "1/12.5"}, {Shutter_1_DividedBy_10, "1/10"},
                             {Shutter_1_DividedBy_8 , "1/8"}, {Shutter_1_DividedBy_6_25 , "1/6.25"}, {Shutter_1_DividedBy_5, "1/5"},
                             {Shutter_1_DividedBy_4 , "1/4"}, {Shutter_1_DividedBy_3 , "1/3"}, {Shutter_1_DividedBy_2_5, "1/2.5"},
                             {Shutter_1_DividedBy_2 , "1.2"}, {Shutter_1_DividedBy_1_67 , "1/1.67"}, {Shutter_1_DividedBy_1_25, "1/1.25"},
                             {Shutter_1 , "1"}, {Shutter_1_3 , "1.3"}, {Shutter_1_6, "1.6"},
                             {Shutter_2 , "2"}, {Shutter_2_5 , "2.5"}, {Shutter_3, "3"},
                             {Shutter_3_2 , "3.2"}, {Shutter_4 , "4"}, {Shutter_5, "5"},
                             {Shutter_6 , "6"}, {Shutter_7 , "7"}, {Shutter_8, "8"},
                             {Shutter_9 , "9"}, {Shutter_10 , "10"}, {Shutter_13, "13"},
                             {Shutter_15 , "15"}, {Shutter_20 , "20"}, {Shutter_25, "25"},
                             {Shutter_30 , "30"}}));
    itemData.push_back(constructItemData_Camera_(
                           Remo_CmdId_Camera_Bott, Remo_CmdId_Camera_Bott, Remo_CmdId_Camera_Get_Record_Shutter_Range, "录影模式快门范围",
                            {{Shutter_1_DividedBy_8000, "1/8000"}, {Shutter_1_DividedBy_6400 , "1/6400"}, {Shutter_1_DividedBy_5000, "1/5000"},
                             {Shutter_1_DividedBy_4000, "1/4000"}, {Shutter_1_DividedBy_3200 , "1/3200"}, {Shutter_1_DividedBy_2500, "1/2500"},
                             {Shutter_1_DividedBy_2000, "1/2000"}, {Shutter_1_DividedBy_1600 , "1/1600"}, {Shutter_1_DividedBy_1250, "1/1250"},
                             {Shutter_1_DividedBy_1000, "1/1000"}, {Shutter_1_DividedBy_800 , "1/800"}, {Shutter_1_DividedBy_640, "1/640"},
                             {Shutter_1_DividedBy_500 , "1/500"}, {Shutter_1_DividedBy_400 , "1/400"}, {Shutter_1_DividedBy_320, "1/320"},
                             {Shutter_1_DividedBy_240 , "1/240"}, {Shutter_1_DividedBy_200 , "1/200"}, {Shutter_1_DividedBy_160, "1/160"},
                             {Shutter_1_DividedBy_120 , "1/120"}, {Shutter_1_DividedBy_100 , "1/100"}, {Shutter_1_DividedBy_80, "1/80"},
                             {Shutter_1_DividedBy_60 , "1/60"}, {Shutter_1_DividedBy_50 , "1/50"}, {Shutter_1_DividedBy_40, "1/40"},
                             {Shutter_1_DividedBy_30 , "1/30"}, {Shutter_1_DividedBy_25 , "1/25"}, {Shutter_1_DividedBy_20, "1/20"},
                             {Shutter_1_DividedBy_15 , "1/15"}, {Shutter_1_DividedBy_12_5 , "1/12.5"}, {Shutter_1_DividedBy_10, "1/10"},
                             {Shutter_1_DividedBy_8 , "1/8"}, {Shutter_1_DividedBy_6_25 , "1/6.25"}, {Shutter_1_DividedBy_5, "1/5"},
                             {Shutter_1_DividedBy_4 , "1/4"}, {Shutter_1_DividedBy_3 , "1/3"}, {Shutter_1_DividedBy_2_5, "1/2.5"},
                             {Shutter_1_DividedBy_2 , "1.2"}, {Shutter_1_DividedBy_1_67 , "1/1.67"}, {Shutter_1_DividedBy_1_25, "1/1.25"},
                             {Shutter_1 , "1"}, {Shutter_1_3 , "1.3"}, {Shutter_1_6, "1.6"},
                             {Shutter_2 , "2"}, {Shutter_2_5 , "2.5"}, {Shutter_3, "3"},
                             {Shutter_3_2 , "3.2"}, {Shutter_4 , "4"}, {Shutter_5, "5"},
                             {Shutter_6 , "6"}, {Shutter_7 , "7"}, {Shutter_8, "8"},
                             {Shutter_9 , "9"}, {Shutter_10 , "10"}, {Shutter_13, "13"},
                             {Shutter_15 , "15"}, {Shutter_20 , "20"}, {Shutter_25, "25"},
                             {Shutter_30 , "30"}}));
    itemData.push_back(constructItemData_Camera_(
                           Remo_CmdId_Camera_Bott, Remo_CmdId_Camera_Bott, Remo_CmdId_Camera_Get_Photo_ISO_Range, "照相模式ISO范围",
                            {{ISO_100 , "关闭"}, {ISO_200 , "3张合成"}, {ISO_400, "5张合成"},
                             {ISO_800 , "关闭"}, {ISO_1600 , "3张合成"}, {ISO_3200, "5张合成"},
                             {ISO_6400 , "关闭"}, {ISO_12800 , "3张合成"}, {ISO_25600, "5张合成"}}));
    itemData.push_back(constructItemData_Camera_(
                           Remo_CmdId_Camera_Bott, Remo_CmdId_Camera_Bott, Remo_CmdId_Camera_Get_Record_ISO_Range, "录影模式ISO范围",
                            {{ISO_100 , "关闭"}, {ISO_200 , "3张合成"}, {ISO_400, "5张合成"},
                             {ISO_800 , "关闭"}, {ISO_1600 , "3张合成"}, {ISO_3200, "5张合成"},
                             {ISO_6400 , "关闭"}, {ISO_12800 , "3张合成"}, {ISO_25600, "5张合成"}}));
    itemData.push_back(constructItemData_Camera_(
                           Remo_CmdId_Camera_Bott, Remo_CmdId_Camera_Bott, Remo_CmdId_Camera_Get_Aperture_Range, "光圈范围",
                            {{Aperture_FNO_1 , "FNO.1"}, {Aperture_FNO_1_4 , "FNO.1.4"}, {Aperture_FNO_2, "FNO.2"}, {Aperture_FNO_2_8, "FNO.2.8"}, {Aperture_FNO_4, "FNO.4"}}));
    itemData.push_back(constructItemData_Camera_(
                           Remo_CmdId_Camera_Get_MeterMode, Remo_CmdId_Camera_Set_MeterMode, Remo_CmdId_Camera_Get_MeterMode_Range, "测光模式",
                            {{MeterMode_Global , "全局测光"}, {MeterMode_Point , "点测光"}}));
    itemData.push_back(constructItemData_Camera_(
                           Remo_CmdId_Camera_Get_Antiflick, Remo_CmdId_Camera_Set_Antiflick, Remo_CmdId_Camera_Get_Antiflick_Range, "抗闪烁",
                            {{Antiflick_Off , "关闭"}, {Antiflick_50HZ , "50HZ"}, {Antiflick_60HZ, "60HZ"}, {Antiflick_Auto, "自动"}}));
    itemData.push_back(constructItemData_Camera_(
                           Remo_CmdId_Camera_Get_AELockStatus, Remo_CmdId_Camera_Set_AELockStatus, Remo_CmdId_Camera_Bott, "AELOCK",
                            {{AELockStatus_Off , "关闭"}, {AELockStatus_On , "打开"}}));
    itemData.push_back(constructItemData_Camera_(
                           Remo_CmdId_Camera_Get_AFMode, Remo_CmdId_Camera_Set_AFMode, Remo_CmdId_Camera_Get_AFMode_Range, "对焦模式",
                            {{AFMode_MF , "MF"}, {AFMode_AFS , "AFS"}, {AFMode_AFC , "AFC"}}));
    itemData.push_back(constructItemData_Camera_(
                           Remo_CmdId_Camera_Get_PosArea, Remo_CmdId_Camera_Bott, Remo_CmdId_Camera_Bott, "对焦位置"));
    itemData.push_back(constructItemData_Camera_(
                           Remo_CmdId_Camera_Get_Sharpness, Remo_CmdId_Camera_Set_Sharpness, Remo_CmdId_Camera_Bott, "锐度",
                            {{Sharpness_Lowest , "弱锐化"}, {Sharpness_Lower , "柔和"}, {Sharpness_Medium, "一般"}, {Sharpness_Higher, "清晰"}, {Sharpness_Highest, "锐利"}}));
    itemData.push_back(constructItemData_Camera_(
                           Remo_CmdId_Camera_Get_HDRMode, Remo_CmdId_Camera_Set_HDRMode, Remo_CmdId_Camera_Bott, "HDR",
                            {{HDRMode_Off , "关闭"}, {HDRMode_3 , "3张合成"}, {HDRMode_5, "5张合成"}, {HDRMode_Auto, "自动"}}));
    itemData.push_back(constructItemData_Camera_(
                           Remo_CmdId_Camera_Get_Rotation, Remo_CmdId_Camera_Set_Rotation, Remo_CmdId_Camera_Get_Rotation_Range, "旋转",
                            {{Rotation_0D , "0度"}, {Rotation_90D , "90度"}, {Rotation_180D, "180度"},
                             {Rotation_360D , "360度"}, {Rotation_Mirror , "镜像"}, {Rotation_90D_Mirror, "90度镜像"},
                             {Rotation_180D_Mirror , "180度镜像"}, {Rotation_270D_Mirror , "270度镜像"}}));
}

std::map<void*, ItemData> ui2ItemData;
void addItem2Map(void * item, Remo_CmdId_e idGet, int idSet, int idRange, int set)
{
    if (nullptr == item) return;

    for (auto it : itemData) {
        if (set == it.CmdSet &&
                (idGet == it.CmdId_GetData || idSet == it.CmdId_SetData || idRange == it.CmdId_GetRange)) {
            ui2ItemData[item] = it;
        }
    }
}

void * findUiPtrById(Remo_CmdId_e id, Remo_CmdSet_e set)
{
    for (auto it : ui2ItemData) {
        ItemData item = it.second;
        if (set == item.CmdSet &&
                (id == item.CmdId_GetData || id == item.CmdId_SetData || id == item.CmdId_GetRange)) {
            return it.first;
        }
    }
    return nullptr;
}

bool findItemByUiPtr(void * ptr, ItemData & data)
{
    auto it = ui2ItemData.find(ptr);
    if (it == ui2ItemData.end()) {
        return false;
    } else {
        data = (*it).second;
    }
    return true;
}
