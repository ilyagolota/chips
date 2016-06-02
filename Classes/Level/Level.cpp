#include "Level.h"

#include <LevelData/LevelData.h>
#include <Tiled/TiledPhysicsWorld.h>
#include <Tiled/TiledSoundEnvironment.h>
#include <Tiled/TiledProjector.h>
#include "IPlayerControl.h"
#include "Inventory.h"
#include "Creature.h"
#include "Objects/LevelObject.h"
#include "Objects/Beartrap.h"
#include "Objects/BlueWall.h"
#include "Objects/Bomb.h"
#include "Objects/Burglar.h"
#include "Objects/Button.h"
#include "Objects/CloneMachine.h"
#include "Objects/Dirt.h"
#include "Objects/Door.h"
#include "Objects/Exit.h"
#include "Objects/Fire.h"
#include "Objects/HiddenWall.h"
#include "Objects/HintButton.h"
#include "Objects/Ice.h"
#include "Objects/PopupWall.h"
#include "Objects/Slide.h"
#include "Objects/Socket.h"
#include "Objects/SwitchWall.h"
#include "Objects/Teleport.h"
#include "Objects/Water.h"
#include "Item.h"

Level* Level::create(cocos2d::Node* stage)
{
    auto instance = new Level(stage);
    instance->autorelease();
    return instance;
}

Level::Level(cocos2d::Node* stage)
{
    _stage = stage;
    _levelData = nullptr;
    _timeMultiplier = 1.3f;
    
    _inventory = Inventory::create();
    _inventory->retain();
    
    auto levelSize = cocos2d::Size(32, 32);
    
    _physicsWorld = TiledPhysicsWorld::create(levelSize);
    _physicsWorld->retain();
    
    _soundEnvironment = TiledSoundEnvironment::create(_physicsWorld);
    _soundEnvironment->setPhysicsLayerMask(1);
    _soundEnvironment->retain();
    
    _projector = TiledProjector::create(cocos2d::Size(180, 128), levelSize, _physicsWorld);
    _projector->retain();

	_playerControl = nullptr;
    
    _objects.resize(levelSize.width * levelSize.height);
    _items.resize(levelSize.width * levelSize.height);
    _playerCreature = nullptr;
}

Level::~Level()
{
    _inventory->release();
    _physicsWorld->release();
    _soundEnvironment->release();
    _projector->release();
}

LevelData* Level::getLevelData() const
{
    return _levelData;
}

void Level::start(LevelData* levelData)
{
    if (_levelData != levelData)
    {
        if (_levelData != nullptr)
        {
            _levelData->release();
        }
        _levelData = levelData;
        _levelData->retain();
        _rebuild();
    }
    _reset();
}

void Level::restart()
{
    _reset();
}

void Level::makeTurn(float dt)
{
    if (_playerCreature != nullptr)
    {
        _playerCreature->onTurn(dt);
    }
    
    for (auto creature : _creatures)
    {
        if (creature != _playerCreature)
        {
            creature->onTurn(dt);
        }
    }
}

cocos2d::Node* Level::getStage() const
{
    return _stage;
}

TiledPhysicsWorld* Level::getPhysicsWorld() const
{
    return _physicsWorld;
}

TiledSoundEnvironment* Level::getSoundEnvironment() const
{
    return _soundEnvironment;
}

TiledProjector* Level::getProjector() const
{
    return _projector;
}

Inventory* Level::getInventory() const
{
    return _inventory;
}

float Level::getTurnDuration()
{
    return 0.2f * _timeMultiplier;
}

IPlayerControl* Level::getPlayerControl()
{
	return _playerControl;
}

void Level::setPlayerControl(IPlayerControl* playerControl)
{
	_playerControl = playerControl;
}

void Level::addObject(LevelObject* object)
{
    size_t index = _coordinateToIndex(object->getCoordinate());
    if (index < _items.size())
    {
        auto existingObject = _objects[index];
        if (existingObject != object)
        {
            if (existingObject != nullptr)
            {
                existingObject->setLevel(nullptr);
                existingObject->release();
            }
            _objects[index] = object;
            object->setLevel(this);
            object->retain();
        }
    }
}

LevelObject* Level::getObjectAt(const cocos2d::Vec2& coordinate) const
{
    size_t index = _coordinateToIndex(coordinate);
    if (index >= _objects.size())
    {
        return nullptr;
    }
    else
    {
        return _objects[index];
    }
}

