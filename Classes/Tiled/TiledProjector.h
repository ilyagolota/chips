#ifndef _CHIPS_CHALLENGE_TILED_PROJECTOR_H_
#define _CHIPS_CHALLENGE_TILED_PROJECTOR_H_

#include <cocos2d.h>
#include <vector>
#include "Direction.h"

class TiledPhysicsWorld;

class TiledProjector : public cocos2d::Ref
{
private:
    static const int UNKNOWN_DEPTH;
    
    cocos2d::Size _tileSize;
    cocos2d::Size _mapSize;
    std::vector<int> _zOrderMap;
    TiledPhysicsWorld* _physicsWorld;
    
public:
    static TiledProjector* create(const cocos2d::Size& tileSize, const cocos2d::Size& mapSize, TiledPhysicsWorld* physicsWorld);
    
    TiledProjector(const cocos2d::Size& tileSize, const cocos2d::Size& mapSize, TiledPhysicsWorld* physicsWorld);
    ~TiledProjector();
    cocos2d::Size getMapSize() const;
    void clear();
    cocos2d::Vec2 coordinateToPoint(const cocos2d::Vec2& coordinate);
    int coordinateToZOrder(const cocos2d::Vec2& coordinate);
    cocos2d::Vec2 pointToCoordinate(const cocos2d::Vec2& point);
};

#endif
