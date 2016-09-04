#include "Slide.h"
#include <Tiled/TiledProjector.h>
#include <Level/Level.h>
#include <Level/Creature.h>

Slide* Slide::create(Level* level, const cocos2d::Vec2& coordinate, TileType tileType)
{
    auto instance = new Slide(level, coordinate, tileType);
    instance->autorelease();
    return instance;
}

Slide::Slide(Level* level, const cocos2d::Vec2& coordinate, TileType tileType) : LevelObject(level, coordinate)
{
    _type = tileType;

	_floor = cocos2d::Sprite::create();
	_floor->setAnchorPoint(cocos2d::Vec2::ZERO);
	_floor->setPosition(_level->getProjector()->coordinateToPoint(_coordinate) + cocos2d::Vec2(0, -12));
	_floor->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::BACK_Z_ORDER);
	_level->getStage()->addChild(_floor);

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

	_floor->runAction(cocos2d::RepeatForever::create(cocos2d::Animate::create(animation)));
}

void Slide::afterEnter(Creature* creature)
{
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
		creature->move(direction);
	}
}
