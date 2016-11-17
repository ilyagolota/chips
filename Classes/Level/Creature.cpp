#include "Creature.h"

#include <cocos2d.h>

#include "Level.h"
#include "Item.h"
#include "Objects/LevelObject.h"

namespace std
{
	string& to_string(CreatureType type)
	{
		static string names[] =
		{
			"block",
			"bug",
			"fireball",
			"ball",
			"tank",
			"glider",
			"teeth",
			"walker",
			"blob",
			"paramecium",
			"chip"
		};
		return names[static_cast<size_t>(type)];
	}
}

Creature* Creature::create(Level* level, CreatureType type)
{
    auto instance = new Creature(level, type);
    instance->autorelease();
    return instance;
}

Creature::Creature(Level* level, CreatureType type):
	_level(level),
	_type(type),
	_direction(Direction::NORTH),
	_state(CreatureState::NORMAL),
	_coordinate(cocos2d::Vec2::ZERO),
	_turnsToNextMove(0),
	_queuedMove(false),
	_queuedDirection(Direction::NORTH)
{
    _sprite = cocos2d::Sprite::create();
    _sprite->setAnchorPoint(cocos2d::Point::ZERO);
    _sprite->retain();
    
    _soundEmitter = nullptr;
}

Creature::~Creature()
{
    _sprite->release();
}

void Creature::queueMove(Direction direction)
{
	_queuedMove = true;
	_queuedDirection = direction;
}

bool Creature::hasQueuedMove()
{
	return _queuedMove;
}

bool Creature::isMoving() const
{
    return (_turnsToNextMove > 0);
}

int Creature::getTurnsToNextMove() const
{
    return _turnsToNextMove;
}

int Creature::getTurnsPerMove() const
{
    if (_type == CreatureType::TEETH || _type == CreatureType::BLOB)
    {
        return 2;
    }
    else
    {
        return 1;
    }
}

int Creature::getZOrderDelta() const
{
    if (_type == CreatureType::CHIP)
    {
        return Level::CREATURE_SMALL_Z_ORDER;
    }
    else if (_type == CreatureType::BLOCK)
    {
        return Level::CREATURE_BIG_Z_ORDER;
    }
    else
    {
        return Level::CREATURE_MEDIUM_Z_ORDER;
    }
}

int Creature::getPhysicsLayerMask() const
{
    if (_type == CreatureType::CHIP)
    {
        return 0x01;
    }
    else if (_type == CreatureType::BLOCK)
    {
        return 0x02;
    }
    else
    {
        return 0x04;
    }
}

cocos2d::Sprite *Creature::getSprite() const
{
    return _sprite;
}

CreatureType Creature::getType() const
{
    return _type;
}

cocos2d::Vec2 Creature::getCoordinate() const
{
    return _coordinate;
}

void Creature::setCoordinate(const cocos2d::Vec2 &coordinate)
{
    _coordinate = coordinate;
    _turnsToNextMove = 0;
    if (_level != nullptr)
    {
        _updatePosition();
    }
}

Direction Creature::getDirection() const
{
    return _direction;
}

void Creature::setDirection(Direction value)
{
    if (_direction != value)
    {
		_direction = value;
    }
}

CreatureState Creature::getState() const
{
    return _state;
}

void Creature::setState(CreatureState state)
{
    if (_state != state)
    {
        _state = state;
    }
}

Level* Creature::getLevel() const
{
    return _level;
}

void Creature::onTurn(float dt)
{
    if (_turnsToNextMove > 0)
    {
        _turnsToNextMove -= 1;
        if (_turnsToNextMove == 0)
        {
            _sprite->stopAllActionsByTag(MOVE_ACTION_TAG);
            _updatePosition();
            
            auto object = _level->getObjectAt(_coordinate);
            if (object != nullptr)
            {
                object->afterEnter(this);
            }
            
            auto item = _level->getItemAt(_coordinate);
            if (item != nullptr)
            {
                item->pick(this);
            }
        }
    }
    
    if (_type == CreatureType::CHIP)
    {
        for (auto creature : _level->getCreatures())
        {
            if (creature->getCoordinate() == _coordinate && creature != this)
            {
                _level->removeCreature(this);
                if (creature->_type == CreatureType::BLOCK)
                {
                    _level->fail("Ooops! Watch out for moving blocks!");
                }
                else
                {
                    _level->fail("Ooops! Look out for creatures!");
                }
            }
        }
    }
    
    _tryMoveNext();
    updateAnimation();
}

