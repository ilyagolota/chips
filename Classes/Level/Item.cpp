#include "Item.h"
#include <cocos2d.h>
#include "Level.h"

Item* Item::create(const cocos2d::Vec2& coordinate, TileType type)
{
    auto instance = new Item(coordinate, type);
    instance->autorelease();
    return instance;
}

Item::Item(const cocos2d::Vec2& coordinate, TileType type)
{
    _level = nullptr;
    _coordinate = coordinate;
    _type = type;
    _sprite = cocos2d::Sprite::create();
    _sprite->retain();
}

cocos2d::Vec2 Item::getCoordinate() const
{
    return _coordinate;
}

Level* Item::getLevel() const
{
    return _level;
}

void Item::setLevel(Level* level)
{
    if (_level != level)
    {
        if (_level != nullptr)
        {
            _level->getStage()->removeChild(_sprite);
        }
        _level = level;
        if (_level != nullptr)
        {
            //_sprite->setPosition();
            _level->getStage()->addChild(_sprite);
        }
    }
}
