#include "Fire.h"
#include "LevelData/TileType.h"
#include "Level/Level.h"
#include "Level/Creature.h"
#include "Level/Inventory.h"

Fire* Fire::create(const cocos2d::Vec2& coordinate)
{
	auto instance = new Fire(coordinate);
    instance->autorelease();
    return instance;
}

Fire::Fire(const cocos2d::Vec2& coordinate) : LevelObject(coordinate)
{
}

bool Fire::isEnterableBy(const Creature *creature, Direction direction) const
{
    return (creature->getType() != CreatureType::BUG);
}

void Fire::afterEnter(Creature *creature)
{
    if (creature->getType() != CreatureType::CHIP || _level->getInventory()->getItemCount(TileType::BOOTS_FIRE) == 0)
    {
        
    }
}

void Fire::buildNodes()
{

}

void Fire::destroyNodes()
{

}
