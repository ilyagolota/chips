#include "Teleport.h"
#include <LevelData/LevelData.h>
#include <Level/Level.h>
#include <Level/Creature.h>

const cocos2d::Vec2 Teleport::BOTTOM_POSITION = cocos2d::Vec2(0, -80);

Teleport* Teleport::create(const cocos2d::Vec2& coordinate)
{
    auto instance = new Teleport(coordinate);
    instance->autorelease();
    return instance;
}

Teleport::Teleport(const cocos2d::Vec2& coordinate) : LevelObject(coordinate)
{
}

void Teleport::beforeEnter(Creature* creature)
{
    auto duration = _level->getTurnDuration() * creature->getTurnsPerMove();
    auto delayAction = cocos2d::DelayTime::create(0.6f * duration);
    auto moveAction = cocos2d::MoveBy::create(0.4f * duration, BOTTOM_POSITION);
    creature->getSprite()->runAction(cocos2d::Sequence::create(delayAction, moveAction, nullptr));
}

void Teleport::afterEnter(Creature* creature)
{
    long x = _coordinate.x;
    long y = _coordinate.y;
    do
    {
        x--;
        if (x < 0)
        {
            x = _level->getLevelData()->getWidth() - 1;
            y--;
        }
        if (y < 0)
        {
            y = _level->getLevelData()->getHeight() - 1;
        }
        
        bool teleportFound = false;
        size_t index = y * _level->getLevelData()->getWidth() + x;
        for (auto& layer : _level->getLevelData()->getLayers())
        {
            if (layer[index] == TileType::TELEPORT)
            {
                teleportFound = true;
                break;
            }
        }
        
        if (teleportFound)
        {
            creature->setCoordinate(cocos2d::Vec2(x, y));
            if (creature->canMove(creature->getDirection()))
            {
                creature->move(creature->getDirection());
                break;
            }
        }
    } while (x != _coordinate.x || y != _coordinate.y);

    creature->getSprite()->setPosition(creature->getSprite()->getPosition() + BOTTOM_POSITION);
}

void Teleport::beforeEscape(Creature* creature)
{
    auto duration = _level->getTurnDuration() * creature->getTurnsPerMove();
    auto moveAction = cocos2d::MoveBy::create(0.4f * duration, -BOTTOM_POSITION);
    creature->getSprite()->runAction(moveAction);
}
