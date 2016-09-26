#include "Beartrap.h"
#include <Level/Level.h>

cocos2d::Vec2 Beartrap::BOTTOM_POSITION(0, -46);

Beartrap* Beartrap::create(Level* level, const cocos2d::Vec2& coordinate)
{
    auto instance = new Beartrap(level, coordinate);
    instance->autorelease();
    return instance;
}

Beartrap::Beartrap(Level* level, const cocos2d::Vec2& coordinate) : LevelObject(level, coordinate)
{
    _floor = cocos2d::Sprite::createWithSpriteFrameName("trap.png");
	_floor->setPosition(_level->getProjector()->coordinateToPoint(_coordinate) + cocos2d::Vec2(0, -12));
	_floor->setZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::BACK_Z_ORDER);
	_floor->setAnchorPoint(cocos2d::Vec2::ZERO);
	_level->getStage()->addChild(_floor);

	_piston = cocos2d::Sprite::createWithSpriteFrameName("trap-piston.png");
	_piston->setPosition(cocos2d::Vec2::ZERO);
	_piston->setAnchorPoint(cocos2d::Vec2::ZERO);
	_floor->addChild(_piston);

	_front = cocos2d::Sprite::createWithSpriteFrameName("trap-front.png");
	_front->setPosition(_level->getProjector()->coordinateToPoint(_coordinate) + cocos2d::Vec2(0, -12));
	_front->setAnchorPoint(cocos2d::Vec2::ZERO);
	_front->setZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::WALL_Z_ORDER);
	_level->getStage()->addChild(_front);

	reset();
}

bool Beartrap::isOpen() const
{
    return _open;
}

void Beartrap::setOpen(bool open)
{
	if (_open != open)
	{
		_open = open;
	
		auto duration = _level->getTurnDuration();
		_piston->stopAllActions();
		_piston->runAction(cocos2d::MoveTo::create(0.5f * duration, _open ? cocos2d::Vec2::ZERO : BOTTOM_POSITION));

		auto creaturePosition = _level->getProjector()->coordinateToPoint(_coordinate) + (_open ? cocos2d::Vec2::ZERO : BOTTOM_POSITION);
		for (auto creature : _level->getCreatures())
		{
			if (creature->getCoordinate() == _coordinate)
			{
				auto action = cocos2d::MoveTo::create(duration, creaturePosition);
				action->setTag(90);
				creature->getSprite()->stopAllActionsByTag(90);
				creature->getSprite()->runAction(action);
			}
		}
	}
}

void Beartrap::reset()
{
	_open = false;
	_piston->setPosition(BOTTOM_POSITION);
}

bool Beartrap::isEscapableBy(const Creature* creature, Direction direction) const
{
    return _open;
}

void Beartrap::beforeEnter(Creature* creature)
{
	if (!_open)
	{
		auto duration = _level->getTurnDuration() * creature->getTurnsPerMove();

		creature->getSprite()->runAction(cocos2d::Sequence::create(
			cocos2d::DelayTime::create(0.51f * duration),
			cocos2d::CallFunc::create([this, creature, duration]() {
				auto creatureSprite = creature->getSprite();

				auto thisPosition = _level->getProjector()->coordinateToPoint(_coordinate);
				auto action = cocos2d::Sequence::create(
					cocos2d::MoveTo::create(0.14f * duration, thisPosition),
					cocos2d::MoveTo::create(0.35f * duration, thisPosition + BOTTOM_POSITION),
					nullptr
				);

				// Override default moving trajectory
				creatureSprite->stopActionByTag(Creature::MOVE_ACTION_TAG);
				action->setTag(Creature::MOVE_ACTION_TAG);
				creatureSprite->runAction(action);
			}),
			nullptr)
		);
	}
}

void Beartrap::afterEnter(Creature* creature)
{
	if (!_open)
	{
		auto thisPosition = _level->getProjector()->coordinateToPoint(_coordinate);
		creature->getSprite()->setPosition(thisPosition + BOTTOM_POSITION);
	}
}
