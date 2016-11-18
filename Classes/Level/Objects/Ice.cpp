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
}

void Ice::onAdd()
{
    _node = cocos2d::Sprite::createWithSpriteFrameName("ice.png");
	_node->setAnchorPoint(cocos2d::Vec2::ZERO);
	_node->setPosition(_level->getProjector()->coordinateToPoint(_coordinate) + cocos2d::Vec2(0, -12));
	_node->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::BACK_Z_ORDER);
	_level->getStage()->addChild(_node);
    
	if (_type == TileType::ICE_WALL_SOUTH_WEST)
    {
        _backNode = cocos2d::Sprite::createWithSpriteFrameName("ice-wall-north.png");
		_backNode->setAnchorPoint(cocos2d::Vec2::ZERO);
		_backNode->setPosition(_level->getProjector()->coordinateToPoint(_coordinate) + cocos2d::Vec2(0, -4));
		_backNode->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::BACK_Z_ORDER);
		_level->getStage()->addChild(_backNode);
        
		_frontNode = cocos2d::Sprite::createWithSpriteFrameName("ice-wall-east.png");
		_frontNode->setAnchorPoint(cocos2d::Vec2::ZERO);
		_frontNode->setPosition(_level->getProjector()->coordinateToPoint(_coordinate) + cocos2d::Vec2(0, -4));
		_frontNode->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::FRONT_Z_ORDER);
		_level->getStage()->addChild(_frontNode);
    }
	else if (_type == TileType::ICE_WALL_NORTH_WEST)
    {
		_frontNode = cocos2d::Sprite::createWithSpriteFrameName("ice-wall-south-east.png");
		_frontNode->setAnchorPoint(cocos2d::Vec2::ZERO);
		_frontNode->setPosition(_level->getProjector()->coordinateToPoint(_coordinate) + cocos2d::Vec2(0, -4));
		_frontNode->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::FRONT_Z_ORDER);
		_level->getStage()->addChild(_frontNode);
    }
	else if (_type == TileType::ICE_WALL_SOUTH_EAST)
    {
		_backNode = cocos2d::Sprite::createWithSpriteFrameName("ice-wall-north-west.png");
		_backNode->setAnchorPoint(cocos2d::Vec2::ZERO);
		_backNode->setPosition(_level->getProjector()->coordinateToPoint(_coordinate) + cocos2d::Vec2(0, -4));
		_backNode->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::BACK_Z_ORDER);
		_level->getStage()->addChild(_backNode);
    }
	else if (_type == TileType::ICE_WALL_NORTH_EAST)
    {
		_backNode = cocos2d::Sprite::createWithSpriteFrameName("ice-wall-west.png");
		_backNode->setAnchorPoint(cocos2d::Vec2::ZERO);
		_backNode->setPosition(_level->getProjector()->coordinateToPoint(_coordinate) + cocos2d::Vec2(0, -4));
		_backNode->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::BACK_Z_ORDER);
		_level->getStage()->addChild(_backNode);
        
		_frontNode = cocos2d::Sprite::createWithSpriteFrameName("ice-wall-south.png");
		_frontNode->setAnchorPoint(cocos2d::Vec2::ZERO);
		_frontNode->setPosition(_level->getProjector()->coordinateToPoint(_coordinate) + cocos2d::Vec2(0, -4));
		_frontNode->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::FRONT_Z_ORDER);
		_level->getStage()->addChild(_frontNode);
    }

	TileBody body;
	switch (_type)
	{
	case TileType::ICE_WALL_NORTH_EAST:
		body = TileBody::INNER_SOUTH_SIDE | TileBody::INNER_WEST_SIDE | TileBody::OUTER_NORTH_SIDE | TileBody::OUTER_EAST_SIDE;
		break;
	case TileType::ICE_WALL_SOUTH_EAST:
		body = TileBody::INNER_NORTH_SIDE | TileBody::INNER_WEST_SIDE | TileBody::OUTER_SOUTH_SIDE | TileBody::OUTER_EAST_SIDE;
		break;
	case TileType::ICE_WALL_NORTH_WEST:
		body = TileBody::INNER_SOUTH_SIDE | TileBody::INNER_EAST_SIDE | TileBody::OUTER_NORTH_SIDE | TileBody::OUTER_WEST_SIDE;
		break;
	case TileType::ICE_WALL_SOUTH_WEST:
		body = TileBody::INNER_NORTH_SIDE | TileBody::INNER_EAST_SIDE | TileBody::OUTER_SOUTH_SIDE | TileBody::OUTER_WEST_SIDE;
		break;
	default:
		body = TileBody::EMPTY;
		break;
	}

	if (body != TileBody::EMPTY)
	{
		_level->getPhysicsWorld()->setBody(_coordinate, body, 7);
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
			creature->queueMove(direction);
        }
        else
        {
            Direction back = inverse(creature->getDirection());
            if (creature->canMove(back))
            {
                creature->queueMove(back);
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
