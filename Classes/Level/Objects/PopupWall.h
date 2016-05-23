#ifndef _CHIPS_CHALLENGE_POPUP_WALL_H_
#define _CHIPS_CHALLENGE_POPUP_WALL_H_

#include <cocos2d.h>
#include "LevelObject.h"

class PopupWall : public LevelObject
{
private:
    
public:
    static PopupWall* create(const cocos2d::Vec2& coordinate);
    
    PopupWall(const cocos2d::Vec2& coordinate);
};

#endif
