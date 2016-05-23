#ifndef _CHIPS_CHALLENGE_HIDDEN_WALL_H_
#define _CHIPS_CHALLENGE_HIDDEN_WALL_H_

#include <cocos2d.h>
#include "LevelObject.h"

class HiddenWall : public LevelObject
{
private:
	bool _permanent;
	bool _known;

public:
    static HiddenWall* create(const cocos2d::Vec2& coordinate, bool permanent);
    
	HiddenWall(const cocos2d::Vec2& coordinate, bool permanent);
};

#endif
