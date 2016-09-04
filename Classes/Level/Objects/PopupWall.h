#ifndef _CHIPS_CHALLENGE_POPUP_WALL_H_
#define _CHIPS_CHALLENGE_POPUP_WALL_H_

#include <cocos2d.h>
#include "LevelObject.h"

class PopupWall : public LevelObject
{
public:
	static PopupWall* create(Level* level, const cocos2d::Vec2& coordinate);
    
	PopupWall(Level* level, const cocos2d::Vec2& coordinate);
};

#endif
