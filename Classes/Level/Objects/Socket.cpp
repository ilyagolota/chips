#include "Socket.h"
#include <Level/Level.h>

Socket* Socket::create(Level* level, const cocos2d::Vec2& coordinate)
{
    auto instance = new Socket(level, coordinate);
    instance->autorelease();
    return instance;
}

Socket::Socket(Level* level, const cocos2d::Vec2& coordinate) : LevelObject(level, coordinate)
{
    _open = false;
    _open = false;
    
    int shape = _level->getWallShape(_coordinate);
    bool ew = ((shape >> 3) & 1) + ((shape >> 1) & 1) > ((shape >> 2) & 1) + (shape & 1);
    
    std::string shapeName = ew ? "-ew" : "-ns";
    
    _floor = cocos2d::Sprite::createWithSpriteFrameName("socket-floor" + shapeName + ".png");
    _floor->setAnchorPoint(cocos2d::Vec2::ZERO);
    _floor->setPosition(_level->getProjector()->coordinateToPoint(_coordinate) + cocos2d::Vec2(0, -12));
    _floor->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::WALL_Z_ORDER);
    _level->getStage()->addChild(_floor);
    
    _wall = cocos2d::Sprite::createWithSpriteFrameName("socket" + shapeName + ".png");
    _wall->setAnchorPoint(cocos2d::Vec2::ZERO);
    _wall->setPosition(cocos2d::Vec2(0, 12));
    _floor->addChild(_wall);
    
    _cover = cocos2d::Sprite::createWithSpriteFrameName("socket-floor-front" + shapeName + ".png");
    _cover->setAnchorPoint(cocos2d::Vec2::ZERO);
    _cover->setPosition(cocos2d::Vec2::ZERO);
    _floor->addChild(_cover);
}

void Socket::reset()
{
    _open = false;
    
    _wall->stopAllActions();
    _wall->setVisible(true);
    _wall->setPosition(cocos2d::Vec2(0, 12));
    
    _level->getPhysicsWorld()->setBody(_coordinate, TileBody::OUTER_BOX, 7);
}

bool Socket::isOpenableBy(const Creature *creature, Direction direction) const
{
    if (creature->getType() == CreatureType::CHIP)
    {
        if (_level->getInventory()->getItemCount(TileType::IC_CHIP) >= _level->getConfig()->getChipsRequired())
        {
            return true;
        }
    }
    return false;
}

void Socket::beforeEnter(Creature* creature)
{
    if (!_open)
    {
        _open = true;
        _level->getPhysicsWorld()->setBody(_coordinate, TileBody::EMPTY, 7);

        auto zOrder = _level->getProjector()->coordinateToZOrder(_coordinate);
        _floor->setLocalZOrder(zOrder + Level::BACK_Z_ORDER);
        
        float duration = _level->getTurnDuration();
        _wall->runAction(cocos2d::Sequence::create(
            cocos2d::MoveTo::create(duration, cocos2d::Vec2(0, -78)),
            cocos2d::CallFuncN::create([](cocos2d::Node* wall) {
                wall->setVisible(false);
            }),
            nullptr
        ));
        
        cocos2d::experimental::AudioEngine::play2d("sounds/door.mp3");
    }
}
