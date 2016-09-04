#ifndef _CHIPS_CHALLENGE_TELEPORT_H_
#define _CHIPS_CHALLENGE_TELEPORT_H_

#include <cocos2d.h>
#include "LevelObject.h"

class Teleport : public LevelObject
{
private:
    static const cocos2d::Vec2 BOTTOM_POSITION;

	cocos2d::Sprite* _backPart;
	cocos2d::Sprite* _frontPart;

public:
	static Teleport* create(Level* level, const cocos2d::Vec2& coordinate);
    
	Teleport(Level* level, const cocos2d::Vec2& coordinate);
    void beforeEnter(Creature* creature) override;
    void afterEnter(Creature* creature) override;
    void beforeEscape(Creature* creature) override;
    void reset() override;
    
protected:
    Teleport* _findTargetTeleport(Creature* creature);
	void _setDrawAsTwoParts();
	void _setDrawAsSinglePart();
};

#endif
