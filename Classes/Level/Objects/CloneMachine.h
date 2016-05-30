#ifndef _CHIPS_CHALLENGE_CLONE_MACHINE_H_
#define _CHIPS_CHALLENGE_CLONE_MACHINE_H_

#include <cocos2d.h>
#include "LevelObject.h"

class CloneMachine : public LevelObject
{
private:
    bool _working;
    
public:
    static CloneMachine* create(const cocos2d::Vec2& coordinate);
    
    CloneMachine(const cocos2d::Vec2& coordinate);
    
    void performCloning();
    
    bool isEnterableBy(const Creature* creature, Direction direction) const override;
    bool isEscapableBy(const Creature* creature, Direction direction) const override;
    
    void beforeLeave(Creature* creature) override;
    void reset() override;
};

#endif
