#include "Ice.h"
#include <Tiled/TiledProjector.h>
#include <Level/Level.h>
#include <Level/Creature.h>
#include <Level/Inventory.h>

Ice* Ice::create(Level* level, const cocos2d::Vec2& coordinate, TileType type)
{
    auto instance = new Ice(level, coordinate, type);
    instance->autorelease();
    return instance;
}

Ice::Ice(Level* level, const cocos2d::Vec2& coordinate, TileType type) : LevelObject(level, coordinate)
{
    _type = type;
    
    _sprite = cocos2d::Sprite::createWithSpriteFrameName("ice.png");
	_sprite->setAnchorPoint(cocos2d::Vec2::ZERO);
	_sprite->setPosition(_level->getProjector()->coordinateToPoint(_coordinate) + cocos2d::Vec2(0, -12));
	_sprite->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::BACK_Z_ORDER);
	_level->getStage()->addChild(_sprite);
    
	if (_type == TileType::ICE_WALL_SOUTH_WEST)
    {
        _backWall = cocos2d::Sprite::createWithSpriteFrameName("ice-wall-north.png");
		_backWall->setAnchorPoint(cocos2d::Vec2::ZERO);
		_backWall->setPosition(_level->getProjector()->coordinateToPoint(_coordinate));
		_backWall->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::BACK_Z_ORDER);
		_level->getStage()->addChild(_backWall);
        
		_frontWall = cocos2d::Sprite::createWithSpriteFrameName("ice-wall-east.png");
		_frontWall->setAnchorPoint(cocos2d::Vec2::ZERO);
		_frontWall->setPosition(_level->getProjector()->coordinateToPoint(_coordinate));
		_frontWall->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::FRONT_Z_ORDER);
		_level->getStage()->addChild(_frontWall);
    }
	else if (_type == TileType::ICE_WALL_NORTH_WEST)
    {
		_frontWall = cocos2d::Sprite::createWithSpriteFrameName("ice-wall-south-east.png");
		_frontWall->setAnchorPoint(cocos2d::Vec2::ZERO);
		_frontWall->setPosition(_level->getProjector()->coordinateToPoint(_coordinate));
		_frontWall->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::FRONT_Z_ORDER);
		_level->getStage()->addChild(_frontWall);
    }
	else if (_type == TileType::ICE_WALL_SOUTH_EAST)
    {
		_backWall = cocos2d::Sprite::createWithSpriteFrameName("ice-wall-north-west.png");
		_backWall->setAnchorPoint(cocos2d::Vec2::ZERO);
		_backWall->setPosition(_level->getProjector()->coordinateToPoint(_coordinate));
		_backWall->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::BACK_Z_ORDER);
		_level->getStage()->addChild(_backWall);
    }
	else if (_type == TileType::ICE_WALL_NORTH_EAST)
    {
		_backWall = cocos2d::Sprite::createWithSpriteFrameName("ice-wall-west.png");
		_backWall->setAnchorPoint(cocos2d::Vec2::ZERO);
		_backWall->setPosition(_level->getProjector()->coordinateToPoint(_coordinate));
		_backWall->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::BACK_Z_ORDER);
		_level->getStage()->addChild(_backWall);
        
		_frontWall = cocos2d::Sprite::createWithSpriteFrameName("ice-wall-south.png");
		_frontWall->setAnchorPoint(cocos2d::Vec2::ZERO);
		_frontWall->setPosition(_level->getProjector()->coordinateToPoint(_coordinate));
		_frontWall->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::FRONT_Z_ORDER);
		_level->getStage()->addChild(_frontWall);
    }
}

bool Ice::isEnterableBy(const Creature* creature, Direction direction) const
{
	switch (_type)
	{
		case TileType::ICE_WALL_NORTH_EAST:
			return direction == Direction::SOUTH || direction == Direction::WEST;
		case TileType::ICE_WALL_SOUTH_EAST:
			return direction == Direction::NORTH || direction == Direction::WEST;
		case TileType::ICE_WALL_NORTH_WEST:
			return direction == Direction::SOUTH || direction == Direction::EAST;
		case TileType::ICE_WALL_SOUTH_WEST:
			return direction == Direction::NORTH || direction == Direction::EAST;
		default:
			return true;
	}
}

bool Ice::isEscapableBy(const Creature* creature, Direction direction) const
{
	switch (_type)
	{
		case TileType::ICE_WALL_NORTH_EAST:
			return direction == Direction::NORTH || direction == Direction::EAST;
		case TileType::ICE_WALL_SOUTH_EAST:
			return direction == Direction::SOUTH || direction == Direction::EAST;
		case TileType::ICE_WALL_NORTH_WEST:
			return direction == Direction::NORTH || direction == Direction::WEST;
		case TileType::ICE_WALL_SOUTH_WEST:
			return direction == Direction::SOUTH || direction == Direction::WEST;
		default:
			return true;
	}
}

void Ice::beforeEnter(Creature *creature)
{
    if (creature->getType() != CreatureType::CHIP || _level->getInventory()->getItemCount(TileType::BOOTS_ICE) <= 0)
    {
        creature->getSprite()->stopAllActionsByTag(Creature::CHANGE_STATE_ACTION_TAG);
        
        if (creature->getState() != CreatureState::SLIDING)
        {
            auto duration = _level->getTurnDuration() * creature->getTurnsPerMove();
            auto action = cocos2d::Sequence::create(
                cocos2d::DelayTime::create(0.5f * duration),
                cocos2d::CallFunc::create([this, creature]()
			    {
                    creature->setState(CreatureState::SLIDING);
                    creature->updateAnimation();
                }),
                nullptr
            );
            action->setTag(Creature::CHANGE_STATE_ACTION_TAG);
            creature->getSprite()->runAction(action);
        }
    }
}

void Ice::afterEnter(Creature* creature)
{
    if (creature->getType() != CreatureType::CHIP || _level->getInventory()->getItemCount(TileType::BOOTS_ICE) <= 0)
    {
        Direction direction;
        switch (_type)
        {
            case TileType::ICE:
                direction = creature->getDirection();
                break;
            case TileType::ICE_WALL_NORTH_EAST:
                direction = (creature->getDirection() == Direction::SOUTH) ? Direction::EAST : Direction::NORTH;
                break;
            case TileType::ICE_WALL_SOUTH_EAST:
                direction = (creature->getDirection() == Direction::NORTH) ? Direction::EAST : Direction::SOUTH;
                break;
            case TileType::ICE_WALL_NORTH_WEST:
                direction = (creature->getDirection() == Direction::SOUTH) ? Direction::WEST : Direction::NORTH;
                break;
            case TileType::ICE_WALL_SOUTH_WEST:
                direction = (creature->getDirection() == Direction::NORTH) ? Direction::WEST : Direction::SOUTH;
                break;
            default:
                break;
        }
        
        if (creature->canMove(direction))
        {
            creature->move(direction);
        }
        else
        {
            Direction back = inverse(creature->getDirection());
            if (creature->canMove(back))
            {
                creature->move(back);
            }
        }
    }
}

void Ice::beforeEscape(Creature *creature)
{
    if (creature->getState() == CreatureState::SLIDING)
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
