#ifndef _CHIPS_CHALLENGE_FIRE_H_
#define _CHIPS_CHALLENGE_FIRE_H_

#include <cocos2d.h>
#include "LevelObject.h"

class Fire : public LevelObject
{
private:
    
public:
    static Fire* create(const cocos2d::Vec2& coordinate);
    
    Fire(const cocos2d::Vec2& coordinate);
    
    bool isEnterableBy(const Creature* creature, Direction direction) const override;
    void afterEnter(Creature* creature) override;
};

#endif
