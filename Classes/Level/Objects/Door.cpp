#include "Door.h"
#include "Tiled/TiledPhysicsWorld.h"
#include "Level/Level.h"
#include "Level/Inventory.h"
#include "Level/Creature.h"

Door* Door::create(const cocos2d::Vec2& coordinate, TileType type)
{
	auto instance = new Door(coordinate, type);
    instance->autorelease();
    return instance;
}

Door::Door(const cocos2d::Vec2& coordinate, TileType type) : LevelObject(coordinate)
{
    _type = type;
    _open = false;

	_floor = cocos2d::Sprite::createWithSpriteFrameName("floor-" + getColorName() + "-ew.png");
	_floor->setAnchorPoint(cocos2d::Vec2::ZERO);
	_floor->setPosition(cocos2d::Vec2(0, -12));
	_floor->setLocalZOrder(Level::BACK_Z_ORDER);
	addNode(_floor);
}

void Door::reset()
{
    _level->getPhysicsWorld()->setBody(_coordinate, TileBody::OUTER_BOX, 7);
}

bool Door::isOpenableBy(const Creature *creature, Direction direction) const
{
    if (creature->getType() == CreatureType::CHIP)
    {
        auto keyType = getKeyType();
        if (_level->getInventory()->getItemCount(keyType) > 0)
        {
            return true;
        }
    }
    return false;
}

void Door::beforeEnter(Creature* creature)
{
    if (!_open)
    {
        _open = true;
		_level->getPhysicsWorld()->setBody(_coordinate, TileBody::EMPTY, 7);
        
        if (creature->getType() == CreatureType::CHIP)
        {
            auto keyType = getKeyType();
            if (keyType != TileType::KEY_GREEN)
            {
                if (_level->getInventory()->getItemCount(keyType) > 0)
                {
                    _level->getInventory()->addItem(keyType, -1);
                }
            }
        }

        auto animation = cocos2d::AnimationCache::getInstance()->getAnimation("door-" + getColorName() + "-open");
        
        cocos2d::Vector<cocos2d::FiniteTimeAction*> actions;
        actions.pushBack(cocos2d::Animate::create(animation));
        //actions.pushBack();
        
        //_sprites[0]->runAction(cocos2d::Sequence::create(actions));
    }
}

std::string& Door::getColorName() const
{
    static std::string __names[] = { "blue", "red", "green", "yellow" };
	size_t index = static_cast<size_t>(_type) - static_cast<size_t>(TileType::DOOR_BLUE);
    if (index < 0 || index >= 4)
    {
        index = 0;
    }
	return __names[index];
}

TileType Door::getKeyType() const
{
    size_t index = static_cast<size_t>(_type) - static_cast<size_t>(TileType::DOOR_BLUE);
    return static_cast<TileType>(static_cast<size_t>(TileType::KEY_BLUE) + index);
}

void Door::buildNodes()
{

}

void Door::destroyNodes()
{

}