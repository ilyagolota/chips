#include "Door.h"
#include "Tiled/TiledPhysicsWorld.h"
#include "Tiled/TiledProjector.h"
#include "Level/Level.h"
#include "Level/Inventory.h"
#include "Level/Creature.h"

Door* Door::create(Level* level, const cocos2d::Vec2& coordinate, TileType type)
{
	auto instance = new Door(level, coordinate, type);
    instance->autorelease();
    return instance;
}

Door::Door(Level* level, const cocos2d::Vec2& coordinate, TileType type) : LevelObject(coordinate)
{
	_level = level;
    _type = type;
    _open = false;

	int shape = _level->getWallShape(_coordinate);
	bool ew = ((shape >> 3) & 1) + ((shape >> 1) & 1) > ((shape >> 2) & 1) + (shape & 1);

	std::string shapeName = ew ? "-ew" : "-ns";

	_floor = cocos2d::Sprite::createWithSpriteFrameName("way-" + getColorName() + shapeName + ".png");
	_floor->setAnchorPoint(cocos2d::Vec2::ZERO);
	_floor->setPosition(_level->getProjector()->coordinateToPoint(_coordinate) + cocos2d::Vec2(0, -12));
	_floor->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::WALL_Z_ORDER);
	_level->getStage()->addChild(_floor);

	_door = cocos2d::Sprite::createWithSpriteFrameName("door-" + getColorName() + shapeName + ".png");
	_door->setAnchorPoint(cocos2d::Vec2::ZERO);
	_door->setPosition(cocos2d::Vec2(0, 12));
	_floor->addChild(_door);

	_cover = cocos2d::Sprite::createWithSpriteFrameName("way-" + getColorName() + "-front" + shapeName + ".png");
	_cover->setAnchorPoint(cocos2d::Vec2::ZERO);
	_cover->setPosition(cocos2d::Vec2::ZERO);
	_floor->addChild(_cover);
}

void Door::reset()
{
	_open = false;

	_door->stopAllActions();
	_door->setVisible(true);
	_door->setPosition(cocos2d::Vec2(0, 12));

    _level->getPhysicsWorld()->setBody(_coordinate, TileBody::OUTER_BOX, 7);
}

bool Door::isOpenableBy(const Creature *creature, Direction direction) const
{
    if (creature->getType() == CreatureType::CHIP)
    {
        auto keyType = getKeyType();
        if (_level->getInventory()->getItemCount(keyType) > 0)
        {
            return true;
        }
    }
    return false;
}

void Door::beforeEnter(Creature* creature)
{
    if (!_open)
    {
        _open = true;
		_level->getPhysicsWorld()->setBody(_coordinate, TileBody::EMPTY, 7);
        
        if (creature->getType() == CreatureType::CHIP)
        {
            auto keyType = getKeyType();
            if (keyType != TileType::KEY_GREEN)
            {
                if (_level->getInventory()->getItemCount(keyType) > 0)
                {
                    _level->getInventory()->addItem(keyType, -1);
                }
            }
        }

		auto zOrder = _level->getProjector()->coordinateToZOrder(_coordinate);
		_floor->setLocalZOrder(zOrder + Level::BACK_Z_ORDER);

		float duration = _level->getTurnDuration();
		_door->runAction(cocos2d::Sequence::create(
			cocos2d::MoveTo::create(duration, cocos2d::Vec2(0, -78)),
			cocos2d::CallFuncN::create([](cocos2d::Node* door) {
				door->setVisible(false);
			}),
			nullptr
		));

		cocos2d::experimental::AudioEngine::play2d("sounds/door.mp3");
    }
}

std::string& Door::getColorName() const
{
    static std::string __names[] = { "blue", "red", "green", "yellow" };
	size_t index = static_cast<size_t>(_type) - static_cast<size_t>(TileType::DOOR_BLUE);
    if (index >= 4)
    {
        index = 0;
    }
	return __names[index];
}

TileType Door::getKeyType() const
{
    size_t index = static_cast<size_t>(_type) - static_cast<size_t>(TileType::DOOR_BLUE);
    return static_cast<TileType>(static_cast<size_t>(TileType::KEY_BLUE) + index);
}
