#include "LevelObject.h"

LevelObject::LevelObject(const cocos2d::Vec2& coordinate)
{
    _coordinate = coordinate;
}

cocos2d::Vec2 LevelObject::getCoordinate() const
{
    return _coordinate;
}

Level* LevelObject::getLevel() const
{
    return _level;
}

void LevelObject::setLevel(Level* level)
{
    if (_level != level)
    {
        if (_level != nullptr)
        {
            this->destroyNodes();
        }
        _level = level;
        if (_level != nullptr)
        {
            this->buildNodes();
        }
    }
}

bool LevelObject::isOpenableBy(const Creature *creature, Direction direction) const
{
    return false;
}

bool LevelObject::isEnterableBy(const Creature *creature, Direction direction) const
{
    return true;
}

bool LevelObject::isEscapableBy(const Creature *creature, Direction direction) const
{
    return true;
}

void LevelObject::beforeEnter(Creature* creature)
{
    CC_UNUSED_PARAM(creature);
}

void LevelObject::afterEnter(Creature* creature)
{
    CC_UNUSED_PARAM(creature);
}

void LevelObject::beforeLeave(Creature* creature)
{
    CC_UNUSED_PARAM(creature);
}

void LevelObject::afterLeave(Creature* creature)
{
    CC_UNUSED_PARAM(creature);
}

void LevelObject::reset()
{ }

void LevelObject::buildNodes()
{ }

void LevelObject::destroyNodes()
{ }
