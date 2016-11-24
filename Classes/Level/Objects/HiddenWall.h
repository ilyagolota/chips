#ifndef _CHIPS_CHALLENGE_HIDDEN_WALL_H_
#define _CHIPS_CHALLENGE_HIDDEN_WALL_H_

#include <cocos2d.h>
#include "LevelObject.h"

class HiddenWall : public LevelObject
{
public:
	static HiddenWall* create(Level* level, const cocos2d::Vec2& coordinate, bool permanent);
    
	HiddenWall(Level* level, const cocos2d::Vec2& coordinate, bool permanent);
    bool isOpenableBy(const Creature *creature, Direction direction) const override;
    void beforeEnter(Creature* creature) override;
    void afterEnter(Creature* creature) override;
    
private:
	const std::string& getFloorFrameName(int shape);

	bool _permanent;
	bool _known;
    cocos2d::Sprite* _floor;
    cocos2d::Sprite* _wall;
};

#endif
