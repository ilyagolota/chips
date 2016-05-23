#ifndef _CHIPS_CHALLENGE_BLUE_WALL_H_
#define _CHIPS_CHALLENGE_BLUE_WALL_H_

#include <cocos2d.h>
#include "LevelObject.h"

class Creature;

class BlueWall : public LevelObject
{
private:
	bool _fake;
	bool _known;

public:
	static BlueWall* create(const cocos2d::Vec2& coordinate, bool fake);
    
    BlueWall(const cocos2d::Vec2& coordinate, bool fake);
};

#endif
