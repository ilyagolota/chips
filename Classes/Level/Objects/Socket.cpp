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
}

void Socket::onAdd()
{
    int shape = _level->getWallShape(_coordinate);
    bool ew = ((shape >> 3) & 1) + ((shape >> 1) & 1) > ((shape >> 2) & 1) + (shape & 1);
    std::string shapeName = ew ? "-ew" : "-ns";
    
    _node = cocos2d::Sprite::createWithSpriteFrameName("socket-floor" + shapeName + ".png");
    _node->setAnchorPoint(cocos2d::Vec2::ZERO);
    _node->setPosition(_level->getProjector()->coordinateToPoint(_coordinate) + cocos2d::Vec2(0, -12));
    _node->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::WALL_Z_ORDER);
    _level->getStage()->addChild(_node);
    
    _wallNode = cocos2d::Sprite::createWithSpriteFrameName("socket" + shapeName + ".png");
    _wallNode->setAnchorPoint(cocos2d::Vec2::ZERO);
    _wallNode->setPosition(cocos2d::Vec2(0, 12));
    _node->addChild(_wallNode);
    
    _frontNode = cocos2d::Sprite::createWithSpriteFrameName("socket-floor-front" + shapeName + ".png");
    _frontNode->setAnchorPoint(cocos2d::Vec2::ZERO);
    _frontNode->setPosition(cocos2d::Vec2::ZERO);
    _node->addChild(_frontNode);
}

void Socket::reset()
{
    _open = false;
    
    _wallNode->stopAllActions();
    _wallNode->setVisible(true);
    _wallNode->setPosition(cocos2d::Vec2(0, 12));
    
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
        _node->setLocalZOrder(zOrder + Level::BACK_Z_ORDER);
        
        float duration = _level->getTurnDuration();
        _wallNode->runAction(cocos2d::Sequence::create(
            cocos2d::MoveTo::create(duration, cocos2d::Vec2(0, -78)),
            cocos2d::CallFuncN::create([](cocos2d::Node* wallNode) {
                wallNode->setVisible(false);
            }),
            nullptr
        ));
        
        cocos2d::experimental::AudioEngine::play2d("sounds/door.mp3");
    }
}
