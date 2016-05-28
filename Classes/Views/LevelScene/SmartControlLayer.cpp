#include "SmartControlLayer.h"
#include <cmath>
#include <Tiled/TiledProjector.h>
#include <LevelData/TileType.h>
#include <Level/Level.h>
#include <Level/Creature.h>
#include <Level/Inventory.h>
#include <Level/Objects/Fire.h>
#include <Level/Objects/Water.h>

SmartControlLayer* SmartControlLayer::create(Level* level)
{
    auto instance = new SmartControlLayer(level);
    instance->autorelease();
    return instance;
}

SmartControlLayer::SmartControlLayer(Level* level)
{
    Layer::init();
    
    _level = level;
    _hasTarget = false;
    
    auto touchListener = cocos2d::EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(SmartControlLayer::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(SmartControlLayer::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(SmartControlLayer::onTouchEnded, this);
    touchListener->onTouchCancelled = CC_CALLBACK_2(SmartControlLayer::onTouchCancelled, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

SmartControlLayer::~SmartControlLayer()
{
    _eventDispatcher->resumeEventListenersForTarget(this);
}

bool SmartControlLayer::isPressed()
{
    if (!_hasTarget)
    {
        return false;
    }

    auto playerCreature = _level->getPlayerCreature();
    if (playerCreature != nullptr)
    {
        if (_playerCoordinate != playerCreature->getCoordinate())
        {
            _update();
        }
        
        if (!_hasTarget)
        {
            return false;
        }
        
        auto nextCoordinate = _playerCoordinate + toVec2(_selectedDirection);
        for (auto creature : _level->getCreatures())
        {
            if (creature->isMoving() && creature->getCoordinate() == nextCoordinate)
            {
                return false;
            }
        }
    }
    
    return _hasTarget;
}

Direction SmartControlLayer::getSelectedDirection()
{
    auto playerCreature = _level->getPlayerCreature();
    if (playerCreature != nullptr)
    {
        if (_playerCoordinate != playerCreature->getCoordinate())
        {
            _update();
        }
    }
    return _selectedDirection;
}

bool SmartControlLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    return true;
}

void SmartControlLayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{ }

void SmartControlLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    auto point = touch->getLocation() - _level->getStage()->getPosition();
    _targetCoordinate = _level->getProjector()->pointToCoordinate(point);

    auto playerCreature = _level->getPlayerCreature();
    if (playerCreature != nullptr)
    {
        _startCoordinate = _playerCoordinate = playerCreature->getCoordinate();
        _hasTarget = true;
        _update();
    }
}

void SmartControlLayer::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event)
{ }

void SmartControlLayer::_update()
{
    auto playerCreature = _level->getPlayerCreature();
    if (playerCreature != nullptr)
    {
        _playerCoordinate = playerCreature->getCoordinate();
        
        if (_playerCoordinate == _targetCoordinate)
        {
            _hasTarget = false;
            return;
        }

		cocos2d::Vec2 northWest;
		northWest.x = std::min(_startCoordinate.x, _targetCoordinate.x);
		northWest.y = std::min(_startCoordinate.y, _targetCoordinate.y);
		cocos2d::Vec2 southEast;
		southEast.x = std::max(_startCoordinate.x, _targetCoordinate.x);
		southEast.y = std::max(_startCoordinate.y, _targetCoordinate.y);
		if (_playerCoordinate.x < northWest.x || _playerCoordinate.y < northWest.y || _playerCoordinate.x > southEast.x || _playerCoordinate.y > southEast.y)
		{
			_hasTarget = false;
			return;
		}
        
        Direction allowedDirections[2];
        size_t directionCount = 0;

        if (_playerCoordinate.y != _targetCoordinate.y)
        {
            auto dir = (_playerCoordinate.y > _targetCoordinate.y) ? Direction::NORTH : Direction::SOUTH;
            if (playerCreature->canMove(dir))
            {
                allowedDirections[directionCount++] = dir;
            }
        }
        if (_playerCoordinate.x != _targetCoordinate.x)
        {
            auto dir = (_playerCoordinate.x > _targetCoordinate.x) ? Direction::WEST : Direction::EAST;
            if (playerCreature->canMove(dir))
            {
                allowedDirections[directionCount++] = dir;
            }
        }
        
        if (directionCount == 0)
        {
            _hasTarget = false;
        }
        else if (directionCount == 1)
        {
            _selectedDirection = allowedDirections[0];
        }
        else
        {
            int priorities[2] = {0, 0};
            if (std::abs(_playerCoordinate.x - _targetCoordinate.x) <= std::abs(_playerCoordinate.y - _targetCoordinate.y))
            {
                priorities[0] += 1;
            }
            else
            {
                priorities[1] += 1;
            }
            
            for (size_t i = 0; i < 2; i++)
            {
                auto nextCoordinate = _playerCoordinate + toVec2(allowedDirections[i]);
                
                if (_level->getInventory()->getItemCount(TileType::BOOTS_WATER) == 0)
                {
                    auto water = dynamic_cast<Water*>(_level->getObjectAt(nextCoordinate));
                    if (water != nullptr && !water->hasDrawnBlock())
                    {
                        priorities[i] -= 4;
                    }
                }
                
                if (_level->getInventory()->getItemCount(TileType::BOOTS_FIRE) == 0)
                {
                    auto fire = dynamic_cast<Fire*>(_level->getObjectAt(nextCoordinate));
                    if (fire != nullptr)
                    {
                        priorities[i] -= 4;
                    }
                }
                
                for (auto creature : _level->getCreatures())
                {
                    if (creature->getType() == CreatureType::BLOCK && !creature->isMoving() && creature->getCoordinate() == nextCoordinate)
                    {
                        priorities[i] -= 2;
                        break;
                    }
                }
            }
            
            _selectedDirection = allowedDirections[(priorities[1] > priorities[0]) ? 1 : 0];
        }
    }
}
