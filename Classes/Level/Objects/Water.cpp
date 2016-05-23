#include "Water.h"

Water* Water::create(const cocos2d::Vec2& coordinate)
{
    auto instance = new Water(coordinate);
    instance->autorelease();
    return instance;
}

Water::Water(const cocos2d::Vec2& coordinate) : LevelObject(coordinate)
{
}
