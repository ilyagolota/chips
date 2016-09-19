#include "Socket.h"
#include <Level/Level.h>

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
        return (_level->getConfig()->getChipsRequired() <= inventory->getItemCount(TileType::IC_CHIP));
    }
    else
    {
        return false;
    }
}
