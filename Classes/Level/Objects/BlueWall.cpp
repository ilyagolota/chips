#include "BlueWall.h"

BlueWall* BlueWall::create(const cocos2d::Vec2& coordinate, bool fake)
{
    auto instance = new BlueWall(coordinate, fake);
    instance->autorelease();
    return instance;
}

BlueWall::BlueWall(const cocos2d::Vec2& coordinate, bool fake) : LevelObject(coordinate)
{
	_fake = fake;
    _known = false;
}
