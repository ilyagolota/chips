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
	_splash = nullptr;
	_block = nullptr;
}

Water::~Water()
{
}

void Water::onAdd()
{
    _sprite = cocos2d::Sprite::create();
    _sprite->setAnchorPoint(cocos2d::Vec2::ZERO);
    _sprite->setPosition(_level->getProjector()->coordinateToPoint(_coordinate) + cocos2d::Vec2(0, -12));
    _sprite->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::BACK_Z_ORDER);
    _level->getStage()->addChild(_sprite);
    
    auto animation = cocos2d::AnimationCache::getInstance()->getAnimation("water");
    _sprite->runAction(cocos2d::RepeatForever::create(cocos2d::Animate::create(animation)));
    
    //    auto emitter = TileSoundEmitter::create("water.mp3");
    //    emitter->setCoordinate(_coordinate);
    //    _level->getSoundEnvironment()->addEmitter(emitter);
}

void Water::reset()
{
    _state = WATER_STATE;
    
	if (_splash != nullptr)
	{
		_splash->stopAllActions();
		_splash->setVisible(false);
	}
	if (_block != nullptr)
	{
		_block->stopAllActions();
		_block->setVisible(false);
	}
}

bool Water::isEnterableBy(const Creature *creature, Direction /*direction*/) const
{
	if (_state == DIRT_STATE)
	{
		return creature->getType() == CreatureType::CHIP;
	}
	else
	{
		return true;
	}
}

void Water::beforeEnter(Creature* creature)
{
	if (_state == DIRT_STATE)
	{
		_sprite->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::BACK_Z_ORDER);
		_block->runAction(cocos2d::Animate::create(cocos2d::AnimationCache::getInstance()->getAnimation("block-drawn")));
		_state = FLOOR_STATE;
	}
}

void Water::afterEnter(Creature *creature)
{
	if (_state == WATER_STATE)
	{
		if (creature->getType() == CreatureType::GLIDER)
		{
			return;
		}

		if (creature->getType() == CreatureType::CHIP && _level->getInventory()->getItemCount(TileType::BOOTS_WATER) > 0)
		{
			return;
		}

        creature->die();

		if (creature->getType() == CreatureType::BLOCK)
		{
			if (_block == nullptr)
			{
				_block = cocos2d::Sprite::create();
				_block->setAnchorPoint(cocos2d::Vec2::ZERO);
				_block->setPosition(cocos2d::Vec2(0, 2));
				_sprite->addChild(_block);
			}
			_block->setVisible(true);
			_block->runAction(cocos2d::Animate::create(cocos2d::AnimationCache::getInstance()->getAnimation("block-drawn-half")));
			_sprite->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::WALL_Z_ORDER);
			_state = DIRT_STATE;
		}
		else
		{
			if (_splash == nullptr)
			{
				_splash = cocos2d::Sprite::create();
				_splash->setAnchorPoint(cocos2d::Vec2::ZERO);
				_splash->setPosition(cocos2d::Vec2::ZERO);
				_sprite->addChild(_splash);
			}

			_splash->setVisible(true);
			_splash->stopAllActions();
			_splash->runAction(cocos2d::Sequence::create(
				cocos2d::Animate::create(cocos2d::AnimationCache::getInstance()->getAnimation("splash")),
				cocos2d::CallFuncN::create([](cocos2d::Node* splash) {
					splash->setVisible(false);
				}),
				nullptr
			));

			if (creature->getType() == CreatureType::CHIP)
			{
				_level->fail("Ooops! Chip can't swim without flippers!");
			}
		}
	}
}

bool Water::hasDrawnBlock() const
{
    return false;
}
