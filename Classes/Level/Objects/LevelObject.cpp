#include "LevelObject.h"
#include <Tiled/TiledProjector.h>
#include <Level/Level.h>

LevelObject::LevelObject(const cocos2d::Vec2& coordinate)
{
    _level = nullptr;
    _coordinate = coordinate;
    _mainNode = nullptr;
    _body = TileBody::EMPTY;
}

cocos2d::Vec2 LevelObject::getCoordinate() const
{
    return _coordinate;
}

Level* LevelObject::getLevel() const
{
    return _level;
}

void LevelObject::setLevel(Level* level)
{
    if (_level != level)
    {
        if (_level != nullptr)
        {
            if (_mainNode != nullptr)
            {
                _level->getStage()->removeChild(_mainNode);
                for (auto node : _additionalNodes)
                {
                    _level->getStage()->removeChild(node);
                }
            }
            _level->getPhysicsWorld()->setBody(_coordinate, TileBody::EMPTY, 7);
        }
        _level = level;
        if (_level != nullptr)
        {
            if (_mainNode != nullptr)
            {
                _mainNode->setLocalZOrder(_mainNode->getLocalZOrder() + Level::Z_ORDER_PER_TILE * _level->getProjector()->coordinateToZOrder(_coordinate));
                _mainNode->setPosition(_mainNode->getPosition() + _level->getProjector()->coordinateToPoint(_coordinate));
                _level->getStage()->addChild(_mainNode);
                
                for (auto node : _additionalNodes)
                {
                    node->setLocalZOrder(node->getLocalZOrder() + Level::Z_ORDER_PER_TILE * _level->getProjector()->coordinateToZOrder(_coordinate));
                    node->setPosition(node->getPosition() + _level->getProjector()->coordinateToPoint(_coordinate));
                    _level->getStage()->addChild(node);
                }
            }
            _level->getPhysicsWorld()->setBody(_coordinate, _body, 7);
        }
    }
}

bool LevelObject::isOpenableBy(const Creature *creature, Direction direction) const
{
    return false;
}

bool LevelObject::isEnterableBy(const Creature *creature, Direction direction) const
{
    return true;
}

bool LevelObject::isEscapableBy(const Creature *creature, Direction direction) const
{
    return true;
}

void LevelObject::beforeEnter(Creature* creature)
{
    CC_UNUSED_PARAM(creature);
}

void LevelObject::afterEnter(Creature* creature)
{
    CC_UNUSED_PARAM(creature);
}

void LevelObject::beforeLeave(Creature* creature)
{
    CC_UNUSED_PARAM(creature);
}

void LevelObject::afterLeave(Creature* creature)
{
    CC_UNUSED_PARAM(creature);
}

void LevelObject::reset()
{ }

void LevelObject::addNode(cocos2d::Node* node)
{
    if (_mainNode == nullptr)
    {
        _mainNode = node;
        _mainNode->retain();
    }
    else
    {
        _additionalNodes.pushBack(node);
    }
    if (_level != nullptr)
    {
        _level->getStage()->addChild(node);
    }
}

void LevelObject::removeNode(cocos2d::Node* node)
{
    if (_mainNode == node)
    {
        _mainNode->release();
        if (_additionalNodes.empty())
        {
            _mainNode = nullptr;
        }
        else
        {
            auto backNode = _additionalNodes.back();
            _mainNode = backNode;
            _mainNode->retain();
        }
    }
    else
    {
        _additionalNodes.eraseObject(node);
    }
    if (_level != nullptr)
    {
        _level->getStage()->removeChild(node);
    }
}

void LevelObject::setBody(TileBody body, int layerMask)
{
    _body = body;
    if (_level)
    {
        _level->getPhysicsWorld()->setBody(_coordinate, body, layerMask);
    }
}
