#include "Beartrap.h"
#include "Level/Creature.h"

Beartrap* Beartrap::create(const cocos2d::Vec2& coordinate)
{
    auto instance = new Beartrap(coordinate);
    instance->autorelease();
    return instance;
}

Beartrap::Beartrap(const cocos2d::Vec2& coordinate) : LevelObject(coordinate)
{
    _open = false;
    
    /*_sprite = cocos2d::Sprite::createWithSpriteFrameName("floor.png");
    _sprite->setPosition(cocos2d::Vec2::ZERO);
    _sprite->setAnchorPoint(cocos2d::Vec2::ZERO);
    _sprite->retain();
    
    auto holeSprite = cocos2d::Sprite::createWithSpriteFrameName("beartrap.png");
    holeSprite->setPosition(cocos2d::Vec2::ZERO);
    holeSprite->setAnchorPoint(cocos2d::Vec2::ZERO);
    _sprite->addChild(holeSprite);*/
}

bool Beartrap::isOpen() const
{
    return _open;
}

void Beartrap::setOpen(bool open)
{
    _open = open;
}

bool Beartrap::isEscapableBy(const Creature* creature, Direction direction) const
{
    return _open;
}

void Beartrap::afterEnter(Creature* creature)
{
    /*if (!_open)
    {
        auto duration = _level->getTurnDuration() * creature->getTilesPerMove();
    }
    
    creature->getSprite()->runAction();*/
}

void Beartrap::reset()
{
}
