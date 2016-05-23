#ifndef _CHIPS_CHALLENGE_EXIT_H_
#define _CHIPS_CHALLENGE_EXIT_H_

#include <cocos2d.h>
#include "LevelObject.h"

class Exit : public LevelObject
{
private:
    
public:
    static Exit* create(const cocos2d::Vec2& coordinate);
    
    Exit(const cocos2d::Vec2& coordinate);
};

#endif
