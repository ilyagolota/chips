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
    
    //_sprites[0]->runAction();
    
    //clonedCreature->setLocalZOrder();
    //clonedCreature->getSprite()->runAction();
    //TODO: animation
}

void CloneMachine::reset()
{
    _working = false;
}