void Creature::onAdd()
{
    _level->getStage()->addChild(_sprite);
     _updatePosition();
    
    _applyAnimationParams();
    _updateAnimation();
    _updateFlip();
    
    if (_soundEmitter != nullptr)
    {
        _level->getSoundEnvironment()->addEmitter(_soundEmitter);
    }
    
    //auto escapedObject = _level->getObjectAt(_coordinate - toVec2(_direction));
    //if (escapedObject != nullptr)
    //{
    //    escapedObject->afterEscape(this);
    //}
    
    auto object = _level->getObjectAt(_coordinate);
    if (object != nullptr)
    {
        object->afterEnter(this);
    }
    
    auto item = _level->getItemAt(_coordinate);
    if (item != nullptr)
    {
        item->pick(this);
    }
}

void Creature::onRemove()
{
    auto object = _level->getObjectAt(_coordinate);
    if (object != nullptr)
    {
        object->afterEscape(this);
    }
    
    _level->getStage()->removeChild(_sprite);
    
    if (_soundEmitter != nullptr)
    {
        _level->getSoundEnvironment()->removeEmitter(_soundEmitter);
    }
}

bool Creature::canMove(Direction direction) const
{
	int mask = getPhysicsLayerMask();
	auto physicsWorld = _level->getPhysicsWorld();
	
	if ((physicsWorld->getBody(_coordinate, mask) & (TileBody::INNER_NORTH_SIDE << direction)) != TileBody::EMPTY)
	{
		return false;
	}
	auto currentObject = _level->getObjectAt(_coordinate);
	if (currentObject != nullptr && !currentObject->isEscapableBy(this, direction))
	{
		return false;
	}

	auto targetCoordinate = _coordinate + toVec2(direction);
	auto frontObject = _level->getObjectAt(targetCoordinate);
	if ((physicsWorld->getBody(targetCoordinate, mask) & (TileBody::OUTER_NORTH_SIDE << direction)) != TileBody::EMPTY)
	{
		if (frontObject == nullptr || !frontObject->isOpenableBy(this, direction))
		{
			return false;
		}
	}
	else
	{
		if (frontObject != nullptr && !frontObject->isEnterableBy(this, direction))
		{
			return false;
		}
	}
    
	auto targetItem = _level->getItemAt(targetCoordinate);
    if (targetItem != nullptr && !targetItem->isPickableBy(this))
    {
        return false;
    }
    
	auto frontCreature = _level->getCreatureAt(targetCoordinate);
    if (frontCreature != nullptr)
    {
		if (_type == CreatureType::CHIP)
		{
			return (frontCreature->_type != CreatureType::BLOCK) || frontCreature->canMove(direction);
		}
		else if (_type == CreatureType::BLOCK)
		{
			return (frontCreature->_type != CreatureType::BLOCK);
		}
		else if (_type == CreatureType::TEETH)
		{
			return (frontCreature->_type != CreatureType::BLOCK && frontCreature->_type != CreatureType::TEETH);
		}
		else
		{
			return (frontCreature->_type == CreatureType::CHIP);
		}
    }
    
    return true;
}

void Creature::updateAnimation()
{
    Direction wasAnimatedDirection = _animatedDirection;
    CreatureState wasAnimatedState = _animatedState;
    bool wasAnimatedMoving = _animatedMoving;
    
    _applyAnimationParams();
    if (_animatedDirection != wasAnimatedDirection || _animatedState != wasAnimatedState || _animatedMoving != wasAnimatedMoving)
    {
        _updateAnimation();
    }
    
    _updateFlip();
}

