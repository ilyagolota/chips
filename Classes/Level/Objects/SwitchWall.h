#ifndef _CHIPS_CHALLENGE_SWITCH_WALL_H_
#define _CHIPS_CHALLENGE_SWITCH_WALL_H_

#include <cocos2d.h>
#include "LevelObject.h"

class SwitchWall : public LevelObject
{
public:
	static SwitchWall* create(Level* level, const cocos2d::Vec2& coordinate, bool initiallyOpen);
    
	SwitchWall(Level* level, const cocos2d::Vec2& coordinate, bool initiallyOpen);
    
    void onAdd() override;
    void reset() override;
    
    bool isOpen();
    void open();
    void close();
    
private:
    bool _initiallyOpen;
    bool _open;
    
    cocos2d::Sprite* _node;
    cocos2d::Sprite* _wallNode;
    cocos2d::Sprite* _frontNode;
};

#endif
