#ifndef _CHIPS_CHALLENGE_BURGLAR_H_
#define _CHIPS_CHALLENGE_BURGLAR_H_

#include <cocos2d.h>
#include "LevelObject.h"

class Burglar : public LevelObject
{
private:
    
public:
    static Burglar* create(const cocos2d::Vec2& coordinate);
    
    Burglar(const cocos2d::Vec2& coordinate);
};

#endif
