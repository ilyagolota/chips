#ifndef _CHIPS_CHALLENGE_DIRT_H_
#define _CHIPS_CHALLENGE_DIRT_H_

#include <cocos2d.h>
#include "LevelObject.h"

class Dirt : public LevelObject
{
private:
    
public:
    static Dirt* create(const cocos2d::Vec2& coordinate);
    
    Dirt(const cocos2d::Vec2& coordinate);
};

#endif
