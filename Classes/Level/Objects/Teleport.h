#ifndef _CHIPS_CHALLENGE_TELEPORT_H_
#define _CHIPS_CHALLENGE_TELEPORT_H_

#include <cocos2d.h>
#include "LevelObject.h"

class Teleport : public LevelObject
{
public:
	static Teleport* create(Level* level, const cocos2d::Vec2& coordinate);
    
	Teleport(Level* level, const cocos2d::Vec2& coordinate);
    void beforeEnter(Creature* creature) override;
    void afterEnter(Creature* creature) override;
    void beforeEscape(Creature* creature) override;
    void reset() override;
    
private:
	static const cocos2d::Vec2 BOTTOM_POSITION;

	Teleport* _findTargetTeleport(Creature* creature);
	
	cocos2d::Sprite* _floor;
	cocos2d::Sprite* _abuse;
	cocos2d::Sprite* _front;
};

#endif
