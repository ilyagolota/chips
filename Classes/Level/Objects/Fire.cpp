#include "Fire.h"
#include <Level/Level.h>

Fire* Fire::create(Level* level, const cocos2d::Vec2& coordinate)
{
	auto instance = new Fire(level, coordinate);
    instance->autorelease();
    return instance;
}

Fire::Fire(Level* level, const cocos2d::Vec2& coordinate) : LevelObject(level, coordinate)
{
}

void Fire::onAdd()
{
    _node = cocos2d::Sprite::createWithSpriteFrameName("fire-floor.png");
    _node->setAnchorPoint(cocos2d::Vec2::ZERO);
    _node->setPosition(_level->getProjector()->coordinateToPoint(_coordinate) + cocos2d::Vec2(0, -12));
	_node->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::ITEM_Z_ORDER);
    _level->getStage()->addChild(_node);
    
    _fire1Node = cocos2d::Sprite::create();
	_fire1Node->setScale(-1, 1);
    _fire1Node->setAnchorPoint(cocos2d::Vec2(0.5f, 0));
    _fire1Node->setPosition(cocos2d::Vec2(62, 68));
	_fire1Node->runAction(cocos2d::RepeatForever::create(cocos2d::Animate::create(cocos2d::AnimationCache::getInstance()->getAnimation("fire1"))));
	_node->addChild(_fire1Node);
    
    _fire2Node = cocos2d::Sprite::create();
	_fire2Node->setAnchorPoint(cocos2d::Vec2(0.5f, 0));
    _fire2Node->setPosition(cocos2d::Vec2(113, 76));
	_fire2Node->runAction(cocos2d::RepeatForever::create(cocos2d::Animate::create(cocos2d::AnimationCache::getInstance()->getAnimation("fire2"))));
	_node->addChild(_fire2Node);

    _fire3Node = cocos2d::Sprite::create();
    _fire3Node->setAnchorPoint(cocos2d::Vec2(0.5f, 0));
    _fire3Node->setPosition(_level->getProjector()->coordinateToPoint(_coordinate) + cocos2d::Vec2(92, 17));
    _fire3Node->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::WALL_Z_ORDER);
	_fire3Node->runAction(cocos2d::RepeatForever::create(cocos2d::Animate::create(cocos2d::AnimationCache::getInstance()->getAnimation("fire3"))));
    _level->getStage()->addChild(_fire3Node);
}

bool Fire::isEnterableBy(const Creature *creature, Direction direction) const
{
    return (creature->getType() != CreatureType::BUG);
}

void Fire::afterEnter(Creature *creature)
{
    if (creature->getType() == CreatureType::FIREBALL)
    {
        return;
    }
    
    if (creature->getType() == CreatureType::CHIP && _level->getInventory()->getItemCount(TileType::BOOTS_FIRE) > 0)
    {
        return;
    }
    
    creature->die();
    
    if (creature->getType() == CreatureType::CHIP)
    {
        _level->fail("Ooops! Don't step in the fire without fire boots!");
    }
}
