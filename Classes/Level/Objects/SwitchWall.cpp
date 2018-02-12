#include "SwitchWall.h"
#include <Level/Level.h>

SwitchWall* SwitchWall::create(Level* level, const cocos2d::Vec2& coordinate, bool initiallyOpen)
{
    auto instance = new SwitchWall(level, coordinate, initiallyOpen);
    instance->autorelease();
    return instance;
}

SwitchWall::SwitchWall(Level* level, const cocos2d::Vec2& coordinate, bool initiallyOpen) : LevelObject(level, coordinate)
{
    _initiallyOpen = initiallyOpen;
    _open = _initiallyOpen;
}

void SwitchWall::onAdd()
{
    int shape = _level->getWallShape(_coordinate);
    bool ew = ((shape >> 3) & 1) + ((shape >> 1) & 1) > ((shape >> 2) & 1) + (shape & 1);
    std::string shapeName = ew ? "-ew" : "-ns";
    
    _node = cocos2d::Sprite::createWithSpriteFrameName("toggle-wall-floor" + shapeName + ".png");
    _node->setAnchorPoint(cocos2d::Vec2::ZERO);
    _node->setPosition(_level->getProjector()->coordinateToPoint(_coordinate) + cocos2d::Vec2(0, -12));
    if (_open)
    {
        _node->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::BACK_Z_ORDER);
    }
    else
    {
        _node->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::WALL_Z_ORDER);
    }
    _level->getStage()->addChild(_node);
    
    _wallNode = cocos2d::Sprite::createWithSpriteFrameName("toggle-wall" + shapeName + ".png");
    _wallNode->setAnchorPoint(cocos2d::Vec2::ZERO);
    _wallNode->setVisible(!_open);
    _node->addChild(_wallNode);
    
    _frontNode = cocos2d::Sprite::createWithSpriteFrameName("toggle-wall-floor-front" + shapeName + ".png");
    _frontNode->setAnchorPoint(cocos2d::Vec2::ZERO);
    _frontNode->setPosition(cocos2d::Vec2::ZERO);
    _node->addChild(_frontNode);
}

void SwitchWall::reset()
{
    _open = _initiallyOpen;

    _wallNode->stopAllActions();
    _wallNode->setVisible(!_open);
    if (_open)
    {
        _wallNode->setPosition(cocos2d::Vec2(0, -78));
        _node->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::BACK_Z_ORDER);
        _level->getPhysicsWorld()->setBody(_coordinate, TileBody::EMPTY, 7);
    }
    else
    {
        _wallNode->setPosition(cocos2d::Vec2(0, 12));
        _node->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::WALL_Z_ORDER);
        _level->getPhysicsWorld()->setBody(_coordinate, TileBody::OUTER_BOX, 7);
    }
}

bool SwitchWall::isOpen()
{
    return _open;
}

void SwitchWall::open()
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
    }
}

void SwitchWall::close()
{
    if (_open)
    {
        _open = false;
        _level->getPhysicsWorld()->setBody(_coordinate, TileBody::OUTER_BOX, 7);
        
        auto zOrder = _level->getProjector()->coordinateToZOrder(_coordinate);
        _node->setLocalZOrder(zOrder + Level::WALL_Z_ORDER);
        
        float duration = _level->getTurnDuration();
        _wallNode->setVisible(true);
        _wallNode->runAction(cocos2d::MoveTo::create(duration, cocos2d::Vec2(0, 12)));
    }
}
