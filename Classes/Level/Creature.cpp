#include "Creature.h"

#include <cocos2d.h>

#include "LevelData/TileType.h"
#include "LevelData/LevelData.h"

#include "Tiled/Direction.h"
#include "Tiled/TiledPhysicsWorld.h"
#include "Tiled/TiledProjector.h"
#include "Tiled/TiledSoundEnvironment.h"

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

static Direction reflectForAnimation(Direction direction)
{
	if (direction == Direction::WEST)
	{
		return Direction::NORTH;
	}
	else if (direction == Direction::SOUTH)
	{
		return Direction::EAST;
	}
	else
	{
		return direction;
	}
}

Creature* Creature::create(CreatureType type)
{
    auto instance = new Creature(type);
    instance->autorelease();
    return instance;
}

Creature::Creature(CreatureType type)
{
    _level = nullptr;
    _type = type;
    _direction = Direction::NORTH;
    _coordinate = cocos2d::Vec2::ZERO;
    
    _sprite = cocos2d::Sprite::create();
    _sprite->setAnchorPoint(cocos2d::Point::ZERO);
    _sprite->retain();
}

Creature::~Creature()
{
    _sprite->release();
}

void Creature::move()
{
    if (canMove())
    {
        _move();
    }
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
    
    _sprite->setPosition(_level->getProjector()->coordinateToPoint(coordinate));
	_sprite->setLocalZOrder(_level->getProjector()->coordinateToZOrder(coordinate));
}

Direction Creature::getDirection() const
{
    return _direction;
}

void Creature::setDirection(Direction value)
{
    if (_direction != value)
    {
		auto wasDirection = _direction;
        _direction = value;
		_updateAnimation(_turnsToNextMove != 0, wasDirection);
    }
}

Level* Creature::getLevel() const
{
    return _level;
}

void Creature::setLevel(Level* level)
{
    if (_level != level)
    {
        if (_level != nullptr)
        {
            _level->getStage()->removeChild(_sprite);
            
            if (_soundEmitter != nullptr)
            {
                _level->getSoundEnvironment()->removeEmitter(_soundEmitter);
            }
        }
        _level = level;
        if (_level != nullptr)
        {
            _sprite->setPosition(_level->getProjector()->coordinateToPoint(_coordinate));
            _sprite->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate));
            _level->getStage()->addChild(_sprite);
            
            if (_soundEmitter != nullptr)
            {
                _level->getSoundEnvironment()->addEmitter(_soundEmitter);
            }
            
            auto object = _level->getObjectAt(_coordinate);
            if (object != nullptr)
            {
                object->afterEnter(this);
            }
            
            auto item = _level->getItemAt(_coordinate);
            if (item != nullptr)
            {
                // if (item->can
                //item->afterEnter(creature);
            }
        }
    }
}

void Creature::onTurn(float dt)
{
    auto wasMoving = (_turnsToNextMove > 0);
    auto wasDirection = _direction;
    
    if (_turnsToNextMove > 0)
    {
        _turnsToNextMove -= 1;
        if (_turnsToNextMove == 0)
        {
            _sprite->stopAllActionsByTag(MOVE_ACTION_TAG);
            _sprite->setPosition(_level->getProjector()->coordinateToPoint(_coordinate));
            _sprite->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate));
            
            auto object = _level->getObjectAt(_coordinate);
            if (object != nullptr)
            {
                object->afterEnter(this);
            }
        }
    }
    
    if (_type == CreatureType::CHIP)
    {
        auto metCreature = _level->getCreatureAt(_coordinate);
        if (metCreature != nullptr && metCreature != this)
        {
            if (metCreature->_type != CreatureType::BLOCK)
            {
                //_level->fail("Ooops! Look out for creatures!");
            }
            else
            {
                //_level->fail("Ooops! Watch out for moving blocks!");
            }
            //_game->playSound("bummer");
        }
    }
    
    if (_turnsToNextMove == 0)
    {
        _tryMoveNext();
    }
    
    _updateAnimation(wasMoving, wasDirection);
}

