#include "Dirt.h"
#include "Level/Level.h"

Dirt* Dirt::create(const cocos2d::Vec2& coordinate)
{
	auto instance = new Dirt(coordinate);
    instance->autorelease();
    return instance;
}

Dirt::Dirt(const cocos2d::Vec2& coordinate) : LevelObject(coordinate)
{
}
