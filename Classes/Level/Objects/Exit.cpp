#include "Exit.h"
#include "Level/Level.h"

Exit* Exit::create(const cocos2d::Vec2& coordinate)
{
	auto instance = new Exit(coordinate);
    instance->autorelease();
    return instance;
}

Exit::Exit(const cocos2d::Vec2& coordinate) : LevelObject(coordinate)
{
}