void Creature::_tryMoveNext()
{
    if (_queuedMove)
    {
        if (canMove(_queuedDirection))
        {
			_move(_queuedDirection);
			return;
        }
    }
    
    if (_type == CreatureType::BLOCK || _type == CreatureType::CHIP)
    { }
    else if (_type == CreatureType::TEETH)
    {
		auto playerCreature = _level->getPlayerCreature();
		if (playerCreature != nullptr)
		{
			auto targetCoordinate = playerCreature->getCoordinate();
			Direction dirs[2];
			size_t dirCount = 0;
			if (_coordinate.y != targetCoordinate.y)
			{
				dirs[dirCount++] = (_coordinate.y > targetCoordinate.y) ? Direction::NORTH : Direction::SOUTH;
			}
			if (_coordinate.x != targetCoordinate.x)
			{
				dirs[dirCount++] = (_coordinate.x > targetCoordinate.x) ? Direction::WEST : Direction::EAST;
			}
        
			if (dirCount == 2 && fabsf(_coordinate.x - targetCoordinate.x) > fabsf(_coordinate.y - targetCoordinate.y))
			{
				std::swap(dirs[0], dirs[1]);
			}
        
			for (size_t i = 0; i < dirCount; i++)
			{
				if (canMove(dirs[i]))
				{
					_move(dirs[i]);
					break;
				}
			}
        }
    }
    else if (_type == CreatureType::WALKER || _type == CreatureType::BLOB)
    {
        if (_type == CreatureType::WALKER && canMove(_direction))
        {
            _move(_direction);
        }
        else
        {
            Direction dirs[4];
            size_t dirCount = 0;
            
            for (int dirIndex = 0; dirIndex < 4; dirIndex++)
            {
                Direction direction = static_cast<Direction>(dirIndex);
                if (canMove(direction))
                {
                    dirs[dirCount++] = _direction;
                }
            }
            
            if (dirCount > 0)
            {
                _move(dirs[std::rand() % dirCount]);
            }
        }
    }
    else
    {
        Direction direction = _direction;
        if (_type == CreatureType::BUG)
        {
            direction = turnLeft(direction);
        }
        else if (_type == CreatureType::PARAMECIUM)
        {
            direction = turnRight(direction);
        }
        
		if (canMove(direction))
		{
			_move(direction);
		}
		else
		{
            auto initialDirection = direction;
            for (;;)
            {
                if (_type == CreatureType::FIREBALL || _type == CreatureType::BUG)
                {
                    direction = turnRight(direction);
                }
                else if (_type == CreatureType::GLIDER || _type == CreatureType::PARAMECIUM)
                {
                    direction = turnLeft(direction);
                }
                else if (_type == CreatureType::BALL)
                {
                    direction = inverse(direction);
                }
                else if (_type == CreatureType::TANK)
                { }
                
                if (direction == initialDirection)
                {
                    break;
                }
                
                if (canMove(direction))
                {
                    _move(direction);
                    break;
                }
            }
        }
    }
}

void Creature::_move(Direction direction)
{
	_queuedMove = false;

    _turnsToNextMove = getTurnsPerMove();
    _direction = direction;
    _coordinate += toVec2(direction);
    
    float duration = _turnsToNextMove * _level->getTurnDuration();
    
    if (_direction == Direction::SOUTH || _direction == Direction::EAST)
    {
        auto zOrder = _level->getProjector()->coordinateToZOrder(_coordinate) + getZOrderDelta();
        _sprite->setLocalZOrder(zOrder);
    }
    
    auto moveAction = cocos2d::MoveTo::create(duration, _level->getProjector()->coordinateToPoint(_coordinate));
    moveAction->setTag(MOVE_ACTION_TAG);
    _sprite->runAction(moveAction);
    
	auto object = _level->getObjectAt(_coordinate - toVec2(direction));
	if (object != nullptr)
	{
		object->beforeEscape(this);
	}

	auto frontObject = _level->getObjectAt(_coordinate);
	if (frontObject != nullptr)
	{
		frontObject->beforeEnter(this);
	}

	if (_type == CreatureType::CHIP)
	{
		for (auto creature : _level->getCreatures())
		{
			if (creature->_type == CreatureType::BLOCK && creature->_coordinate == _coordinate)
			{
				creature->queueMove(_direction);
			}
		}
	}
}

