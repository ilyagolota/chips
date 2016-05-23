#ifndef _CHIPS_CHALLENGE_TELEPORT_H_
#define _CHIPS_CHALLENGE_TELEPORT_H_

#include <cocos2d.h>
#include "LevelObject.h"

class Teleport : public LevelObject
{
private:
    
public:
    static Teleport* create(const cocos2d::Vec2& coordinate);
    
    Teleport(const cocos2d::Vec2& coordinate);
};

#endif
