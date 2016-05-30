#ifndef _CHIPS_CHALLENGE_WATER_H_
#define _CHIPS_CHALLENGE_WATER_H_

#include <cocos2d.h>
#include "LevelObject.h"

class Water : public LevelObject
{
private:
    
public:
    static Water* create(const cocos2d::Vec2& coordinate);
    
    Water(const cocos2d::Vec2& coordinate);
    virtual ~Water();
    
    bool hasDrawnBlock() const;
};

#endif