void Creature::_updatePosition()
{
    _sprite->setPosition(_level->getProjector()->coordinateToPoint(_coordinate));
    
    auto zOrder = _level->getProjector()->coordinateToZOrder(_coordinate) + getZOrderDelta();
    _sprite->setLocalZOrder(zOrder);
    
    if (_soundEmitter != nullptr)
    {
        _soundEmitter->setCoordinate(_coordinate);
    }
}

void Creature::_applyAnimationParams()
{
    if (_type == CreatureType::BLOCK || _type == CreatureType::BLOB)
    {
        // This creature is not rotatable.
        _animatedDirection = Direction::EAST;
        _animatedFlipped = false;
    }
    else
    {
        // There are animations only for east/west directions and north/south are mirrored.
        if (_direction == Direction::SOUTH || _direction == Direction::NORTH)
        {
            _animatedDirection = (_direction == Direction::SOUTH) ? Direction::EAST : Direction::WEST;
            _animatedFlipped = true;
        }
        else
        {
            _animatedDirection = _direction;
            _animatedFlipped = false;
        }
        
        // Paramecium has same animation for move front or back.
        if (_type == CreatureType::PARAMECIUM)
        {
            _animatedDirection = Direction::EAST;
        }
    }
    
    if (_type == CreatureType::BLOCK || _type == CreatureType::BLOB || _type == CreatureType::GLIDER || _type == CreatureType::FIREBALL)
    {
        // This creature has the same animation for standing and moving.
        _animatedMoving = false;
    }
    else
    {
        _animatedMoving = (_turnsToNextMove > 0);
    }
    
    // Balls and walkers
    if (!_animatedMoving)
    {
        if (_type == CreatureType::BALL || _type == CreatureType::WALKER)
        {
            _animatedDirection = Direction::EAST;
        }
    }
    
    if (_state == CreatureState::SLIDING)
    {
        // Sliding creature doesn't move the legs.
        _animatedMoving = false;
    }
    else if (_state == CreatureState::SWIMMING)
    {
        if (_type == CreatureType::CHIP)
        {
            // Swimming chip has the same animation for standing and moving.
            _animatedMoving = false;
        }
        else
        {
            // Only chip has special swimming animation.
            _animatedState = CreatureState::NORMAL;
        }
    }
}

void Creature::_updateAnimation()
{
    if (_animatedMoving)
    {
        auto animationName = std::to_string(_type) + "-walk-" + std::to_string(_animatedDirection);
        auto animation = cocos2d::AnimationCache::getInstance()->getAnimation(animationName);
        auto action = cocos2d::RepeatForever::create(cocos2d::Animate::create(animation));
        action->setTag(ANIMATE_ACTION_TAG);
        _sprite->stopAllActionsByTag(ANIMATE_ACTION_TAG);
        _sprite->runAction(action);
    }
    else
    {
        bool hasStaticAnimation = (_type == CreatureType::BLOCK
           || _type == CreatureType::GLIDER
           || _type == CreatureType::BUG
           || _type == CreatureType::PARAMECIUM
           || _type == CreatureType::BALL
           || _type == CreatureType::WALKER
           || _type == CreatureType::TANK);
        
        if (hasStaticAnimation)
        {
            auto spriteFrameName = std::to_string(_type) + "-stay-" + std::to_string(_animatedDirection) + ".png";
            auto spriteFrame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);
            _sprite->setSpriteFrame(spriteFrame);
        }
        else
        {
            auto animationName = std::to_string(_type) + (_state == CreatureState::SWIMMING ? "-swim-" : "-stay-") + std::to_string(_animatedDirection);
            auto animation = cocos2d::AnimationCache::getInstance()->getAnimation(animationName);
            auto action = cocos2d::RepeatForever::create(cocos2d::Animate::create(animation));
            action->setTag(ANIMATE_ACTION_TAG);
            _sprite->stopAllActionsByTag(ANIMATE_ACTION_TAG);
            _sprite->runAction(action);
        }
    }
}

void Creature::_updateFlip()
{
    if (_animatedFlipped)
    {
        _sprite->setAnchorPoint(cocos2d::Vec2(1, 0));
        _sprite->setScaleX(-1);
    }
    else
    {
        _sprite->setAnchorPoint(cocos2d::Vec2(0, 0));
        _sprite->setScaleX(1);
    }
}
