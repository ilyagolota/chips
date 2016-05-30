#include "Ice.h"
#include "Level/Level.h"
#include "Level/Creature.h"
#include "Level/Inventory.h"

Ice* Ice::create(const cocos2d::Vec2& coordinate, TileType type)
{
    auto instance = new Ice(coordinate, type);
    instance->autorelease();
    return instance;
}

Ice::Ice(const cocos2d::Vec2& coordinate, TileType type) : LevelObject(coordinate)
{
    _type = type;
    
    auto sprite = cocos2d::Sprite::createWithSpriteFrameName("ice.png");
    sprite->setAnchorPoint(cocos2d::Vec2::ZERO);
    sprite->setPosition(cocos2d::Vec2(0, -12));
    sprite->setLocalZOrder(0);
    addNode(sprite);
    
    if (_type == TileType::ICE_WALL_NORTH_EAST)
    {
        auto northWall = cocos2d::Sprite::createWithSpriteFrameName("ice-wall-north.png");
        northWall->setAnchorPoint(cocos2d::Vec2::ZERO);
        northWall->setPosition(cocos2d::Vec2::ZERO);
        northWall->setLocalZOrder(Level::BACK_Z_ORDER);
        addNode(northWall);
        
        auto eastWall = cocos2d::Sprite::createWithSpriteFrameName("ice-wall-east.png");
        eastWall->setAnchorPoint(cocos2d::Vec2::ZERO);
        eastWall->setPosition(cocos2d::Vec2::ZERO);
        eastWall->setLocalZOrder(Level::FRONT_Z_ORDER);
        addNode(eastWall);
    }
    else if (_type == TileType::ICE_WALL_SOUTH_EAST)
    {
        auto wall = cocos2d::Sprite::createWithSpriteFrameName("ice-wall-south-east.png");
        wall->setAnchorPoint(cocos2d::Vec2::ZERO);
        wall->setPosition(cocos2d::Vec2::ZERO);
        wall->setLocalZOrder(Level::WALL_Z_ORDER);
        addNode(wall);
    }
    else if (_type == TileType::ICE_WALL_NORTH_WEST)
    {
        auto wall = cocos2d::Sprite::createWithSpriteFrameName("ice-wall-north-west.png");
        wall->setAnchorPoint(cocos2d::Vec2::ZERO);
        wall->setPosition(cocos2d::Vec2::ZERO);
        wall->setLocalZOrder(Level::BACK_Z_ORDER);
        addNode(wall);
    }
    else if (_type == TileType::ICE_WALL_SOUTH_WEST)
    {
        auto westWall = cocos2d::Sprite::createWithSpriteFrameName("ice-wall-west.png");
        westWall->setAnchorPoint(cocos2d::Vec2::ZERO);
        westWall->setPosition(cocos2d::Vec2::ZERO);
        westWall->setLocalZOrder(Level::BACK_Z_ORDER);
        addNode(westWall);
        
        auto southWall = cocos2d::Sprite::createWithSpriteFrameName("ice-wall-south.png");
        southWall->setAnchorPoint(cocos2d::Vec2::ZERO);
        southWall->setPosition(cocos2d::Vec2::ZERO);
        southWall->setLocalZOrder(Level::FRONT_Z_ORDER);
        addNode(southWall);
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
