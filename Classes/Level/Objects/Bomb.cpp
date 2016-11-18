#include "Bomb.h"
#include <Level/Level.h>

Bomb* Bomb::create(Level* level, const cocos2d::Vec2& coordinate)
{
	auto instance = new Bomb(level, coordinate);
    instance->autorelease();
    return instance;
}

Bomb::Bomb(Level* level, const cocos2d::Vec2& coordinate) : LevelObject(level, coordinate)
{
	_exploded = false;
}

void Bomb::onAdd()
{
    _node = cocos2d::Sprite::createWithSpriteFrameName("button-floor.png");
    _node->setAnchorPoint(cocos2d::Vec2::ZERO);
    _node->setPosition(_level->getProjector()->coordinateToPoint(_coordinate) + cocos2d::Vec2(0, -12));
    _node->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate));
    _level->getStage()->addChild(_node);
    
    _bombNode = cocos2d::Sprite::createWithSpriteFrameName("button-red.png");
    _bombNode->setAnchorPoint(cocos2d::Vec2::ZERO);
    _bombNode->setPosition(cocos2d::Vec2::ZERO);
    _node->addChild(_bombNode);
}

void Bomb::reset()
{
    _exploded = false;
    _bombNode->setVisible(true);
}

void Bomb::afterEnter(Creature *creature)
{
    if (_exploded)
    {
        return;
    }
    
    _bombNode->setVisible(false);
    
    auto explosionNode = cocos2d::Sprite::create();
    explosionNode->setAnchorPoint(cocos2d::Vec2::ZERO);
    explosionNode->setPosition(cocos2d::Vec2::ZERO);
    explosionNode->setPosition(_level->getProjector()->coordinateToPoint(_coordinate));
    explosionNode->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::WALL_Z_ORDER);
    _level->getStage()->addChild(explosionNode);
    
    explosionNode->runAction(cocos2d::Sequence::create(
        cocos2d::Animate::create(cocos2d::AnimationCache::getInstance()->getAnimation("splash")),
        cocos2d::CallFuncN::create([](cocos2d::Node* node) {
            node->removeFromParent();
        }),
        nullptr
    ));
    
    _exploded = true;
    creature->die();
    if (creature->getType() == CreatureType::CHIP)
    {
        _level->fail("Ooops! Don't touch the bombs!");
    }
}