void Level::addItem(Item* item)
{
    auto coordinate = item->getCoordinate();
    if (coordinate.x >= 0 && coordinate.y >= 0 && coordinate.x < _levelData->getWidth() && coordinate.y < _levelData->getHeight())
    {
        size_t index = _coordinateToIndex(coordinate);
        auto existingItem = _items[index];
        if (existingItem != nullptr)
        {
            existingItem->release();
        }
        
        _items[index] = item;
		item->setLevel(this);
        item->retain();
    }
}

Item* Level::getItemAt(const cocos2d::Vec2& coordinate) const
{
	if (coordinate.x >= 0 && coordinate.y >= 0 && coordinate.x < _levelData->getWidth() && coordinate.y < _levelData->getHeight())
	{
        size_t index = _coordinateToIndex(coordinate);
        return _items[index];
	}
    return nullptr;
}

void Level::addCreature(Creature* creature)
{
    _creatures.pushBack(creature);
    creature->setLevel(this);
    if (creature->getType() == CreatureType::CHIP)
    {
        _playerCreature = creature;
    }
}

void Level::removeCreature(Creature* creature)
{
    _creatures.eraseObject(creature);
    creature->setLevel(nullptr);
    if (creature->getType() == CreatureType::CHIP)
    {
        _playerCreature = nullptr;
    }
}


const cocos2d::Vector<Creature*>& Level::getCreatures() const
{
    return _creatures;
}

Creature* Level::getCreatureAt(const cocos2d::Vec2& coordinate) const
{
    for (auto creature : _creatures)
    {
        if (creature->getCoordinate() == coordinate)
        {
            return creature;
        }
    }
    return nullptr;
}

Creature* Level::getPlayerCreature() const
{
	return _playerCreature;
}

void Level::_rebuild()
{
    _stage->removeAllChildren();
    _physicsWorld->clear();
    _soundEnvironment->clearEmitters();
    _projector->clear();
    
    _creatures.clear();
    
    auto& layers = _levelData->getLayers();
    size_t width = _levelData->getWidth();
    size_t tileCount = _levelData->getWidth() * _levelData->getHeight();
    cocos2d::Vec2 coordinate;
    for (size_t i = 0; i < tileCount; i++)
    {
        if (_items.at(i) != nullptr)
        {
            _items[i]->release();
            _items[i] = nullptr;
        }
        
        if (_objects[i] != nullptr)
        {
            _objects[i]->release();
            _objects[i] = nullptr;
        }
        
        bool tileBuilt = false;
        for (auto& layer : layers)
        {
            if (_tryBuildTile(layer[i], coordinate))
            {
                tileBuilt = true;
            }
        }
        
        if (!tileBuilt)
        {
            _buildFloor(coordinate);
        }
        
        coordinate.x += 1;
        if (coordinate.x == width)
        {
            coordinate.x = 0;
            coordinate.y += 1;
        }
    }
}

void Level::_reset()
{
    auto& layers = _levelData->getLayers();
    size_t width = _levelData->getWidth();
    size_t tileCount = width * _levelData->getHeight();
    cocos2d::Vec2 coordinate;
    
    _creatures.clear();
    _playerCreature = nullptr;
    for (size_t i = 0; i < tileCount; i++)
    {
        for (auto& layer : layers)
        {
            _tryBuildCreature(layer[i], coordinate);
        }
        
        if (_items[i] != nullptr)
        {
            //_items[i]->reset();
        }
        
        if (_objects[i] != nullptr)
        {
            _objects[i]->reset();
        }
        
        coordinate.x += 1;
        if (coordinate.x == width)
        {
            coordinate.x = 0;
            coordinate.y += 1;
        }
    }
    
    _inventory->clear();
    if (_playerCreature != nullptr)
    {
        size_t i = _playerCreature->getCoordinate().x + _playerCreature->getCoordinate().y * width;
        for (auto& layer : layers)
        {
            if (layer[i] == TileType::WATER)
            {
                _inventory->addItem(TileType::BOOTS_WATER, 1);
                break;
            }
        }
    }
}

