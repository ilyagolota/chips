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
	_blockNode = nullptr;
}

void Water::onAdd()
{
    _node = cocos2d::Sprite::create();
    _node->setAnchorPoint(cocos2d::Vec2::ZERO);
    _node->setPosition(_level->getProjector()->coordinateToPoint(_coordinate) + cocos2d::Vec2(0, -12));
    _node->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::BACK_Z_ORDER);
    _level->getStage()->addChild(_node);
    
    auto animation = cocos2d::AnimationCache::getInstance()->getAnimation("water");
    _node->runAction(cocos2d::RepeatForever::create(cocos2d::Animate::create(animation)));
    
    //    auto emitter = TileSoundEmitter::create("water.mp3");
    //    emitter->setCoordinate(_coordinate);
    //    _level->getSoundEnvironment()->addEmitter(emitter);
}

void Water::reset()
{
    _state = WATER_STATE;
    
    _node->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::BACK_Z_ORDER);
	if (_splash != nullptr)
	{
		_splash->stopAllActions();
		_splash->setVisible(false);
	}
	if (_blockNode != nullptr)
	{
		_blockNode->stopAllActions();
		_blockNode->setVisible(false);
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
		_node->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::BACK_Z_ORDER);
		_blockNode->runAction(cocos2d::Animate::create(cocos2d::AnimationCache::getInstance()->getAnimation("block-drawn")));
		_state = FLOOR_STATE;
	}
	else if (_state == WATER_STATE)
	{
		if (creature->getType() == CreatureType::CHIP && _level->getInventory()->getItemCount(TileType::BOOTS_WATER) > 0)
		{
			creature->getSprite()->stopAllActionsByTag(Creature::CHANGE_STATE_ACTION_TAG);

			if (creature->getState() != CreatureState::SWIMMING)
			{
				auto duration = _level->getTurnDuration() * creature->getTurnsPerMove();
				auto action = cocos2d::Sequence::create(
					cocos2d::DelayTime::create(0.5f * duration),
					cocos2d::CallFunc::create([this, creature]()
					{
						creature->setState(CreatureState::SWIMMING);
						creature->updateAnimation();
					}),
					nullptr
				);
				action->setTag(Creature::CHANGE_STATE_ACTION_TAG);
				creature->getSprite()->runAction(action);
			}
		}
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
			if (_blockNode == nullptr)
			{
				_blockNode = cocos2d::Sprite::create();
				_blockNode->setAnchorPoint(cocos2d::Vec2::ZERO);
				_blockNode->setPosition(cocos2d::Vec2(0, 2));
				_node->addChild(_blockNode);
			}
			_blockNode->setVisible(true);
			_blockNode->runAction(cocos2d::Animate::create(cocos2d::AnimationCache::getInstance()->getAnimation("block-drawn-half")));
			_node->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::WALL_Z_ORDER);
			_state = DIRT_STATE;
		}
		else
		{
			if (_splash == nullptr)
			{
				_splash = cocos2d::Sprite::create();
				_splash->setAnchorPoint(cocos2d::Vec2::ZERO);
				_splash->setPosition(cocos2d::Vec2::ZERO);
				_node->addChild(_splash);
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

void Water::beforeEscape(Creature *creature)
{
	if (creature->getState() == CreatureState::SWIMMING)
	{
		auto duration = _level->getTurnDuration() * creature->getTurnsPerMove();
		auto action = cocos2d::Sequence::create(
			cocos2d::DelayTime::create(0.5f * duration),
			cocos2d::CallFunc::create([this, creature]()
			{
				creature->setState(CreatureState::NORMAL);
				creature->updateAnimation();
			}),
			nullptr
		);
		action->setTag(Creature::CHANGE_STATE_ACTION_TAG);
		creature->getSprite()->runAction(action);
	}
}

bool Water::hasDrawnBlock() const
{
    return false;
}
