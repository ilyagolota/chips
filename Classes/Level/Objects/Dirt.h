#ifndef _CHIPS_CHALLENGE_DIRT_H_
#define _CHIPS_CHALLENGE_DIRT_H_

#include <cocos2d.h>
#include "LevelObject.h"

class Dirt : public LevelObject
{
public:
    static Dirt* create(Level* level, const cocos2d::Vec2& coordinate);
    
    Dirt(Level* level, const cocos2d::Vec2& coordinate);
    
    void onAdd() override;
    void reset() override;
    
    bool isEnterableBy(const Creature* creature, Direction direction) const override;
    
    void afterEnter(Creature* creature) override;
    
protected:
    bool _removed;
    cocos2d::Sprite* _node;
    cocos2d::Sprite* _dirtNode;
};

#endif