bool Level::_tryBuildTile(TileType tileType, const cocos2d::Vec2& coordinate)
{
    switch (tileType)
    {
        case TileType::EMPTY:
            return false;
            
        case TileType::WALL:
            _buildWall(coordinate);
            break;
            
        case TileType::IC_CHIP:
            addItem(Item::create(coordinate, tileType));
            return false;
            break;
            
        case TileType::WATER:
            addObject(Water::create(coordinate));
            break;
            
        case TileType::FIRE:
            addObject(Fire::create(coordinate));
            break;
            
        case TileType::HIDDEN_WALL_PERM:
            addObject(HiddenWall::create(coordinate, true));
            break;
            
        case TileType::WALL_NORTH:
        case TileType::WALL_WEST:
        case TileType::WALL_SOUTH:
        case TileType::WALL_EAST:
            _buildWall(coordinate);
            break;
            
        case TileType::DIRT:
            addObject(Dirt::create(coordinate));
            break;
            
        case TileType::ICE:
            addObject(Ice::create(coordinate, tileType));
            break;
            
        case TileType::SLIDE_SOUTH:
            addObject(Slide::create(coordinate, tileType));
            break;
            
        case TileType::SLIDE_NORTH:
        case TileType::SLIDE_EAST:
        case TileType::SLIDE_WEST:
            addObject(Slide::create(coordinate, tileType));
            break;
            
        case TileType::EXIT:
            addObject(Exit::create(coordinate));
            break;
            
        case TileType::DOOR_BLUE:
        case TileType::DOOR_RED:
        case TileType::DOOR_GREEN:
        case TileType::DOOR_YELLOW:
            addObject(Door::create(coordinate, tileType));
            break;
            
        case TileType::ICE_WALL_SOUTH_EAST:
        case TileType::ICE_WALL_SOUTH_WEST:
        case TileType::ICE_WALL_NORTH_WEST:
        case TileType::ICE_WALL_NORTH_EAST:
            addObject(Ice::create(coordinate, tileType));
            break;
            
        case TileType::BLUE_WALL_FAKE:
        case TileType::BLUE_WALL_REAL:
            addObject(BlueWall::create(coordinate, (tileType == TileType::BLUE_WALL_REAL)));
            break;
            
        case TileType::OVERLAY_BUFFER:
            return false;
            
        case TileType::BURGLAR:
            addObject(Burglar::create(coordinate));
            break;
            
        case TileType::SOCKET:
            addObject(Socket::create(coordinate));
            break;
            
        case TileType::BUTTON_GREEN:
        case TileType::BUTTON_RED:
            addObject(Button::create(coordinate, tileType));
            break;
            
        case TileType::SWITCH_WALL_CLOSED:
        case TileType::SWITCH_WALL_OPEN:
            addObject(SwitchWall::create(coordinate, (tileType == TileType::SWITCH_WALL_OPEN)));
            break;
            
        case TileType::BUTTON_BROWN:
        case TileType::BUTTON_BLUE:
            addObject(Button::create(coordinate, tileType));
            break;
            
        case TileType::TELEPORT:
            addObject(Teleport::create(coordinate));
            break;
            
        case TileType::BOMB:
            addObject(Bomb::create(coordinate));
            break;
            
        case TileType::BEARTRAP:
            addObject(Beartrap::create(coordinate));
            break;
            
        case TileType::HIDDEN_WALL_TEMP:
            addObject(HiddenWall::create(coordinate, false));
            break;
            
        case TileType::GRAVEL:
            _buildGravel(coordinate);
            break;
            
        case TileType::POPUP_WALL:
            addObject(PopupWall::create(coordinate));
            break;
            
        case TileType::HINT_BUTTON:
            addObject(HintButton::create(coordinate));
            break;
            
        case TileType::WALL_SOUTH_EAST:
            _buildWall(coordinate);
            break;
            
        case TileType::CLONE_MACHINE:
            addObject(CloneMachine::create(coordinate));
            break;
            
        case TileType::SLIDE_RANDOM:
            addObject(Slide::create(coordinate, tileType));
            break;
            
        case TileType::HIDDEN_WALL_PERM_1:
        case TileType::HIDDEN_WALL_PERM_2:
        case TileType::HIDDEN_WALL_PERM_3:
            addObject(HiddenWall::create(coordinate, true));
            break;
            
        case TileType::EXIT_EXTRA_1:
        case TileType::EXIT_EXTRA_2:
            addObject(Exit::create(coordinate));
            break;
            
        case TileType::KEY_BLUE:
        case TileType::KEY_RED:
        case TileType::KEY_GREEN:
        case TileType::KEY_YELLOW:
        case TileType::BOOTS_WATER:
        case TileType::BOOTS_FIRE:
        case TileType::BOOTS_ICE:
        case TileType::BOOTS_SLIDE:
            addItem(Item::create(coordinate, tileType));
            return false;
            
        default:
            return false;
    }
    return true;
}

