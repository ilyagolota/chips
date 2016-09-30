#ifndef _CHIPS_CHALLENGE_CLONE_MACHINE_H_
#define _CHIPS_CHALLENGE_CLONE_MACHINE_H_

#include <cocos2d.h>
#include "LevelObject.h"

class CloneMachine : public LevelObject
{
public:
	static CloneMachine* create(Level* level, const cocos2d::Vec2& coordinate);
    
	CloneMachine(Level* level, const cocos2d::Vec2& coordinate);
    
    void performCloning();
    
    bool isEnterableBy(const Creature* creature, Direction direction) const override;
    bool isEscapableBy(const Creature* creature, Direction direction) const override;
    
    void beforeEscape(Creature* creature) override;
    void reset() override;
    
private:
    bool _working;
    cocos2d::Sprite* _floor;
    cocos2d::Sprite* _front;
};

#endif
