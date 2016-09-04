#include "Dirt.h"
#include "Level/Level.h"

Dirt* Dirt::create(Level* level, const cocos2d::Vec2& coordinate)
{
	auto instance = new Dirt(level, coordinate);
    instance->autorelease();
    return instance;
}

Dirt::Dirt(Level* level, const cocos2d::Vec2& coordinate) : LevelObject(level, coordinate)
{
}
