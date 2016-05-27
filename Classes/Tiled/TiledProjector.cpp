#include "TiledProjector.h"
#include <algorithm>
#include "TiledPhysicsWorld.h"

const int TiledProjector::UNKNOWN_DEPTH = -1;

TiledProjector* TiledProjector::create(const cocos2d::Size& tileSize, const cocos2d::Size& mapSize, TiledPhysicsWorld* physicsWorld)
{
    auto instance = new TiledProjector(tileSize, mapSize, physicsWorld);
    instance->autorelease();
    return instance;
}

TiledProjector::TiledProjector(const cocos2d::Size& tileSize, const cocos2d::Size& mapSize, TiledPhysicsWorld* physicsWorld)
{
    _tileSize = tileSize;
    _mapSize = mapSize;
    _zOrderMap.resize(std::floor(_mapSize.width * _mapSize.height));
    
    _physicsWorld = physicsWorld;
    if (_physicsWorld != nullptr)
    {
        _physicsWorld->retain();
    }
}

TiledProjector::~TiledProjector()
{
    if (_physicsWorld != nullptr)
    {
        _physicsWorld->release();
    }
}

cocos2d::Size TiledProjector::getMapSize() const
{
    return _mapSize;
}

void TiledProjector::clear()
{
    std::fill(_zOrderMap.begin(), _zOrderMap.end(), UNKNOWN_DEPTH);
}

cocos2d::Vec2 TiledProjector::coordinateToPoint(const cocos2d::Vec2& coordinate)
{
    cocos2d::Vec2 point;
    point.x = 0.5f * _tileSize.width * (coordinate.y - coordinate.x) - 0.5f * _tileSize.width;
    point.y = -0.5f * _tileSize.height * (coordinate.y + coordinate.x) - _tileSize.height;
    return point;
}

int TiledProjector::coordinateToZOrder(const cocos2d::Vec2& coordinate)
{
    size_t index = coordinate.x + coordinate.y * _mapSize.width;
    if (_zOrderMap[index] == UNKNOWN_DEPTH)
    {
        unsigned int zOrder = coordinate.x + coordinate.y;
        
        if (coordinate.y > 0)
        {
            size_t northIndex = coordinate.x + (coordinate.y - 1) * _mapSize.width;
            if (_zOrderMap[northIndex] != UNKNOWN_DEPTH)
            {
                unsigned int newZOrder = _zOrderMap[northIndex] + 1;
                
                TileBody northTileBody = _physicsWorld->getBody(cocos2d::Vec2(coordinate.x, (coordinate.y - 1)), 1);
                bool northTileHasSouthSide = (northTileBody & TileBody::OUTER_SOUTH_SIDE) != TileBody::EMPTY;
                bool northTileHasEastSide = (northTileBody & TileBody::OUTER_EAST_SIDE) != TileBody::EMPTY;
                if (northTileHasSouthSide && !northTileHasEastSide)
                {
                    newZOrder += 1;
                }
                
                if (zOrder < newZOrder)
                {
                    zOrder = newZOrder;
                }
            }
        }
        
        if (coordinate.x > 0)
        {
            size_t westIndex = (coordinate.x - 1) + coordinate.y * _mapSize.width;
            if (_zOrderMap[westIndex] != UNKNOWN_DEPTH)
            {
                unsigned int newZOrder = _zOrderMap[westIndex] + 1;
                
                TileBody westTileBody = _physicsWorld->getBody(cocos2d::Vec2(coordinate.x, (coordinate.y - 1)), 1);
                bool westTileHasSouthSide = (westTileBody & TileBody::OUTER_SOUTH_SIDE) != TileBody::EMPTY;
                bool westTileHasEastSide = (westTileBody & TileBody::OUTER_EAST_SIDE) != TileBody::EMPTY;
                if (!westTileHasSouthSide && westTileHasEastSide)
                {
                    newZOrder += 1;
                }
                
                if (zOrder < newZOrder)
                {
                    zOrder = newZOrder;
                }
            }
        }
        
        _zOrderMap[index] = zOrder;
    }
    return _zOrderMap[index];
}

cocos2d::Vec2 TiledProjector::pointToCoordinate(const cocos2d::Vec2& point)
{
    cocos2d::Point coordinate;
    coordinate.x = -1.0f - std::floor(point.y / _tileSize.height - point.x / _tileSize.width);
    coordinate.y = -1.0f - std::floor(point.y / _tileSize.height + point.x / _tileSize.width);
    return coordinate;
}
