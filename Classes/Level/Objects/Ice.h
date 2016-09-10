#ifndef _CHIPS_CHALLENGE_ICE_H_
#define _CHIPS_CHALLENGE_ICE_H_

#include <cocos2d.h>
#include "LevelObject.h"
#include "LevelData/TileType.h"

class Ice : public LevelObject
{
public:
	static Ice* create(Level* level, const cocos2d::Vec2& coordinate, TileType type);
    
	Ice(Level* level, const cocos2d::Vec2& coordinate, TileType type);
	bool isEnterableBy(const Creature* creature, Direction direction) const override;
	bool isEscapableBy(const Creature* creature, Direction direction) const override;
    void beforeEnter(Creature* creature) override;
    void afterEnter(Creature* creature) override;
    void beforeEscape(Creature* creature) override;

private:
    
    TileType _type;
	cocos2d::Sprite* _sprite;
	cocos2d::Sprite* _frontWall;
	cocos2d::Sprite* _backWall;
};

#endif
