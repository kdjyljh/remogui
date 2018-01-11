#ifndef ITEMDATADEF_H
#define ITEMDATADEF_H
#include "cmddef.h"
#include "camerafielddef.h"

#include <string>
#include <set>
#include <vector>
#include <map>
#include <QMetaType>

struct SubItemData
{
    int Index;
    std::string ShowStr;
    bool operator < (const SubItemData & obj) const
    {
        return Index < obj.Index ? true : false;
    }
};
struct ItemData
{
    int CmdSet;
    int CmdId_GetData;
    int CmdId_SetData;
    int CmdId_GetRange;
    std::string ShowStr;
    std::set<SubItemData> subItemData;
    bool operator < (const ItemData & obj) const
    {
        return (CmdSet < obj.CmdSet || CmdId_GetData < obj.CmdId_GetData ||
                CmdId_GetRange < obj.CmdId_GetRange || CmdId_SetData < obj.CmdId_SetData);
    }
};

typedef enum Range_Type_E
{
    SINGLE = 0,//单个离散的数
    RANGE_1,//最小和最大值
    RANGE_2//最小和最大值和step，类似等差数列
}Range_Type;

typedef struct Range_Data_S
{
    Range_Type type;
    int length;
    int * data;
}Range_Data;

struct CmdContent
{
    std::set<SubItemData> range;
    std::vector<uint8_t> custom;
    int cmdSet;
    int cmdId;
};
Q_DECLARE_METATYPE(CmdContent);

enum DispatcheType
{
    DispatcheType_AeMode            = 0,
    DispatcheType_DeviceInfo        = 1,
    DispatcheType_Focus_Zoom        = 2,
    DispatcheType_Gimbal            = 3,
    DispatcheType_WorkMode          = 4,
    DispatcheType_Algorithm         = 5
};

extern std::set<ItemData> itemData;
extern std::map<void*, ItemData> ui2ItemData;
extern void addItem2Map(void * item, Remo_CmdId_Camera_e idGet = Remo_CmdId_Camera_PlaceHolder, int idSet = Remo_CmdId_Camera_PlaceHolder, int idRange = Remo_CmdId_Camera_PlaceHolder, int set = Remo_CmdSet_Camera, bool addItemData = false);
extern void * findUiPtrById(Remo_CmdId_Camera_e id, Remo_CmdSet_e set = Remo_CmdSet_Camera);
extern void * findUiPtrById(Remo_CmdId_Camera_e idGet, Remo_CmdId_Camera_e idSet, Remo_CmdId_Camera_e idGetRange, Remo_CmdSet_e set = Remo_CmdSet_Camera);
extern bool findItemByUiPtr(void * ptr, ItemData & data);
extern void init_itemData();
#endif // ITEMDATADEF_H
