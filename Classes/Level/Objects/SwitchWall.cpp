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

    int shape = _level->getWallShape(_coordinate);
    bool ew = ((shape >> 3) & 1) + ((shape >> 1) & 1) > ((shape >> 2) & 1) + (shape & 1);
    
    std::string shapeName = ew ? "-ew" : "-ns";
    
    _floor = cocos2d::Sprite::createWithSpriteFrameName("toggle-wall-floor" + shapeName + ".png");
    _floor->setAnchorPoint(cocos2d::Vec2::ZERO);
    _floor->setPosition(_level->getProjector()->coordinateToPoint(_coordinate) + cocos2d::Vec2(0, -12));
    _floor->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::WALL_Z_ORDER);
    _level->getStage()->addChild(_floor);
    
    _wall = cocos2d::Sprite::createWithSpriteFrameName("toggle-wall" + shapeName + ".png");
    _wall->setAnchorPoint(cocos2d::Vec2::ZERO);
    _floor->addChild(_wall);
    
    _cover = cocos2d::Sprite::createWithSpriteFrameName("toggle-wall-floor-front" + shapeName + ".png");
    _cover->setAnchorPoint(cocos2d::Vec2::ZERO);
    _cover->setPosition(cocos2d::Vec2::ZERO);
    _floor->addChild(_cover);
    
    reset();
}

void SwitchWall::reset()
{
    _open = _initiallyOpen;

    _wall->stopAllActions();
    _wall->setVisible(!_open);
    if (_open)
    {
        _wall->setPosition(cocos2d::Vec2(0, -78));
        _level->getPhysicsWorld()->setBody(_coordinate, TileBody::EMPTY, 7);
    }
    else
    {
        _wall->setPosition(cocos2d::Vec2(0, 12));
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
        _floor->setLocalZOrder(zOrder + Level::BACK_Z_ORDER);
        
        float duration = _level->getTurnDuration();
        _wall->runAction(cocos2d::Sequence::create(
            cocos2d::MoveTo::create(duration, cocos2d::Vec2(0, -78)),
            cocos2d::CallFuncN::create([](cocos2d::Node* wall) {
                wall->setVisible(false);
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
        _floor->setLocalZOrder(zOrder + Level::WALL_Z_ORDER);
        
        float duration = _level->getTurnDuration();
        _wall->setVisible(true);
        _wall->runAction(cocos2d::MoveTo::create(duration, cocos2d::Vec2(0, 12)));
    }
}
