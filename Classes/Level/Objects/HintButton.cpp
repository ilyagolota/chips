#include "HintButton.h"
#include <Tiled/TiledProjector.h>
#include "Level/Level.h"

HintButton* HintButton::create(Level* level, const cocos2d::Vec2& coordinate)
{
	auto instance = new HintButton(level, coordinate);
    instance->autorelease();
    return instance;
}

HintButton::HintButton(Level* level, const cocos2d::Vec2& coordinate) : LevelObject(level, coordinate)
{
	_floor = cocos2d::Sprite::createWithSpriteFrameName("help.png");
	_floor->setAnchorPoint(cocos2d::Vec2::ZERO);
	_floor->setPosition(_level->getProjector()->coordinateToPoint(_coordinate) + cocos2d::Vec2(0, -12));
	_floor->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::BACK_Z_ORDER);
	_level->getStage()->addChild(_floor);
}
