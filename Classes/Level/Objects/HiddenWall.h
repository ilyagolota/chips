#ifndef _CHIPS_CHALLENGE_HIDDEN_WALL_H_
#define _CHIPS_CHALLENGE_HIDDEN_WALL_H_

#include <cocos2d.h>
#include "LevelObject.h"

class HiddenWall : public LevelObject
{
public:
	static HiddenWall* create(Level* level, const cocos2d::Vec2& coordinate, bool permanent);
    
	HiddenWall(Level* level, const cocos2d::Vec2& coordinate, bool permanent);

private:
	bool _permanent;
	bool _known;
};

#endif
