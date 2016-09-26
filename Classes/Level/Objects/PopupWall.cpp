#include "PopupWall.h"
#include <Level/Level.h>

cocos2d::Vec2 PopupWall::BOTTOM_POSITION(0, -17);
cocos2d::Vec2 PopupWall::TOP_POSITION(0, 25);

PopupWall* PopupWall::create(Level* level, const cocos2d::Vec2& coordinate)
{
	auto instance = new PopupWall(level, coordinate);
	instance->autorelease();
	return instance;
}

PopupWall::PopupWall(Level* level, const cocos2d::Vec2& coordinate) : LevelObject(level, coordinate)
{
	_floor = cocos2d::Sprite::createWithSpriteFrameName("trap.png");
	_floor->setPosition(_level->getProjector()->coordinateToPoint(_coordinate) + cocos2d::Vec2(0, -12));
	_floor->setZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::BACK_Z_ORDER);
	_floor->setAnchorPoint(cocos2d::Vec2::ZERO);
	_level->getStage()->addChild(_floor);

	_piston = cocos2d::Sprite::createWithSpriteFrameName("popup-wall-piston.png");
	_piston->setPosition(cocos2d::Vec2::ZERO);
	_piston->setAnchorPoint(cocos2d::Vec2::ZERO);
	_floor->addChild(_piston);

	_front = cocos2d::Sprite::createWithSpriteFrameName("trap-front.png");
	_front->setPosition(cocos2d::Vec2::ZERO);
	_front->setAnchorPoint(cocos2d::Vec2::ZERO);
	_floor->addChild(_front);

	reset();
}

void PopupWall::reset()
{
	_open = true;
	_piston->setPosition(BOTTOM_POSITION);
	_level->getPhysicsWorld()->setBody(_coordinate, TileBody::EMPTY, 7);
	_floor->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::BACK_Z_ORDER);
}

void PopupWall::afterEnter(Creature* /*creature*/)
{
	_level->getPhysicsWorld()->setBody(_coordinate, TileBody::OUTER_BOX, 7);
}

void PopupWall::beforeEscape(Creature* /*creature*/)
{
	if (_open)
	{
		_open = false;
		auto duration = _level->getTurnDuration();

		_floor->runAction(cocos2d::Sequence::create(
			cocos2d::DelayTime::create(0.5f * duration),
			cocos2d::CallFunc::create([this]() {
				_floor->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::WALL_Z_ORDER);

				auto duration = _level->getTurnDuration();
				_piston->runAction(cocos2d::MoveTo::create(duration, TOP_POSITION));
			}),
			nullptr
		));
	}
}
