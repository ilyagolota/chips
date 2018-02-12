#include "Door.h"
#include "../Level.h"

Door* Door::create(Level* level, const cocos2d::Vec2& coordinate, TileType type)
{
    auto instance = new Door(level, coordinate, type);
    instance->autorelease();
    return instance;
}

Door::Door(Level* level, const cocos2d::Vec2& coordinate, TileType type) : LevelObject(level, coordinate)
{
    _type = type;
    _open = false;
}

void Door::onAdd()
{
    int shape = _level->getWallShape(_coordinate);
    bool ew = ((shape >> 3) & 1) + ((shape >> 1) & 1) > ((shape >> 2) & 1) + (shape & 1);
    std::string shapeName = ew ? "-ew" : "-ns";

    _node = cocos2d::Sprite::createWithSpriteFrameName("way-" + getColorName() + shapeName + ".png");
    _node->setAnchorPoint(cocos2d::Vec2::ZERO);
    _node->setPosition(_level->getProjector()->coordinateToPoint(_coordinate) + cocos2d::Vec2(0, -12));
    _node->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::WALL_Z_ORDER);
    _level->getStage()->addChild(_node);

    _doorNode = cocos2d::Sprite::createWithSpriteFrameName("door-" + getColorName() + shapeName + ".png");
    _doorNode->setAnchorPoint(cocos2d::Vec2::ZERO);
    _doorNode->setPosition(cocos2d::Vec2(0, 12));
    _node->addChild(_doorNode);

    _frontNode = cocos2d::Sprite::createWithSpriteFrameName("way-" + getColorName() + "-front" + shapeName + ".png");
    _frontNode->setAnchorPoint(cocos2d::Vec2::ZERO);
    _frontNode->setPosition(cocos2d::Vec2::ZERO);
    _node->addChild(_frontNode);
    
    _level->getPhysicsWorld()->setBody(_coordinate, TileBody::OUTER_BOX, 7);
}

void Door::reset()
{
    _open = false;

    _doorNode->stopAllActions();
    _doorNode->setVisible(true);
    _doorNode->setPosition(cocos2d::Vec2(0, 12));

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

        _node->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::BACK_Z_ORDER);

        float duration = _level->getTurnDuration();
        _doorNode->runAction(cocos2d::Sequence::create(
            cocos2d::MoveTo::create(duration, cocos2d::Vec2(0, -78)),
            cocos2d::CallFuncN::create([](cocos2d::Node* node) {
                node->setVisible(false);
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
