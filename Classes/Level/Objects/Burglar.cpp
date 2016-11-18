#include "Burglar.h"
#include "../Level.h"

Burglar* Burglar::create(Level* level, const cocos2d::Vec2& coordinate)
{
    auto instance = new Burglar(level, coordinate);
    instance->autorelease();
    return instance;
}

Burglar::Burglar(Level* level, const cocos2d::Vec2& coordinate) : LevelObject(level, coordinate)
{
}

void Burglar::onAdd()
{
    _node = cocos2d::Sprite::createWithSpriteFrameName("floor.png");
    _node->setAnchorPoint(cocos2d::Vec2::ZERO);
    _node->setPosition(_level->getProjector()->coordinateToPoint(_coordinate) + cocos2d::Vec2(0, -12));
    _node->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::BACK_Z_ORDER);
    _level->getStage()->addChild(_node);
    
    _burglarNode = cocos2d::Sprite::create();
    _burglarNode->setAnchorPoint(cocos2d::Vec2::ZERO);
    _burglarNode->setPosition(cocos2d::Vec2(0, 44));
    _node->addChild(_burglarNode);
    
    auto animation = cocos2d::AnimationCache::getInstance()->getAnimation("chip-stay-east");
    _burglarNode->runAction(cocos2d::RepeatForever::create(cocos2d::Animate::create(animation)));
}

bool Burglar::isEnterableBy(const Creature* creature, Direction direction) const
{
    return (creature->getType() == CreatureType::CHIP);
}

void Burglar::afterEnter(Creature* creature)
{
    if (creature->getType() == CreatureType::CHIP)
    {
        auto inventory = _level->getInventory();
        inventory->setItemCount(TileType::BOOTS_FIRE, 0);
        inventory->setItemCount(TileType::BOOTS_SLIDE, 0);
        inventory->setItemCount(TileType::BOOTS_WATER, 0);
        inventory->setItemCount(TileType::BOOTS_ICE, 0);
    }
}
