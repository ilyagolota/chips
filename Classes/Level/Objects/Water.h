#ifndef _CHIPS_CHALLENGE_WATER_H_
#define _CHIPS_CHALLENGE_WATER_H_

#include <cocos2d.h>
#include "LevelObject.h"

class Water : public LevelObject
{
public:
	static Water* create(Level* level, const cocos2d::Vec2& coordinate);
    
    Water(Level* level, const cocos2d::Vec2& coordinate);
    
    void onAdd() override;
	void reset() override;
    
	bool isEnterableBy(const Creature *creature, Direction direction) const override;
	void beforeEnter(Creature* creature) override;
    void afterEnter(Creature *creature) override;
    
    bool hasDrawnBlock() const;

private:
	cocos2d::Sprite* _node;
	cocos2d::Sprite* _blockNode;
    cocos2d::Sprite* _splash;
    
	enum {
		WATER_STATE,
		DIRT_STATE,
		FLOOR_STATE
	} _state;
};

#endif
