#ifndef _CHIPS_CHALLENGE_INVENTORY_H_
#define _CHIPS_CHALLENGE_INVENTORY_H_

#include <cocos2d.h>
#include <Utils/Protected.h>
#include <Level/TileType.h>

class Inventory : public cocos2d::Ref
{
private:
    std::map<TileType, Protected<int>> _itemCount;
    
public:
    static const std::string ITEM_COUNT_CHANGED;
    static Inventory* create();
    
    int getItemCount(TileType itemType);
    void setItemCount(TileType itemType, int count);
    void addItem(TileType itemType, int count);
    void clear();
};

#endif
