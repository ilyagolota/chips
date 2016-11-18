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
	reset();
}

void Bomb::reset()
{
    _exploded = false;
}

void Bomb::afterEnter(Creature *creature)
{
    if (!_exploded)
    {
        _exploded = true;
        creature->die();
        if (creature->getType() == CreatureType::CHIP)
        {
            _level->fail("Ooops! Don't touch the bombs!");
        }
    }
}
