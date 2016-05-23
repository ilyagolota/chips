#include "Burglar.h"

Burglar* Burglar::create(const cocos2d::Vec2& coordinate)
{
    auto instance = new Burglar(coordinate);
    instance->autorelease();
    return instance;
}

Burglar::Burglar(const cocos2d::Vec2& coordinate) : LevelObject(coordinate)
{
}
