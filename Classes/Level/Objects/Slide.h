#ifndef _CHIPS_CHALLENGE_SLIDE_H_
#define _CHIPS_CHALLENGE_SLIDE_H_

#include <cocos2d.h>
#include "LevelObject.h"
#include <Level/TileType.h>

class Level;

class Slide : public LevelObject
{   
public:
    static Slide* create(Level* level, const cocos2d::Vec2& coordinate, TileType tileType);
    Slide(Level* level, const cocos2d::Vec2& coordinate, TileType tileType);
    
    void onAdd() override;
    
    void beforeEnter(Creature* creature) override;
    void afterEnter(Creature* creature) override;
    void beforeEscape(Creature* creature) override;
    
private:
    TileType _type;
    cocos2d::Sprite* _node;
};

#endif
