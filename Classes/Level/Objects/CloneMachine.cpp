#include "CloneMachine.h"
#include "Level/Level.h"
#include "Level/Creature.h"

CloneMachine* CloneMachine::create(Level* level, const cocos2d::Vec2& coordinate)
{
	auto instance = new CloneMachine(level, coordinate);
    instance->autorelease();
    return instance;
}

CloneMachine::CloneMachine(Level* level, const cocos2d::Vec2& coordinate) : LevelObject(level, coordinate)
{
    _working = false;
    
    _floor = cocos2d::Sprite::createWithSpriteFrameName("trap2.png");
    _floor->setPosition(_level->getProjector()->coordinateToPoint(_coordinate) + cocos2d::Vec2(0, -12));
    _floor->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::BACK_Z_ORDER);
    _floor->setAnchorPoint(cocos2d::Vec2::ZERO);
    _level->getStage()->addChild(_floor);
    
    _front = cocos2d::Sprite::createWithSpriteFrameName("trap2-front.png");
    _front->setPosition(_level->getProjector()->coordinateToPoint(_coordinate) + cocos2d::Vec2(0, -12));
    _front->setAnchorPoint(cocos2d::Vec2::ZERO);
    _front->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::ITEM_Z_ORDER);
    _level->getStage()->addChild(_front);
}

void CloneMachine::performCloning()
{
    // Now creature can leave cloner and it will be cloned in `beforeEscape` method.
    _working = true;
}

bool CloneMachine::isEnterableBy(const Creature* creature, Direction direction) const
{
    return false;
}

bool CloneMachine::isEscapableBy(const Creature* creature, Direction direction) const
{
    return _working;
}

void CloneMachine::beforeEscape(Creature* creature)
{
    _working = false;

    auto clonedCreature = Creature::create(creature->getLevel(), creature->getType());
    clonedCreature->setDirection(creature->getDirection());
	clonedCreature->setCoordinate(_coordinate);
    _level->addCreature(clonedCreature);
    
    auto sprite = clonedCreature->getSprite();
    sprite->setPosition(_level->getProjector()->coordinateToPoint(_coordinate) + cocos2d::Vec2(0, -88));
    sprite->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::BACK_Z_ORDER);
    sprite->runAction(cocos2d::Sequence::create(
        cocos2d::MoveTo::create(_level->getTurnDuration() * 0.9f, _level->getProjector()->coordinateToPoint(_coordinate)),
        cocos2d::CallFuncN::create([this, creature](cocos2d::Node* sprite) {
            sprite->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + creature->getZOrderDelta());
        }),
        nullptr
    ));
}

void CloneMachine::reset()
{
    _working = false;
}
