#include "Teleport.h"

Teleport* Teleport::create(const cocos2d::Vec2& coordinate)
{
    auto instance = new Teleport(coordinate);
    instance->autorelease();
    return instance;
}

Teleport::Teleport(const cocos2d::Vec2& coordinate) : LevelObject(coordinate)
{
}
