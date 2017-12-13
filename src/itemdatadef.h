#ifndef ITEMDATADEF_H
#define ITEMDATADEF_H
#include "cmddef.h"
#include "camerafielddef.h"

#include <string>
#include <set>
#include <QComboBox>
#include <QAction>

struct SubItemData
{
    int Index;
    QString ShowStr;
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
    QString ShowStr;
    std::set<SubItemData> subItemData;
};

extern std::vector<ItemData> itemData;
#endif // ITEMDATADEF_H
