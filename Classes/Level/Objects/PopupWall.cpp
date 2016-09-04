#include "PopupWall.h"

PopupWall* PopupWall::create(Level* level, const cocos2d::Vec2& coordinate)
{
    auto instance = new PopupWall(level, coordinate);
    instance->autorelease();
    return instance;
}

PopupWall::PopupWall(Level* level, const cocos2d::Vec2& coordinate) : LevelObject(level, coordinate)
{
}
