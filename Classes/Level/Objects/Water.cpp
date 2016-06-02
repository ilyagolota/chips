#include "Water.h"
#include <Tiled/TiledProjector.h>
#include <Level/Level.h>

Water* Water::create(const cocos2d::Vec2& coordinate)
{
    auto instance = new Water(coordinate);
    instance->autorelease();
    return instance;
}

Water::Water(const cocos2d::Vec2& coordinate) : LevelObject(coordinate)
{
    auto sprite = cocos2d::Sprite::create();
    sprite->setAnchorPoint(cocos2d::Vec2::ZERO);
    sprite->setPosition(cocos2d::Vec2(0, -12));
    sprite->setLocalZOrder(0);
    
    auto animation = cocos2d::AnimationCache::getInstance()->getAnimation("water");
    sprite->runAction(cocos2d::RepeatForever::create(cocos2d::Animate::create(animation)));
    
    addNode(sprite);
}

Water::~Water()
{
}

bool Water::hasDrawnBlock() const
{
    return false;
}
