#ifndef ITEMDATADEF_H
#define ITEMDATADEF_H
#include "cmddef.h"
#include "camerafielddef.h"

#include <string>
#include <set>
#include <vector>
#include <map>

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
};

extern std::vector<ItemData> itemData;
extern std::map<void*, ItemData> ui2ItemData;
extern void addItem2Map(void * item, Remo_CmdId_e idGet = Remo_CmdId_Camera_PlaceHolder, int idSet = Remo_CmdId_Camera_PlaceHolder, int idRange = Remo_CmdId_Camera_PlaceHolder, int set = Remo_CmdSet_PlaceHolder);
extern void * findUiPtrById(Remo_CmdId_e id, Remo_CmdSet_e set = Remo_CmdSet_Camera);
extern bool findItemByUiPtr(void * ptr, ItemData & data);
extern void init_itemData();
#endif // ITEMDATADEF_H