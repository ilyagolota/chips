#ifndef _CHIPS_CHALLENGE_ICE_H_
#define _CHIPS_CHALLENGE_ICE_H_

#include <cocos2d.h>
#include <Level/TileType.h>
#include "LevelObject.h"

class Ice : public LevelObject
{
public:
	static Ice* create(Level* level, const cocos2d::Vec2& coordinate, TileType type);
    
	Ice(Level* level, const cocos2d::Vec2& coordinate, TileType type);
    
    void onAdd() override;
    
    void beforeEnter(Creature* creature) override;
    void afterEnter(Creature* creature) override;
    void beforeEscape(Creature* creature) override;

private:
    TileType _type;
	cocos2d::Sprite* _node;
	cocos2d::Sprite* _frontNode;
	cocos2d::Sprite* _backNode;
    
};

#endif
