#ifndef _CHIPS_CHALLENGE_BOMB_H_
#define _CHIPS_CHALLENGE_BOMB_H_

#include <cocos2d.h>
#include "LevelObject.h"

class Bomb : public LevelObject
{
public:
	static Bomb* create(Level* level, const cocos2d::Vec2& coordinate);
    
	Bomb(Level* level, const cocos2d::Vec2& coordinate);
    
    void onAdd() override;
    void reset() override;
    
    void afterEnter(Creature* creature) override;
    
protected:
    bool _exploded;
    cocos2d::Sprite* _rootNode;
    cocos2d::Sprite* _bombNode;
};

#endif
