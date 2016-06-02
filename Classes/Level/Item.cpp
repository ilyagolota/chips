#include "Item.h"
#include <cocos2d.h>
#include <Tiled/TiledProjector.h>
#include "Level.h"
#include "Creature.h"
#include "Inventory.h"

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
    _picked = false;
    _sprite = cocos2d::Sprite::createWithSpriteFrameName(_getSpriteFrameName());
    _sprite->setAnchorPoint(cocos2d::Vec2::ZERO);
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
            _sprite->setPosition(_level->getProjector()->coordinateToPoint(_coordinate));
            _sprite->setZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) * Level::Z_ORDER_PER_TILE + Level::ITEM_Z_ORDER);
            _level->getStage()->addChild(_sprite);
        }
    }
}

bool Item::isPickableBy(const Creature* creature) const
{
    return _type == TileType::KEY_BLUE || creature->getType() == CreatureType::CHIP;
}

void Item::pick(Creature* creature)
{
    if (!_picked)
    {
        _picked = true;
        
        if (creature->getType() == CreatureType::CHIP)
        {
            _level->getInventory()->addItem(_type, 1);
        }
        
        _sprite->setPosition(_sprite->getPosition() + cocos2d::Vec2(0.5f * _sprite->getContentSize().width, 128));
        _sprite->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
        _sprite->setOpacity(128);
        _sprite->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) * Level::Z_ORDER_PER_TILE + Level::WALL_Z_ORDER);
        
        auto duration = 1.2f * _level->getTurnDuration();

        auto flyAction = cocos2d::MoveBy::create(duration, cocos2d::Vec2(0, 44));
        auto fadeOutAction = cocos2d::FadeTo::create(duration, 0);
        auto scaleAction = cocos2d::ScaleTo::create(duration, 1.5f);
        auto spawnAction = cocos2d::Spawn::create(flyAction, scaleAction, fadeOutAction, nullptr);
        auto callbackAction = cocos2d::CallFunc::create([this](){
            _sprite->setVisible(false);
        });
        _sprite->runAction(cocos2d::Sequence::createWithTwoActions(spawnAction, callbackAction));
    }
}

std::string& Item::_getSpriteFrameName() const
{
    static std::string names[] =
    {
        "boots-fire.png"
    };
    return names[0];
}
