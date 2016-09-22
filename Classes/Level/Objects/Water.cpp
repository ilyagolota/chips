#include "Water.h"
#include <Level/Level.h>

Water* Water::create(Level* level, const cocos2d::Vec2& coordinate)
{
    auto instance = new Water(level, coordinate);
    instance->autorelease();
    return instance;
}

Water::Water(Level* level, const cocos2d::Vec2& coordinate) : LevelObject(level, coordinate)
{
	_sprite = cocos2d::Sprite::create();
	_sprite->setAnchorPoint(cocos2d::Vec2::ZERO);
	_sprite->setPosition(_level->getProjector()->coordinateToPoint(_coordinate) + cocos2d::Vec2(0, -12));
	_sprite->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::BACK_Z_ORDER);
	_level->getStage()->addChild(_sprite);
    
    auto animation = cocos2d::AnimationCache::getInstance()->getAnimation("water");
	_sprite->runAction(cocos2d::RepeatForever::create(cocos2d::Animate::create(animation)));

	auto emitter = TileSoundEmitter::create("water.mp3");
	emitter->setCoordinate(_coordinate);
	_level->getSoundEnvironment()->addEmitter(emitter);
}

Water::~Water()
{
}

void Water::afterEnter(Creature *creature)
{
    if (creature->getType() == CreatureType::GLIDER)
    {
        return;
    }
    
    if (creature->getType() == CreatureType::CHIP && _level->getInventory()->getItemCount(TileType::BOOTS_WATER) > 0)
    {
        return;
    }
    
    _level->removeCreature(creature);
    if (creature->getType() == CreatureType::CHIP)
    {
        _level->fail("Ooops! Chip can't swim without flippers!");
    }
}

bool Water::hasDrawnBlock() const
{
    return false;
}
