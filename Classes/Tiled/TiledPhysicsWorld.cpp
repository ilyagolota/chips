#include "TiledPhysicsWorld.h"

size_t TiledPhysicsWorld::LAYER_COUNT = 3;

TiledPhysicsWorld* TiledPhysicsWorld::create(const cocos2d::Size& mapSize)
{
    auto instance = new TiledPhysicsWorld(mapSize);
    instance->autorelease();
    return instance;
}

TiledPhysicsWorld::TiledPhysicsWorld(const cocos2d::Size& mapSize)
{
    _width = mapSize.width;
    _height = mapSize.height;
    _bodies.resize(_width * _height);
    std::fill(_bodies.begin(), _bodies.end(), 0);
}

void TiledPhysicsWorld::clear()
{
    std::fill(_bodies.begin(), _bodies.end(), 0);
}

cocos2d::Size TiledPhysicsWorld::getMapSize() const
{
    return cocos2d::Size(_width, _height);
}

TileBody TiledPhysicsWorld::getBody(const cocos2d::Vec2& coordinate, int layerMask)
{
    auto body = TileBody::EMPTY;
    if (coordinate.x >= 0 && coordinate.y >= 0 && coordinate.x < _width && coordinate.y < _height)
    {
        size_t index = _width * static_cast<size_t>(coordinate.y) + static_cast<size_t>(coordinate.x);
        for (int layerIndex = 0; layerIndex < LAYER_COUNT; layerIndex++)
        {
            if (((layerMask >> layerIndex) & 1) != 0)
            {
                int layerOffset = layerIndex * 8;
                body |= static_cast<TileBody>((_bodies[index] & (0xff << layerOffset)) >> layerOffset);
            }
        }
    }
    return body;
}

void TiledPhysicsWorld::setBody(const cocos2d::Vec2& coordinate, TileBody body, int layerMask)
{
    if (coordinate.x >= 0 && coordinate.y >= 0 && coordinate.x < _width && coordinate.y < _height)
    {
        size_t index = _width * static_cast<size_t>(coordinate.y) + static_cast<size_t>(coordinate.x);
        for (int layerIndex = 0; layerIndex < LAYER_COUNT; layerIndex++)
        {
            if (((layerMask >> layerIndex) & 1) != 0)
            {
                int layerOffset = layerIndex * 8;
                _bodies[index] = _bodies[index] & ~(0xffu << layerOffset);
                _bodies[index] |= static_cast<int>(body) << layerOffset;
            }
        }
    }
}

bool TiledPhysicsWorld::rayCast(const cocos2d::Vec2& origin, Direction direction, int layerMask) const
{
    if (direction == Direction::NONE)
    {
        return false;
    }
    
    if (origin.x < 0 && origin.y >= 0 && origin.x >= _width && origin.y >= _height)
    {
        return true;
    }
    
    size_t originIndex = _width * static_cast<size_t>(origin.y) + static_cast<size_t>(origin.x);
    unsigned int outDirectionMask = 1 << (static_cast<unsigned int>(direction) + 4);
    for (int layerIndex = 0; layerIndex < LAYER_COUNT; layerIndex++)
    {
        if (((layerMask >> layerIndex) & 1) != 0)
        {
            unsigned int body = _bodies[originIndex] >> (layerIndex * 8);
            if ((body & outDirectionMask) != 0)
            {
                return true;
            }
        }
    }
    
    auto target = origin + toVec2(direction);
    if (target.x < 0 && target.y >= 0 && target.x >= _width && target.y >= _height)
    {
        return true;
    }
    
    size_t targetIndex = _width * static_cast<size_t>(target.y) + static_cast<size_t>(target.x);
    unsigned int directionMask = 1 << static_cast<unsigned int>(direction);
    for (int layerIndex = 0; layerIndex < LAYER_COUNT; layerIndex++)
    {
        if (((layerMask >> layerIndex) & 1) != 0)
        {
            unsigned int body = _bodies[targetIndex] >> (layerIndex << 8);
            if ((body & directionMask) != 0)
            {
                return true;
            }
        }
    }
    
    return false;
}
