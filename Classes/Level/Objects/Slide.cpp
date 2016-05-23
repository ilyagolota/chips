#include "Slide.h"

Slide* Slide::create(const cocos2d::Vec2& coordinate, TileType tileType)
{
    auto instance = new Slide(coordinate, tileType);
    instance->autorelease();
    return instance;
}

Slide::Slide(const cocos2d::Vec2& coordinate, TileType tileType) : LevelObject(coordinate)
{
    _type = tileType;
}