bool Level::_tryBuildCreature(TileType tileType, const cocos2d::Vec2& coordinate)
{
    Creature* creature = nullptr;
    switch (tileType)
    {
        case TileType::EMPTY:
        case TileType::WALL:
            return false;
            
        case TileType::BLOCK:
            creature = Creature::create(CreatureType::BLOCK);
            break;
            
        case TileType::BLOCK_NORTH:
        case TileType::BLOCK_WEST:
        case TileType::BLOCK_SOUTH:
        case TileType::BLOCK_EAST:
            creature = Creature::create(CreatureType::BLOCK);
            creature->setDirection(static_cast<Direction>(static_cast<int>(tileType) - static_cast<int>(TileType::BLOCK_NORTH)));
            break;
                        
        case TileType::DROWNED_CHIP:
        case TileType::BURNED_CHIP:
        case TileType::BOMBED_CHIP:
            creature = Creature::create(CreatureType::CHIP);
            break;
        
        case TileType::EXITED_CHIP:
            creature = Creature::create(CreatureType::CHIP);
            break;
        
        case TileType::SWIMMING_CHIP_NORTH:
        case TileType::SWIMMING_CHIP_WEST:
        case TileType::SWIMMING_CHIP_SOUTH:
        case TileType::SWIMMING_CHIP_EAST:
            creature = Creature::create(CreatureType::CHIP);
            creature->setDirection(static_cast<Direction>(static_cast<int>(tileType) - static_cast<int>(TileType::SWIMMING_CHIP_NORTH)));
            break;
                    
        case TileType::BUG_NORTH:
        case TileType::BUG_WEST:
        case TileType::BUG_SOUTH:
        case TileType::BUG_EAST:
            creature = Creature::create(CreatureType::BUG);
            creature->setDirection(static_cast<Direction>(static_cast<int>(tileType) - static_cast<int>(TileType::BUG_NORTH)));
            break;
                                
        case TileType::FIREBALL_NORTH:
        case TileType::FIREBALL_WEST:
        case TileType::FIREBALL_SOUTH:
        case TileType::FIREBALL_EAST:
            creature = Creature::create(CreatureType::FIREBALL);
            creature->setDirection(static_cast<Direction>(static_cast<int>(tileType) - static_cast<int>(TileType::FIREBALL_NORTH)));
            break;

        case TileType::BALL_NORTH:
        case TileType::BALL_WEST:
        case TileType::BALL_SOUTH:
        case TileType::BALL_EAST:
            creature = Creature::create(CreatureType::BALL);
            creature->setDirection(static_cast<Direction>(static_cast<int>(tileType) - static_cast<int>(TileType::BALL_NORTH)));
            break;

        case TileType::TANK_NORTH:
        case TileType::TANK_WEST:
        case TileType::TANK_SOUTH:
        case TileType::TANK_EAST:
            creature = Creature::create(CreatureType::TANK);
            creature->setDirection(static_cast<Direction>(static_cast<int>(tileType) - static_cast<int>(TileType::TANK_NORTH)));
            break;

        case TileType::GLIDER_NORTH:
        case TileType::GLIDER_WEST:
        case TileType::GLIDER_SOUTH:
        case TileType::GLIDER_EAST:
            creature = Creature::create(CreatureType::GLIDER);
            creature->setDirection(static_cast<Direction>(static_cast<int>(tileType) - static_cast<int>(TileType::GLIDER_NORTH)));
            break;

        case TileType::TEETH_NORTH:
        case TileType::TEETH_WEST:
        case TileType::TEETH_SOUTH:
        case TileType::TEETH_EAST:
            creature = Creature::create(CreatureType::TEETH);
            creature->setDirection(static_cast<Direction>(static_cast<int>(tileType) - static_cast<int>(TileType::TEETH_NORTH)));
            break;

        case TileType::WALKER_NORTH:
        case TileType::WALKER_WEST:
        case TileType::WALKER_SOUTH:
        case TileType::WALKER_EAST:
            creature = Creature::create(CreatureType::WALKER);
            creature->setDirection(static_cast<Direction>(static_cast<int>(tileType) - static_cast<int>(TileType::WALKER_NORTH)));
            break;

        case TileType::BLOB_NORTH:
        case TileType::BLOB_WEST:
        case TileType::BLOB_SOUTH:
        case TileType::BLOB_EAST:
            creature = Creature::create(CreatureType::BLOB);
            creature->setDirection(static_cast<Direction>(static_cast<int>(tileType) - static_cast<int>(TileType::BLOB_NORTH)));
            break;

        case TileType::PARAMECIUM_NORTH:
        case TileType::PARAMECIUM_WEST:
        case TileType::PARAMECIUM_SOUTH:
        case TileType::PARAMECIUM_EAST:
            creature = Creature::create(CreatureType::PARAMECIUM);
            creature->setDirection(static_cast<Direction>(static_cast<int>(tileType) - static_cast<int>(TileType::PARAMECIUM_NORTH)));
            break;
            
        case TileType::CHIP_NORTH:
        case TileType::CHIP_WEST:
        case TileType::CHIP_SOUTH:
        case TileType::CHIP_EAST:
            creature = Creature::create(CreatureType::CHIP);
            creature->setDirection(static_cast<Direction>(static_cast<int>(tileType) - static_cast<int>(TileType::CHIP_NORTH)));
            break;
            
        default:
            return false;
    }
    
    creature->setCoordinate(coordinate);
    addCreature(creature);
    return true;
}

