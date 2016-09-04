#include "Exit.h"
#include "Level/Level.h"

Exit* Exit::create(Level* level, const cocos2d::Vec2& coordinate)
{
	auto instance = new Exit(level, coordinate);
    instance->autorelease();
    return instance;
}

Exit::Exit(Level* level, const cocos2d::Vec2& coordinate) : LevelObject(level, coordinate)
{
}
