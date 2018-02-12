#include "PopupWall.h"
#include <Level/Level.h>

cocos2d::Vec2 PopupWall::BOTTOM_POSITION(0, -17);
cocos2d::Vec2 PopupWall::TOP_POSITION(0, 25);

PopupWall* PopupWall::create(Level* level, const cocos2d::Vec2& coordinate)
{
    auto instance = new PopupWall(level, coordinate);
    instance->autorelease();
    return instance;
}

PopupWall::PopupWall(Level* level, const cocos2d::Vec2& coordinate) : LevelObject(level, coordinate)
{
    _open = true;
}

void PopupWall::onAdd()
{
    _node = cocos2d::Sprite::createWithSpriteFrameName("trap.png");
    _node->setPosition(_level->getProjector()->coordinateToPoint(_coordinate) + cocos2d::Vec2(0, -12));
    _node->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::BACK_Z_ORDER);
    _node->setAnchorPoint(cocos2d::Vec2::ZERO);
    _level->getStage()->addChild(_node);

    _pistonNode = cocos2d::Sprite::createWithSpriteFrameName("popup-wall-piston.png");
    _pistonNode->setPosition(BOTTOM_POSITION);
    _pistonNode->setAnchorPoint(cocos2d::Vec2::ZERO);
    _node->addChild(_pistonNode);

    _frontNode = cocos2d::Sprite::createWithSpriteFrameName("trap-front.png");
    _frontNode->setPosition(cocos2d::Vec2::ZERO);
    _frontNode->setAnchorPoint(cocos2d::Vec2::ZERO);
    _node->addChild(_frontNode);
    
    _level->getPhysicsWorld()->setBody(_coordinate, TileBody::OUTER_BOX, 2 | 4);
    _level->getPhysicsWorld()->setBody(_coordinate, TileBody::EMPTY, 1);
}

void PopupWall::reset()
{
    _open = true;
    _pistonNode->setPosition(BOTTOM_POSITION);
    
    _level->getPhysicsWorld()->setBody(_coordinate, TileBody::OUTER_BOX, 2 | 4);
    _level->getPhysicsWorld()->setBody(_coordinate, TileBody::EMPTY, 1);

    _node->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::BACK_Z_ORDER);
}

void PopupWall::afterEnter(Creature* /*creature*/)
{
    _level->getPhysicsWorld()->setBody(_coordinate, TileBody::OUTER_BOX, 1);
}

void PopupWall::beforeEscape(Creature* /*creature*/)
{
    if (_open)
    {
        _open = false;
        auto duration = _level->getTurnDuration();

        _node->runAction(cocos2d::Sequence::create(
            cocos2d::DelayTime::create(0.5f * duration),
            cocos2d::CallFunc::create([this]() {
                _node->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::WALL_Z_ORDER);

                auto duration = _level->getTurnDuration();
                _pistonNode->runAction(cocos2d::MoveTo::create(duration, TOP_POSITION));
            }),
            nullptr
        ));
    }
}
