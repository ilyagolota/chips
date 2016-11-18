#include "Dirt.h"
#include <Level/Level.h>

Dirt* Dirt::create(Level* level, const cocos2d::Vec2& coordinate)
{
    auto instance = new Dirt(level, coordinate);
    instance->autorelease();
    return instance;
}

Dirt::Dirt(Level* level, const cocos2d::Vec2& coordinate) : LevelObject(level, coordinate)
{
    _removed = false;
}

void Dirt::onAdd()
{
    _node = cocos2d::Sprite::createWithSpriteFrameName("button-floor.png");
    _node->setAnchorPoint(cocos2d::Vec2::ZERO);
    _node->setPosition(_level->getProjector()->coordinateToPoint(_coordinate) + cocos2d::Vec2(0, -12));
    _node->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::WALL_Z_ORDER);
    _level->getStage()->addChild(_node);
    
    _dirtNode = cocos2d::Sprite::createWithSpriteFrameName("button-brown.png");
    _dirtNode->setAnchorPoint(cocos2d::Vec2::ZERO);
    _dirtNode->setPosition(cocos2d::Vec2::ZERO);
    _node->addChild(_dirtNode);
}

void Dirt::reset()
{
    _removed = false;
    
    _node->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::WALL_Z_ORDER);
    
    _dirtNode->setVisible(true);
    _dirtNode->setSpriteFrame(cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName("button-brown.png"));
}

bool Dirt::isEnterableBy(const Creature* creature, Direction /*direction*/) const
{
    return _removed || (creature->getType() == CreatureType::CHIP);
}

void Dirt::afterEnter(Creature *creature)
{
    if (_removed)
    {
        return;
    }
    
    _node->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::BACK_Z_ORDER);
    
    _dirtNode->runAction(cocos2d::Sequence::create(
        cocos2d::Animate::create(cocos2d::AnimationCache::getInstance()->getAnimation("splash")),
        cocos2d::CallFuncN::create([](cocos2d::Node* node) {
            node->setVisible(false);
        }),
        nullptr
    ));
    
    _removed = true;
}
