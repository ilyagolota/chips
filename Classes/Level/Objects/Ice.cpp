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
