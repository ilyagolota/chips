#include "Bomb.h"

Bomb* Bomb::create(Level* level, const cocos2d::Vec2& coordinate)
{
	auto instance = new Bomb(level, coordinate);
    instance->autorelease();
    return instance;
}

Bomb::Bomb(Level* level, const cocos2d::Vec2& coordinate) : LevelObject(level, coordinate)
{
	_exploded = false;
}
