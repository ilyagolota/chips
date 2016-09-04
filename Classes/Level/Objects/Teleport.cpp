#include "Teleport.h"
#include <LevelData/LevelData.h>
#include <Tiled/TiledProjector.h>
#include <Level/Level.h>
#include <Level/Creature.h>

const cocos2d::Vec2 Teleport::BOTTOM_POSITION = cocos2d::Vec2(0, -120);

Teleport* Teleport::create(Level* level, const cocos2d::Vec2& coordinate)
{
    auto instance = new Teleport(level, coordinate);
    instance->autorelease();
    return instance;
}

Teleport::Teleport(Level* level, const cocos2d::Vec2& coordinate) : LevelObject(level, coordinate)
{
	_backPart = cocos2d::Sprite::createWithSpriteFrameName("hole.png");
	_backPart->setAnchorPoint(cocos2d::Vec2::ZERO);
	_backPart->setLocalZOrder(Level::BACK_Z_ORDER);
	_backPart->setPosition(cocos2d::Vec2(0, -12));

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
		cocos2d::DelayTime::create(0.51f * duration),
		cocos2d::CallFunc::create([this, creature, duration]() {
			auto creatureSprite = creature->getSprite();
        
			auto thisPosition = _level->getProjector()->coordinateToPoint(_coordinate);
            auto action = cocos2d::Sequence::create(

                // Move creature to the top of teleport
                cocos2d::MoveTo::create(0.14f * duration, thisPosition),
          
                // Split teleport sprite to two parts and change creature z order
                cocos2d::CallFunc::create([this, creatureSprite]() {
                    _setDrawAsTwoParts();
                    auto zOrder = _level->getProjector()->coordinateToZOrder(_coordinate);
                    creatureSprite->setLocalZOrder(zOrder + Level::CREATURE_SMALL_Z_ORDER);
                }),
      
                // Move creature down and fade it out
                cocos2d::Spawn::create(
                    cocos2d::FadeTo::create(0.35f * duration, 0),
                    cocos2d::MoveTo::create(0.35f * duration, thisPosition + BOTTOM_POSITION),
                    nullptr
                ),
                nullptr
            );
        
            // Override default moving trajectory
            creatureSprite->stopActionByTag(Creature::MOVE_ACTION_TAG);
            action->setTag(Creature::MOVE_ACTION_TAG);
            creatureSprite->runAction(action);
		}),
		nullptr)
	);
}

void Teleport::afterEnter(Creature* creature)
{
    auto targetTeleport = _findTargetTeleport(creature);
    if (targetTeleport != nullptr)
    {
        creature->setCoordinate(targetTeleport->getCoordinate());
        creature->move(creature->getDirection());
        
        if (this != targetTeleport)
        {
            _setDrawAsSinglePart();
            targetTeleport->_setDrawAsTwoParts();
        }
    }
    
    auto creatureSprite = creature->getSprite();
    creatureSprite->setPosition(_level->getProjector()->coordinateToPoint(targetTeleport->_coordinate) + BOTTOM_POSITION);
    auto zOrder = _level->getProjector()->coordinateToZOrder(targetTeleport->_coordinate);
    creatureSprite->setLocalZOrder(zOrder + Level::CREATURE_SMALL_Z_ORDER);
}

void Teleport::beforeEscape(Creature* creature)
{
	_setDrawAsTwoParts();
    auto duration = _level->getTurnDuration() * creature->getTurnsPerMove();
    
    auto creatureSprite = creature->getSprite();
    
    // Override back creature z order changed by creature movement.
    auto storedZOrder = creatureSprite->getLocalZOrder();
    auto zOrder = _level->getProjector()->coordinateToZOrder(_coordinate);
    creatureSprite->setLocalZOrder(zOrder + Level::CREATURE_SMALL_Z_ORDER);
    
    auto destination = _level->getProjector()->coordinateToPoint(_coordinate + toVec2(creature->getDirection()));
    
    auto action = cocos2d::Sequence::create(
        // Move creature to the top of teleport and fade it in
        cocos2d::Spawn::create(
             cocos2d::FadeTo::create(0.35f * duration, 255),
             cocos2d::MoveTo::create(0.35f * duration, _level->getProjector()->coordinateToPoint(_coordinate)),
             nullptr
        ),
         
        // Join teleport sprites and restore creature z order
        cocos2d::CallFunc::create([this, creature, storedZOrder]() {
            _setDrawAsSinglePart();
            creature->getSprite()->setLocalZOrder(storedZOrder);
        }),

        // Move creature to it's destination
        cocos2d::MoveTo::create(0.65f * duration, destination),

        nullptr
    );
	
    // Override default moving trajectory
    creatureSprite->stopActionByTag(Creature::MOVE_ACTION_TAG);
    action->setTag(Creature::MOVE_ACTION_TAG);
    creatureSprite->runAction(action);
}

void Teleport::reset()
{
    _setDrawAsSinglePart();
}

Teleport* Teleport::_findTargetTeleport(Creature* creature)
{
    Teleport* targetTeleport = nullptr;
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
        
        auto teleport = dynamic_cast<Teleport*>(_level->getObjectAt(cocos2d::Vec2(x, y)));
        if (teleport != nullptr)
        {
            creature->setCoordinate(teleport->getCoordinate());
            if (creature->canMove(creature->getDirection()))
            {
                targetTeleport = teleport;
                break;
            }
        }
    } while (x != _coordinate.x || y != _coordinate.y);
    
    return targetTeleport;
}

void Teleport::_setDrawAsTwoParts()
{
	//if (_frontPart == nullptr)
	//{
	//	_backPart->setSpriteFrame("hole-back.png");
	//	_frontPart = cocos2d::Sprite::createWithSpriteFrameName("hole-front.png");
	//	_frontPart->setAnchorPoint(cocos2d::Vec2::ZERO);
	//	_frontPart->setLocalZOrder(Level::WALL_Z_ORDER);
	//	_frontPart->setPosition(cocos2d::Vec2(0, -12));
	//	addNode(_frontPart);
	//}
}

void Teleport::_setDrawAsSinglePart()
{
	//if (_frontPart != nullptr)
	//{
	//	_backPart->setSpriteFrame("hole.png");
	//	removeNode(_frontPart);
	//	_frontPart = nullptr;
	//}
}
