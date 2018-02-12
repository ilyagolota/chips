#include "Exit.h"
#include "Level/Level.h"

Exit* Exit::create(Level* level, const cocos2d::Vec2& coordinate)
{
    auto instance = new Exit(level, coordinate);
    instance->autorelease();
    return instance;
}

Exit::Exit(Level* level, const cocos2d::Vec2& coordinate) : LevelObject(level, coordinate)
{
}

void Exit::onAdd()
{
    _node = cocos2d::Sprite::create();
    _node->setAnchorPoint(cocos2d::Vec2::ZERO);
    _node->setPosition(_level->getProjector()->coordinateToPoint(_coordinate) + cocos2d::Vec2(0, -12));
    _node->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::BACK_Z_ORDER);
    _level->getStage()->addChild(_node);
    
    auto animation = cocos2d::AnimationCache::getInstance()->getAnimation("water");
    _node->runAction(cocos2d::RepeatForever::create(cocos2d::Animate::create(animation)));
}

bool Exit::isEnterableBy(const Creature* creature, Direction /*direction*/) const
{
    return (creature->getType() == CreatureType::CHIP || creature->getType() == CreatureType::BLOCK);
}

void Exit::afterEnter(Creature *creature)
{
    if (creature->getType() == CreatureType::CHIP)
    {
        _level->win();
    }
}
