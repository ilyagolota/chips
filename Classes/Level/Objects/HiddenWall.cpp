#include "HiddenWall.h"
#include "Level/Level.h"

HiddenWall* HiddenWall::create(Level* level, const cocos2d::Vec2& coordinate, bool permanent)
{
	auto instance = new HiddenWall(level, coordinate, permanent);
    instance->autorelease();
    return instance;
}

HiddenWall::HiddenWall(Level* level, const cocos2d::Vec2& coordinate, bool permanent) : LevelObject(level, coordinate)
{
	_permanent = permanent;
	_known = false;
}

/*void HiddenWall::buildNodes()
{
    _level->setCollision(_coordinate, 0xFFFFFF);
    _level->getSoundWorld()->setSoundSource(_coordinate, );
    
    _sprite->setPosition(_level->coordinateToPosition());
    _level->getStage()->addChild(_sprite);
}*/
