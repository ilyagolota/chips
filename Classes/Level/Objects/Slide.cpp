#include "Slide.h"
#include <Tiled/TiledProjector.h>
#include <Level/Level.h>
#include <Level/Creature.h>
#include <Level/Inventory.h>

Slide* Slide::create(Level* level, const cocos2d::Vec2& coordinate, TileType tileType)
{
    auto instance = new Slide(level, coordinate, tileType);
    instance->autorelease();
    return instance;
}

Slide::Slide(Level* level, const cocos2d::Vec2& coordinate, TileType tileType) : LevelObject(level, coordinate)
{
    _type = tileType;
}

void Slide::onAdd()
{
    _node = cocos2d::Sprite::create();
    _node->setAnchorPoint(cocos2d::Vec2::ZERO);
    _node->setPosition(_level->getProjector()->coordinateToPoint(_coordinate) + cocos2d::Vec2(0, -12));
    _node->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::BACK_Z_ORDER);
    _level->getStage()->addChild(_node);

    cocos2d::Animation* animation;
    switch (_type)
    {
    case TileType::SLIDE_NORTH:
        animation = cocos2d::AnimationCache::getInstance()->getAnimation("slide-north");
        break;

    case TileType::SLIDE_SOUTH:
        animation = cocos2d::AnimationCache::getInstance()->getAnimation("slide-south");
        break;

    case TileType::SLIDE_EAST:
        animation = cocos2d::AnimationCache::getInstance()->getAnimation("slide-east");
        break;

    case TileType::SLIDE_WEST:
        animation = cocos2d::AnimationCache::getInstance()->getAnimation("slide-west");
        break;

    default:
        animation = nullptr;
        break;
    }

    _node->runAction(cocos2d::RepeatForever::create(cocos2d::Animate::create(animation)));
}

void Slide::beforeEnter(Creature *creature)
{
    if (creature->getType() != CreatureType::CHIP || _level->getInventory()->getItemCount(TileType::BOOTS_SLIDE) <= 0)
    {
        creature->getSprite()->stopAllActionsByTag(Creature::CHANGE_STATE_ACTION_TAG);
        
        if (creature->getState() != CreatureState::SLIDING)
        {
            auto duration = _level->getTurnDuration() * creature->getTurnsPerMove();
            auto action = cocos2d::Sequence::create(
                cocos2d::DelayTime::create(0.5f * duration),
                cocos2d::CallFunc::create([this, creature]() {
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

void Slide::afterEnter(Creature* creature)
{
    if (creature->getType() == CreatureType::CHIP && _level->getInventory()->getItemCount(TileType::BOOTS_SLIDE) > 0)
    {
        return;
    }

    if (creature->hasQueuedMove())
    {
        auto prevCoordinate = creature->getCoordinate() + toVec2(inverse(creature->getDirection()));
        if (dynamic_cast<Slide*>(_level->getObjectAt(prevCoordinate)) != nullptr)
        {
            return;
        }
    }

    Direction direction;
    switch (_type)
    {
    case TileType::SLIDE_NORTH:
        direction = Direction::NORTH;
        break;

    case TileType::SLIDE_SOUTH:
        direction = Direction::SOUTH;
        break;

    case TileType::SLIDE_EAST:
        direction = Direction::EAST;
        break;

    case TileType::SLIDE_WEST:
        direction = Direction::WEST;
        break;

    default:
        direction = static_cast<Direction>(rand() % 4);
    }

    if (creature->canMove(direction))
    {
        creature->queueMove(direction);
    }
}

void Slide::beforeEscape(Creature *creature)
{
    if (creature->getState() == CreatureState::SLIDING)
    {
        auto duration = _level->getTurnDuration() * creature->getTurnsPerMove();
        auto action = cocos2d::Sequence::create(
            cocos2d::DelayTime::create(0.5f * duration),
            cocos2d::CallFunc::create([this, creature]() {
                creature->setState(CreatureState::NORMAL);
                creature->updateAnimation();
            }),
            nullptr
        );
        action->setTag(Creature::CHANGE_STATE_ACTION_TAG);
        creature->getSprite()->runAction(action);
    }
}