bool Creature::canMove() const
{
    Direction direction = _direction;
    
    int physicsLayerMask = (_type == CreatureType::CHIP) ? 0x01 : ((_type == CreatureType::BLOCK) ? 0x02 : 0x04);
    if (_level->getPhysicsWorld()->rayCast(_coordinate, _direction, physicsLayerMask))
    {
        return false;
    }
    
    auto currentObject = _level->getObjectAt(_coordinate);
    if (currentObject != nullptr && !currentObject->isEscapableBy(this, direction))
    {
        return false;
    }
    
    auto nextCoordinate = _coordinate + toVec2(_direction);
    
    auto frontObject = _level->getObjectAt(nextCoordinate);
    if (frontObject != nullptr && !frontObject->isEnterableBy(this, direction))
    {
        return false;
    }
    
    /*auto targetItem = _level->getItemAt(nextCoordinate);
    if (targetItem != nullptr && !targetItem->isPickableBy(this))
    {
        return false;
    }*/
    
    auto frontCreature = _level->getCreatureAt(nextCoordinate);
    if (frontCreature != nullptr)
    {
        if (frontCreature->_type == CreatureType::BLOCK)
        {
            if (_type == CreatureType::CHIP)
            {
                frontCreature->setDirection(_direction);
                return frontCreature->canMove();
            }
        }
        else if (frontCreature->_type == CreatureType::CHIP)
        {
            if (_type != CreatureType::CHIP)
            {
                return true;
            }
        }
        else if (frontCreature->_type == CreatureType::TEETH)
        {
            return false;
        }
        else
        {
            return _type == CreatureType::TEETH;
        }
    }
    
    return true;
}

void Creature::_tryMoveNext()
{
    if (_type == CreatureType::CHIP)
    {
        /*auto controlDirection = _level->getController()->getSelectedDirection();
        if (controlDirection != Direction::NONE)
        {
            _direction = controlDirection;
            if (canMove())
            {
                _move();
            }
        }*/
    }
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
				_direction = dirs[i];
				if (canMove())
				{
					_move();
					break;
				}
			}
        }
    }
    else if (_type == CreatureType::WALKER || _type == CreatureType::BLOB)
    {
        if (_type == CreatureType::WALKER && canMove())
        {
            _move();
        }
        else
        {
            Direction dirs[4];
            size_t dirCount = 0;
            
            for (int direction = 0; direction < 4; direction++)
            {
                _direction = static_cast<Direction>(direction);
                if (canMove())
                {
                    dirs[dirCount++] = _direction;
                }
            }
            
            if (dirCount > 0)
            {
                _direction = dirs[std::rand() % dirCount];
                _move();
            }
        }
    }
    else
    {
        if (_type == CreatureType::BUG)
        {
            _direction = turnLeft(_direction);
        }
        else if (_type == CreatureType::PARAMECIUM)
        {
            _direction = turnRight(_direction);
        }
        
        if (!canMove())
        {
            auto initialDirection = _direction;
            for (;;)
            {
                if (_type == CreatureType::FIREBALL || _type == CreatureType::BUG)
                {
                    _direction = turnRight(_direction);
                }
                else if (_type == CreatureType::GLIDER || _type == CreatureType::PARAMECIUM)
                {
                    _direction = turnLeft(_direction);
                }
                else if (_type == CreatureType::BALL)
                {
                    _direction = inverse(_direction);
                }
                else if (_type == CreatureType::TANK)
                { }
                
                if (_direction == initialDirection)
                {
                    break;
                }
                
                if (canMove())
                {
                    _move();
                    break;
                }
            }
        }
    }
}

void Creature::_move()
{
    /*if (direction != Direction::NONE)
    {
        _leftTileAt(_coordinate);
        _coordinate += directionToVec2(direction);
        _turnsToNextMove = getTurnsPerMove();
        
        auto wasDirection = _direction;
        _direction = direction;
        _movedToTileAt(_coordinate);
        
        _setAnimationState(WALK, wasDirection != _direction);
        
        float duration = _turnsToNextMove * _level->getTurnDuration();
        auto targetPosition = _level->getScene()->getTiledMap()->getLayer(1)->coordinateToPosition(_coordinate);
        auto moveAction = cocos2d::MoveTo::create(duration, targetPosition);
        moveAction->setTag(MOVE_ACTION_TAG);
        _sprite->runAction(moveAction);
    }*/
}

