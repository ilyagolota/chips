#include "Socket.h"
#include "LevelData/TileType.h"
#include "LevelData/LevelData.h"
#include "Level/Level.h"
#include "Level/Inventory.h"
#include "Level/Creature.h"

Socket* Socket::create(Level* level, const cocos2d::Vec2& coordinate)
{
    auto instance = new Socket(level, coordinate);
    instance->autorelease();
    return instance;
}

Socket::Socket(Level* level, const cocos2d::Vec2& coordinate) : LevelObject(level, coordinate)
{
    _open = false;
}

bool Socket::isOpenableBy(const Creature* creature, Direction direction) const
{
    if (creature->getType() == CreatureType::CHIP)
    {
        auto inventory = _level->getInventory();
        return (_level->getLevelData()->getChipsRequired() <= inventory->getItemCount(TileType::IC_CHIP));
    }
    else
    {
        return false;
    }
}
