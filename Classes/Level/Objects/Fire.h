#ifndef _CHIPS_CHALLENGE_FIRE_H_
#define _CHIPS_CHALLENGE_FIRE_H_

#include <cocos2d.h>
#include "LevelObject.h"

class Fire : public LevelObject
{
private:
    
public:
	static Fire* create(Level* level, const cocos2d::Vec2& coordinate);
    
	Fire(Level* level, const cocos2d::Vec2& coordinate);
    
    void onAdd() override;
    
    bool isEnterableBy(const Creature* creature, Direction direction) const override;
    
    void afterEnter(Creature* creature) override;
    
private:
    cocos2d::Sprite* _node;
    cocos2d::Sprite* _fire1Node;
    cocos2d::Sprite* _fire2Node;
    cocos2d::Sprite* _fire3Node;
};

#endif
