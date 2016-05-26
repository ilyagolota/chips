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
    _waterSprite = cocos2d::Sprite::create();
    _waterSprite->retain();
    
    auto animation = cocos2d::AnimationCache::getInstance()->getAnimation("water");
    _waterSprite->runAction(cocos2d::RepeatForever::create(cocos2d::Animate::create(animation)));
}

Water::~Water()
{
    _waterSprite->release();
}

void Water::build()
{
    std::vector<int> f;
    
    _waterSprite->setPosition(_level->getProjector()->coordinateToPoint(_coordinate));
    _waterSprite->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) * Level::FRONT_Z_ORDER);
    _level->getStage()->addChild(_waterSprite);
}

void Water::destroy()
{
    _level->getStage()->removeChild(_waterSprite);
}
