#ifndef _CHIPS_CHALLENGE_SLIDE_H_
#define _CHIPS_CHALLENGE_SLIDE_H_

#include <cocos2d.h>
#include "LevelObject.h"
#include "LevelData/TileType.h"

class Slide : public LevelObject
{
private:
    TileType _type;
    
public:
	static Slide* create(const cocos2d::Vec2& coordinate, TileType tileType);
    
    Slide(const cocos2d::Vec2& coordinate, TileType tileType);
};

#endif
