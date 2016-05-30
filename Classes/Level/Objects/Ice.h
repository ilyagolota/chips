#ifndef _CHIPS_CHALLENGE_ICE_H_
#define _CHIPS_CHALLENGE_ICE_H_

#include <cocos2d.h>
#include "LevelObject.h"
#include "LevelData/TileType.h"

class Ice : public LevelObject
{
private:
    TileType _type;
    
public:
    static Ice* create(const cocos2d::Vec2& coordinate, TileType type);
    
    Ice(const cocos2d::Vec2& coordinate, TileType type);
    
	bool isEnterableBy(const Creature* creature, Direction direction) const override;
	bool isEscapableBy(const Creature* creature, Direction direction) const override;
    void afterEnter(Creature* creature) override;
};

#endif
