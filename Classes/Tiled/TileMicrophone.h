#ifndef _CHIPS_CHALLENGE_TILE_MICROPHONE_H_
#define _CHIPS_CHALLENGE_TILE_MICROPHONE_H_

#include <cocos2d.h>

class TileMicrophone : public cocos2d::Ref
{
private:
    cocos2d::Vec2 _coordinate;
    int _maxDistance;
    
public:
    static TileMicrophone* create();
    
    TileMicrophone();
    cocos2d::Vec2 getCoordinate() const;
    void setCoordinate(const cocos2d::Vec2& coordinate);
    int getMaxDistance() const;
    void setMaxDistance(int maxDistance);
};

#endif
