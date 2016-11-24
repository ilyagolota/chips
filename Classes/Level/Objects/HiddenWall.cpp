#include "HiddenWall.h"
#include <Level/Level.h>

HiddenWall* HiddenWall::create(Level* level, const cocos2d::Vec2& coordinate, bool permanent)
{
	auto instance = new HiddenWall(level, coordinate, permanent);
    instance->autorelease();
    return instance;
}

HiddenWall::HiddenWall(Level* level, const cocos2d::Vec2& coordinate, bool permanent) : LevelObject(level, coordinate)
{
	_permanent = permanent;
	_known = false;
    
    _floor = cocos2d::Sprite::createWithSpriteFrameName("floor.png");
    _floor->setAnchorPoint(cocos2d::Vec2::ZERO);
    _floor->setPosition(_level->getProjector()->coordinateToPoint(_coordinate) + cocos2d::Vec2(0, -12));
    _floor->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::BACK_Z_ORDER);
    _level->getStage()->addChild(_floor);
    
    _level->getPhysicsWorld()->setBody(_coordinate, TileBody::OUTER_BOX, 7);
}

bool HiddenWall::isOpenableBy(const Creature *creature, Direction direction) const
{
    return (!_known && creature->getType() == CreatureType::CHIP);
}

void HiddenWall::beforeEnter(Creature* creature)
{
    _known = true;
    
    auto duration = _level->getTurnDuration() * creature->getTurnsPerMove();
    
    auto back = inverse(creature->getDirection());
    auto prevCoordinate = _coordinate + toVec2(back);
    auto prevObject = _level->getObjectAt(prevCoordinate);
    if (prevObject != nullptr)
    {
        auto cachedDirection = creature->getDirection();
        creature->setDirection(back);
        prevObject->beforeEnter(creature);
        creature->setDirection(cachedDirection);
    }
    
    creature->getSprite()->runAction(cocos2d::Sequence::create(
        cocos2d::DelayTime::create(0.51f * duration),
        cocos2d::CallFunc::create([this, creature, duration, prevCoordinate]()
        {
            auto targetPosition = _level->getProjector()->coordinateToPoint(prevCoordinate);
            auto action = cocos2d::MoveTo::create(0.49f * duration, targetPosition);
            
            auto creatureSprite = creature->getSprite();
            creatureSprite->stopActionByTag(Creature::MOVE_ACTION_TAG);
            action->setTag(Creature::MOVE_ACTION_TAG);
            creatureSprite->runAction(action);
        }),
        nullptr
    ));
    
    int shape = _level->getWallShape(_coordinate);
	auto wallSprite = cocos2d::Sprite::createWithSpriteFrameName(getFloorFrameName(shape));
    wallSprite->setAnchorPoint(cocos2d::Vec2::ZERO);
    wallSprite->setPosition(cocos2d::Vec2(0, 0));
    wallSprite->setOpacity(0);
    wallSprite->runAction(cocos2d::Sequence::create(
        cocos2d::DelayTime::create(0.51f * duration),
        cocos2d::FadeTo::create(_level->getTurnDuration() * 0.35, 255),
        nullptr
    ));
    _floor->addChild(wallSprite);
}

void HiddenWall::afterEnter(Creature* creature)
{
    auto back = inverse(creature->getDirection());
    creature->setDirection(back);
    
    auto prevCoordinate = _coordinate + toVec2(back);
    creature->setCoordinate(prevCoordinate);
    
    auto prevObject = _level->getObjectAt(prevCoordinate);
    if (prevObject != nullptr)
    {
        prevObject->afterEnter(creature);
    }
}

const std::string& HiddenWall::getFloorFrameName(int shape)
{
	static std::string frameNames[] =
	{
		"blue-floor-0001.png",
		"blue-floor-0002.png",
		"blue-floor-0003.png",
		"blue-floor-0004.png",
		"blue-floor-0005.png",
		"blue-floor-0006.png"
	};

	if (shape == 0x0c)
	{
		return frameNames[2];
	}
	else if (shape == 0x03)
	{
		return frameNames[3];
	}
	else if (shape == 0x06)
	{
		return frameNames[4];
	}
	else if (shape == 0x09)
	{
		return frameNames[5];
	}
	else if (((shape >> 3) & 1) + ((shape >> 1) & 1) > ((shape >> 2) & 1) + (shape & 1))
	{
		return frameNames[1];
	}
	else
	{
		return frameNames[0];
	}
}
