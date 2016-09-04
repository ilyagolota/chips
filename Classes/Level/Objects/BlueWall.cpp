#include "BlueWall.h"

BlueWall* BlueWall::create(Level* level, const cocos2d::Vec2& coordinate, bool fake)
{
    auto instance = new BlueWall(level, coordinate, fake);
    instance->autorelease();
    return instance;
}

BlueWall::BlueWall(Level* level, const cocos2d::Vec2& coordinate, bool fake) : LevelObject(level, coordinate)
{
	_fake = fake;
    _known = false;
}
