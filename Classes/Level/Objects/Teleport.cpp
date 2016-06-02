#include "Teleport.h"
#include <LevelData/LevelData.h>
#include <Tiled/TiledProjector.h>
#include <Level/Level.h>
#include <Level/Creature.h>

const cocos2d::Vec2 Teleport::BOTTOM_POSITION = cocos2d::Vec2(0, -120);

Teleport* Teleport::create(const cocos2d::Vec2& coordinate)
{
    auto instance = new Teleport(coordinate);
    instance->autorelease();
    return instance;
}

Teleport::Teleport(const cocos2d::Vec2& coordinate) : LevelObject(coordinate)
{
	_backPart = cocos2d::Sprite::createWithSpriteFrameName("hole.png");
	_backPart->setAnchorPoint(cocos2d::Vec2::ZERO);
	_backPart->setLocalZOrder(Level::BACK_Z_ORDER);
	_backPart->setPosition(cocos2d::Vec2(0, -12));
	addNode(_backPart);

	auto abuseSprite = cocos2d::Sprite::create();
	abuseSprite->setAnchorPoint(cocos2d::Vec2::ZERO);
	abuseSprite->setLocalZOrder(-1);
	abuseSprite->runAction(cocos2d::RepeatForever::create(
		cocos2d::Animate::create(cocos2d::AnimationCache::getInstance()->getAnimation("teleport"))
	));
	_backPart->addChild(abuseSprite);

	_frontPart = nullptr;
}

void Teleport::beforeEnter(Creature* creature)
{
	auto duration = _level->getTurnDuration() * creature->getTurnsPerMove();
	creature->getSprite()->runAction(cocos2d::Sequence::create(
		cocos2d::DelayTime::create(0.75f * duration),
		cocos2d::CallFunc::create([this, creature]() {
			_setDrawAsTwoParts();
			auto zOrder = _level->getProjector()->coordinateToZOrder(_coordinate) * Level::Z_ORDER_PER_TILE;
			creature->getSprite()->setLocalZOrder(zOrder + Level::CREATURE_SMALL_Z_ORDER);
		}),
		cocos2d::Spawn::create(
			cocos2d::FadeTo::create(0.25f * duration, 0),
			cocos2d::MoveBy::create(0.25f * duration, BOTTOM_POSITION),
			nullptr
		),
		nullptr)
	);
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
        
        if (dynamic_cast<Teleport*>(_level->getObjectAt(cocos2d::Vec2(x, y))) != nullptr)
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
	creature->getSprite()->setLocalZOrder(_level->getProjector()->coordinateToZOrder(cocos2d::Vec2(x, y)) * Level::Z_ORDER_PER_TILE + 1);

	if (x != _coordinate.x && y != _coordinate.y)
	{
		_setDrawAsSinglePart();
	}
}

void Teleport::beforeEscape(Creature* creature)
{
	_setDrawAsTwoParts();

	auto creatureZOrder = creature->getSprite()->getLocalZOrder();

	auto zOrder = _level->getProjector()->coordinateToZOrder(_coordinate) * Level::Z_ORDER_PER_TILE;
	creature->getSprite()->setLocalZOrder(zOrder + Level::CREATURE_SMALL_Z_ORDER);

    auto duration = _level->getTurnDuration() * creature->getTurnsPerMove();
	creature->getSprite()->runAction(cocos2d::Sequence::create(
		cocos2d::Spawn::create(
			cocos2d::FadeTo::create(0.25f * duration, 255),
			cocos2d::MoveBy::create(0.25f * duration, -BOTTOM_POSITION),
			nullptr
		),
		cocos2d::CallFunc::create([this, creature, creatureZOrder]() {
			creature->getSprite()->setLocalZOrder(creatureZOrder);
			_setDrawAsSinglePart();
		}),
		nullptr
	));
}

void Teleport::_setDrawAsTwoParts()
{
	if (_frontPart == nullptr)
	{
		_backPart->setSpriteFrame("hole-back.png");
		_frontPart = cocos2d::Sprite::createWithSpriteFrameName("hole-front.png");
		_frontPart->setAnchorPoint(cocos2d::Vec2::ZERO);
		_frontPart->setLocalZOrder(Level::WALL_Z_ORDER);
		_frontPart->setPosition(cocos2d::Vec2(0, -12));
		addNode(_frontPart);
	}
}

void Teleport::_setDrawAsSinglePart()
{
	if (_frontPart != nullptr)
	{
		_backPart->setSpriteFrame("hole.png");
		removeNode(_frontPart);
		_frontPart = nullptr;
	}
}
