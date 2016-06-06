#ifndef _CHIPS_CHALLENGE_TILED_PHYSICS_WORLD_H_
#define _CHIPS_CHALLENGE_TILED_PHYSICS_WORLD_H_

#include <cocos2d.h>
#include "Direction.h"

enum class TileBody : unsigned char
{
    EMPTY = 0x00,
    OUTER_NORTH_SIDE = 0x01,
    OUTER_WEST_SIDE = 0x02,
    OUTER_SOUTH_SIDE = 0x04,
    OUTER_EAST_SIDE = 0x08,
    OUTER_BOX = 0x0F,
    INNER_NORTH_SIDE = 0x10,
    INNER_WEST_SIDE = 0x20,
    INNER_SOUTH_SIDE = 0x40,
    INNER_EAST_SIDE = 0x80,
    INNER_BOX = 0xF0,
};

inline TileBody operator | (TileBody bodyA, TileBody bodyB)
{
    return static_cast<TileBody>(static_cast<char>(bodyA) | static_cast<char>(bodyB));
}

inline TileBody& operator |= (TileBody& bodyA, TileBody bodyB)
{
    bodyA = static_cast<TileBody>(static_cast<char>(bodyA) | static_cast<char>(bodyB));
    return bodyA;
}
    
inline TileBody operator & (TileBody bodyA, TileBody bodyB)
{
    return static_cast<TileBody>(static_cast<char>(bodyA) & static_cast<char>(bodyB));
}

inline TileBody& operator &= (TileBody& bodyA, TileBody bodyB)
{
    bodyA = static_cast<TileBody>(static_cast<char>(bodyA) & static_cast<char>(bodyB));
    return bodyA;
}

inline TileBody operator << (TileBody body, Direction direction)
{
	return static_cast<TileBody>(static_cast<int>(body) << static_cast<int>(direction));
}

class TiledPhysicsWorld : public cocos2d::Ref
{
private:
    static size_t LAYER_COUNT;
    
    std::vector<unsigned int> _bodies;
    size_t _width;
    size_t _height;
    
public:
    static TiledPhysicsWorld* create(const cocos2d::Size& mapSize);
    
    TiledPhysicsWorld(const cocos2d::Size& mapSize);
    cocos2d::Size getMapSize() const;
    void clear();
    TileBody getBody(const cocos2d::Vec2& coordinate, int layerMask);
    void setBody(const cocos2d::Vec2& coordinate, TileBody body, int layerMask);
    bool rayCast(const cocos2d::Vec2& origin, Direction direction, int layerMask) const;
};

#endif
