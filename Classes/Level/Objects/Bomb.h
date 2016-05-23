#ifndef _CHIPS_CHALLENGE_BOMB_H_
#define _CHIPS_CHALLENGE_BOMB_H_

#include <cocos2d.h>
#include "LevelObject.h"

class Bomb : public LevelObject
{
private:
	bool _exploded;

public:
    static Bomb* create(const cocos2d::Vec2& coordinate);
    
    Bomb(const cocos2d::Vec2& coordinate);
};

#endif
