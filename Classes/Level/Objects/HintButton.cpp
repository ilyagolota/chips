#include "HintButton.h"
#include "Level/Level.h"

HintButton* HintButton::create(const cocos2d::Vec2& coordinate)
{
	auto instance = new HintButton(coordinate);
    instance->autorelease();
    return instance;
}

HintButton::HintButton(const cocos2d::Vec2& coordinate) : LevelObject(coordinate)
{
}
