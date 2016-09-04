#include "Burglar.h"

Burglar* Burglar::create(Level* level, const cocos2d::Vec2& coordinate)
{
    auto instance = new Burglar(level, coordinate);
    instance->autorelease();
    return instance;
}

Burglar::Burglar(Level* level, const cocos2d::Vec2& coordinate) : LevelObject(level, coordinate)
{
}