void Level::_buildFloor(const cocos2d::Vec2& coordinate)
{
    auto floorSprite = cocos2d::Sprite::createWithSpriteFrameName("floor.png");
    floorSprite->setAnchorPoint(cocos2d::Vec2::ZERO);
	floorSprite->setPosition(_projector->coordinateToPoint(coordinate) - cocos2d::Vec2(0, 12));
    floorSprite->setLocalZOrder(_projector->coordinateToZOrder(coordinate) * Z_ORDER_PER_TILE);
    _stage->addChild(floorSprite);
}

void Level::_buildWall(const cocos2d::Vec2& coordinate)
{
	_physicsWorld->setBody(coordinate, TileBody::OUTER_BOX, 7);

    int mask = 0;
    for (int dirIndex = 0; dirIndex < 4; dirIndex++)
    {
        auto neighborCoordinate = coordinate + toVec2(static_cast<Direction>(dirIndex));
        auto index = _coordinateToIndex(neighborCoordinate);
        
        bool hasNeighborWall = false;
        for (auto& layer : _levelData->getLayers())
        {
            switch (layer[index])
            {
                case TileType::WALL:
                case TileType::DOOR_BLUE:
                case TileType::DOOR_GREEN:
                case TileType::DOOR_YELLOW:
                case TileType::DOOR_RED:
                case TileType::SWITCH_WALL_CLOSED:
                case TileType::SWITCH_WALL_OPEN:
                case TileType::BLUE_WALL_REAL:
                case TileType::BLUE_WALL_FAKE:
                case TileType::HIDDEN_WALL_TEMP:
                case TileType::HIDDEN_WALL_PERM:
                case TileType::HIDDEN_WALL_PERM_1:
                case TileType::HIDDEN_WALL_PERM_2:
                case TileType::HIDDEN_WALL_PERM_3:
				case TileType::SOCKET:
                    hasNeighborWall = true;
                    break;
                    
                default:
                    break;
            }
            
            if (hasNeighborWall)
            {
                mask |= (1 << dirIndex);
                break;
            }
        }
    }
    
    char spriteFrameName[14];
    sprintf(spriteFrameName, "wall-%04d.png", mask + 1);
    
    auto wallSprite = cocos2d::Sprite::createWithSpriteFrameName(spriteFrameName);
    wallSprite->setAnchorPoint(cocos2d::Vec2::ZERO);
    wallSprite->setPosition(_projector->coordinateToPoint(coordinate) - cocos2d::Vec2(0, 12));
    wallSprite->setLocalZOrder(_projector->coordinateToZOrder(coordinate) * Z_ORDER_PER_TILE + WALL_Z_ORDER);
    _stage->addChild(wallSprite);
}

void Level::_buildGravel(const cocos2d::Vec2& coordinate)
{
	_physicsWorld->setBody(coordinate, TileBody::OUTER_BOX, 4);

	auto gravelSprite = cocos2d::Sprite::createWithSpriteFrameName("gravel.png");
	gravelSprite->setAnchorPoint(cocos2d::Vec2::ZERO);
	gravelSprite->setPosition(_projector->coordinateToPoint(coordinate) - cocos2d::Vec2(0, 12));
	gravelSprite->setLocalZOrder(_projector->coordinateToZOrder(coordinate) * Z_ORDER_PER_TILE);
	_stage->addChild(gravelSprite);
}

size_t Level::_coordinateToIndex(const cocos2d::Vec2& coordinate) const
{
    return coordinate.x + coordinate.y * _levelData->getWidth();
}