void Creature::_updateAnimation(bool wasMoving, Direction wasDirection)
{
    bool force = (_sprite->getTexture() == nullptr);
    switch (_type)
    {
        case CreatureType::BLOCK:
            if (force)
            {
				std::string spriteFrameName = "block.png";
				auto spriteFrame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);
                _sprite->setSpriteFrame(spriteFrame);
            }
            break;
            
        case CreatureType::BLOB:
            if (force)
            {
				std::string animationName = std::to_string(_type);
                auto animation = cocos2d::AnimationCache::getInstance()->getAnimation(animationName);
                auto action = cocos2d::RepeatForever::create(cocos2d::Animate::create(animation));
                action->setTag(ANIMATE_ACTION_TAG);
                _sprite->runAction(action);
            }
            break;
            
        case CreatureType::GLIDER:
			if (force || reflectForAnimation(_direction) != reflectForAnimation(wasDirection))
            {
				std::string spriteFrameName = std::to_string(_type) + "-" + std::to_string(reflectForAnimation(_direction)) + ".png";
				auto spriteFrame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);
                _sprite->setSpriteFrame(spriteFrame);
            }
            break;

        case CreatureType::BUG:
        case CreatureType::TANK:
        case CreatureType::BALL:
        case CreatureType::WALKER:
			if (force || wasMoving != isMoving() || reflectForAnimation(_direction) != reflectForAnimation(wasDirection))
            {
                _sprite->stopAllActionsByTag(ANIMATE_ACTION_TAG);
                if (isMoving())
                {
					auto animationName = std::to_string(_type) + "-walk-" + std::to_string(reflectForAnimation(_direction));
                    auto animation = cocos2d::AnimationCache::getInstance()->getAnimation(animationName);
                    auto action = cocos2d::RepeatForever::create(cocos2d::Animate::create(animation));
                    action->setTag(ANIMATE_ACTION_TAG);
                    _sprite->runAction(action);
                }
                else
                {
					auto spriteFrameName = std::to_string(_type) + "-stay-" + std::to_string(reflectForAnimation(_direction)) + ".png";
                    auto spriteFrame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);
                    _sprite->setSpriteFrame(spriteFrame);
                }
            }
            break;
            
        case CreatureType::FIREBALL:
        case CreatureType::TEETH:
        case CreatureType::PARAMECIUM:
            if (force)
            {
				auto animationName = std::to_string(_type) + "-walk-" + std::to_string(reflectForAnimation(_direction));
                auto animation = cocos2d::AnimationCache::getInstance()->getAnimation(animationName);
                auto action = cocos2d::RepeatForever::create(cocos2d::Animate::create(animation));
                action->setTag(ANIMATE_ACTION_TAG);
                _sprite->runAction(action);
            }
            break;
            
        case CreatureType::CHIP:
            if (force || wasMoving != isMoving() || (_direction != wasDirection && _direction != inverse(wasDirection)))
            {
                _sprite->stopAllActionsByTag(ANIMATE_ACTION_TAG);
                if (isMoving())
                {
					auto animationName = std::to_string(_type) + "-walk-" + std::to_string(reflectForAnimation(_direction));
                    auto animation = cocos2d::AnimationCache::getInstance()->getAnimation(animationName);
                    auto action = cocos2d::RepeatForever::create(cocos2d::Animate::create(animation));
                    action->setTag(ANIMATE_ACTION_TAG);
                    _sprite->runAction(action);
                }
                else
                {
					auto animationName = std::to_string(_type) + "-stay-" + std::to_string(reflectForAnimation(_direction));
                    auto animation = cocos2d::AnimationCache::getInstance()->getAnimation(animationName);
                    auto action = cocos2d::RepeatForever::create(cocos2d::Animate::create(animation));
                    action->setTag(ANIMATE_ACTION_TAG);
                    _sprite->runAction(action);
                }
            }
            break;
    }
    
    if (_direction != wasDirection)
    {
        if (_type != CreatureType::BLOCK && _type != CreatureType::BLOB)
        {
            if (_direction == Direction::NORTH || _direction == Direction::EAST)
            {
                _sprite->setAnchorPoint(cocos2d::Vec2(0, 0));
                _sprite->setScaleX(1);
            }
            else
            {
                _sprite->setAnchorPoint(cocos2d::Vec2(0, 1));
                _sprite->setScaleX(-1);
            }
        }
    }
}
