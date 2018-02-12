#ifndef _CHIPS_CHALLENGE_BLUE_WALL_H_
#define _CHIPS_CHALLENGE_BLUE_WALL_H_

#include <cocos2d.h>
#include "LevelObject.h"

class Creature;
class Level;

class BlueWall : public LevelObject
{
public:
    static BlueWall* create(Level* level, const cocos2d::Vec2& coordinate, bool fake);
    BlueWall(Level* level, const cocos2d::Vec2& coordinate, bool fake);
    void onAdd() override;
    void reset() override;
    bool isOpenableBy(const Creature *creature, Direction direction) const override;
    void beforeEnter(Creature* creature) override;
    void afterEnter(Creature* creature) override;

private:
    const std::string& getFloorFrameName(int shape);

    bool _fake;
    bool _known;
    cocos2d::Sprite* _node;
    cocos2d::Node* _unknownWallNode;
};

#endif
