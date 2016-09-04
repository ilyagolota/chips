#ifndef _CHIPS_CHALLENGE_WATER_H_
#define _CHIPS_CHALLENGE_WATER_H_

#include <cocos2d.h>
#include "LevelObject.h"

class Water : public LevelObject
{
public:
	static Water* create(Level* level, const cocos2d::Vec2& coordinate);
    
    Water(Level* level, const cocos2d::Vec2& coordinate);
    virtual ~Water();
    
    bool hasDrawnBlock() const;

private:
	cocos2d::Sprite* _sprite;
};

#endif
