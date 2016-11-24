#include "BlueWall.h"
#include "../Level.h"

BlueWall* BlueWall::create(Level* level, const cocos2d::Vec2& coordinate, bool fake)
{
    auto instance = new BlueWall(level, coordinate, fake);
    instance->autorelease();
    return instance;
}

BlueWall::BlueWall(Level* level, const cocos2d::Vec2& coordinate, bool fake) : LevelObject(level, coordinate)
{
	_fake = fake;
    _known = false;
}

void BlueWall::onAdd()
{
	_known = false;

	int shape = _level->getWallShape(_coordinate);

	_node = cocos2d::Sprite::createWithSpriteFrameName(getFloorFrameName(shape));
	_node->setAnchorPoint(cocos2d::Vec2::ZERO);
	_node->setPosition(_level->getProjector()->coordinateToPoint(_coordinate) + cocos2d::Vec2(0, -12));
	_node->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::WALL_Z_ORDER);
	_level->getStage()->addChild(_node);

	_unknownWallNode = cocos2d::Node::create();
	_unknownWallNode->setAnchorPoint(cocos2d::Vec2::ZERO);
	_unknownWallNode->setPosition(cocos2d::Vec2(0, 12));
	_node->addChild(_unknownWallNode);

	auto wallPart1 = cocos2d::Sprite::createWithSpriteFrameName("blue-wall-0001.png");
	wallPart1->setAnchorPoint(cocos2d::Vec2::ZERO);
	wallPart1->setPosition(cocos2d::Vec2::ZERO);
	_unknownWallNode->addChild(wallPart1);

	auto wallPart2 = cocos2d::Sprite::createWithSpriteFrameName("blue-wall-0002.png");
	wallPart2->setAnchorPoint(cocos2d::Vec2::ZERO);
	wallPart2->setPosition(cocos2d::Vec2::ZERO);
	_unknownWallNode->addChild(wallPart2);
	
	auto duration = 1.5f;

	wallPart2->setOpacity(0);
	wallPart1->runAction(cocos2d::RepeatForever::create(
		cocos2d::Sequence::create(
			cocos2d::FadeTo::create(0.5f * duration, 0),
			cocos2d::FadeTo::create(0.5f * duration, 255),
			nullptr
		)
	));
	wallPart2->runAction(cocos2d::RepeatForever::create(
		cocos2d::Sequence::create(
			cocos2d::FadeTo::create(0.5f * duration, 255),
			cocos2d::FadeTo::create(0.5f * duration, 0),
			nullptr
		)
	));

	_level->getPhysicsWorld()->setBody(_coordinate, TileBody::OUTER_BOX, 7);
}

void BlueWall::reset()
{
	_known = false;
	_unknownWallNode->setVisible(true);
	_node->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::WALL_Z_ORDER);
	_level->getPhysicsWorld()->setBody(_coordinate, TileBody::OUTER_BOX, 7);
}

bool BlueWall::isOpenableBy(const Creature *creature, Direction direction) const
{
	return !_known;
}

void BlueWall::beforeEnter(Creature* creature)
{
	if (!_known)
	{
		_known = true;

		auto duration = _level->getTurnDuration() * creature->getTurnsPerMove();
		if (_fake)
		{
			_level->getPhysicsWorld()->setBody(_coordinate, TileBody::EMPTY, 7);

			_unknownWallNode->runAction(cocos2d::Sequence::create(
				cocos2d::DelayTime::create(0.51f * duration),
				cocos2d::CallFuncN::create([this](cocos2d::Node* unknownWallNode)
				{
					unknownWallNode->setVisible(false);
					_node->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::BACK_Z_ORDER);
				}),
				nullptr
			));
		}
		else
		{
			auto back = inverse(creature->getDirection());
			auto prevCoordinate = _coordinate + toVec2(back);
			auto prevObject = _level->getObjectAt(prevCoordinate);
			if (prevObject != nullptr)
			{
				auto cachedDirection = creature->getDirection();
				creature->setDirection(back);
				prevObject->beforeEnter(creature);
				creature->setDirection(cachedDirection);
			}

			creature->getSprite()->runAction(cocos2d::Sequence::create(
				cocos2d::DelayTime::create(0.51f * duration),
				cocos2d::CallFunc::create([this, creature, duration, prevCoordinate]()
				{
					auto targetPosition = _level->getProjector()->coordinateToPoint(prevCoordinate);
					auto action = cocos2d::MoveTo::create(0.49f * duration, targetPosition);

					auto creatureSprite = creature->getSprite();
					creatureSprite->stopActionByTag(Creature::MOVE_ACTION_TAG);
					action->setTag(Creature::MOVE_ACTION_TAG);
					creatureSprite->runAction(action);
				}),
				nullptr
			));
		}
	}
}

void BlueWall::afterEnter(Creature* creature)
{
	if (!_fake)
	{
		auto back = inverse(creature->getDirection());
		creature->setDirection(back);

		auto prevCoordinate = _coordinate + toVec2(back);
		creature->setCoordinate(prevCoordinate);

		auto prevObject = _level->getObjectAt(prevCoordinate);
		if (prevObject != nullptr)
		{
			prevObject->afterEnter(creature);
		}
	}
}

const std::string& BlueWall::getFloorFrameName(int shape)
{
	static std::string frameNames[] =
	{
		"blue-floor-0001.png",
		"blue-floor-0002.png",
		"blue-floor-0003.png",
		"blue-floor-0004.png",
		"blue-floor-0005.png",
		"blue-floor-0006.png"
	};

	if (shape == 0x0c)
	{
		return frameNames[2];
	}
	else if (shape == 0x03)
	{
		return frameNames[3];
	}
	else if (shape == 0x06)
	{
		return frameNames[4];
	}
	else if (shape == 0x09)
	{
		return frameNames[5];
	}
	else if (((shape >> 3) & 1) + ((shape >> 1) & 1) > ((shape >> 2) & 1) + (shape & 1))
	{
		return frameNames[1];
	}
	else
	{
		return frameNames[0];
	}
}