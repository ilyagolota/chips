#ifndef _CHIPS_CHALLENGE_EXIT_H_
#define _CHIPS_CHALLENGE_EXIT_H_

#include <cocos2d.h>
#include "LevelObject.h"

class Exit : public LevelObject
{
private:
    
public:
    static Exit* create(Level* level, const cocos2d::Vec2& coordinate);
    
    Exit(Level* level, const cocos2d::Vec2& coordinate);
    
    void onAdd() override;
    
    bool isEnterableBy(const Creature* creature, Direction /*direction*/) const override;

    void afterEnter(Creature *creature) override;
    
protected:
    cocos2d::Sprite* _node;
};

#endif
