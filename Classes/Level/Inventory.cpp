#include "Inventory.h"

const int Inventory::SECRET_VALUE = 0xA3AB755A;
const std::string Inventory::ITEM_COUNT_CHANGED = "ITEM_COUNT_CHANGED";

Inventory* Inventory::create()
{
    auto instance = new (std::nothrow) Inventory();
    instance->autorelease();
    return instance;
}

int Inventory::getItemCount(TileType itemType)
{
    auto iter = _itemCount.find(itemType);
    if (iter == _itemCount.end())
    {
        return 0;
    }
    else
    {
        return iter->second ^ SECRET_VALUE;
    }
}

void Inventory::setItemCount(TileType itemType, int count)
{
    auto iter = _itemCount.find(itemType);
    if (iter == _itemCount.end())
    {
        if (count != 0)
        {
            _itemCount[itemType] = count ^ SECRET_VALUE;
            cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(ITEM_COUNT_CHANGED);
        }
    }
    else
    {
        if (iter->second != count)
        {
            iter->second = count ^ SECRET_VALUE;
            cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(ITEM_COUNT_CHANGED);
        }
    }
}

void Inventory::addItem(TileType itemType, int count)
{
    setItemCount(itemType, getItemCount(itemType) + count);
}

void Inventory::clear()
{
    if (!_itemCount.empty())
    {
        _itemCount.clear();
        cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(ITEM_COUNT_CHANGED);
    }
}
