#include "PopupWall.h"

PopupWall* PopupWall::create(const cocos2d::Vec2& coordinate)
{
    auto instance = new PopupWall(coordinate);
    instance->autorelease();
    return instance;
}

PopupWall::PopupWall(const cocos2d::Vec2& coordinate) : LevelObject(coordinate)
{
}
