#ifndef _CHIPS_CHALLENGE_SLIDE_H_
#define _CHIPS_CHALLENGE_SLIDE_H_

#include <cocos2d.h>
#include "LevelObject.h"
#include "LevelData/TileType.h"

class Level;

class Slide : public LevelObject
{   
public:
	static Slide* create(Level* level, const cocos2d::Vec2& coordinate, TileType tileType);
	Slide(Level* level, const cocos2d::Vec2& coordinate, TileType tileType);
	void afterEnter(Creature* creature) override;

private:
	TileType _type;
	cocos2d::Sprite* _floor;
};

#endif
