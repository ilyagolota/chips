#ifndef _CHIPS_CHALLENGE_SWITCH_WALL_H_
#define _CHIPS_CHALLENGE_SWITCH_WALL_H_

#include <cocos2d.h>
#include "LevelObject.h"

class SwitchWall : public LevelObject
{
private:
    bool _initiallyOpen;
    bool _open;
    
public:
	static SwitchWall* create(const cocos2d::Vec2& coordinate, bool initiallyOpen);
    
	SwitchWall(const cocos2d::Vec2& coordinate, bool initiallyOpen);
    bool isOpen();
    void open();
    void close();
};

#endif
