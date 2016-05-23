#include "Bomb.h"

Bomb* Bomb::create(const cocos2d::Vec2& coordinate)
{
	auto instance = new Bomb(coordinate);
    instance->autorelease();
    return instance;
}

Bomb::Bomb(const cocos2d::Vec2& coordinate) : LevelObject(coordinate)
{
	_exploded = false;
}
