#include "LevelObject.h"
#include <Tiled/TiledProjector.h>
#include <Level/Level.h>

LevelObject::LevelObject(Level* level, const cocos2d::Vec2& coordinate)
{
	_level = level;
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

void LevelObject::beforeEscape(Creature* creature)
{
    CC_UNUSED_PARAM(creature);
}

void LevelObject::afterEscape(Creature* creature)
{
    CC_UNUSED_PARAM(creature);
}

void LevelObject::reset()
{ }
