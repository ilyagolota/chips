#ifndef _CHIPS_CHALLENGE_TELEPORT_H_
#define _CHIPS_CHALLENGE_TELEPORT_H_

#include <cocos2d.h>
#include "LevelObject.h"

class Teleport : public LevelObject
{
public:
    static Teleport* create(Level* level, const cocos2d::Vec2& coordinate);
    
    Teleport(Level* level, const cocos2d::Vec2& coordinate);
    
    void onAdd() override;
    
    void beforeEnter(Creature* creature) override;
    void afterEnter(Creature* creature) override;
    void beforeEscape(Creature* creature) override;
    
private:
    static const cocos2d::Vec2 BOTTOM_POSITION;

    Teleport* findTargetTeleport(Creature* creature);
    
    cocos2d::Sprite* _node;
    cocos2d::Sprite* _abuseNode;
    cocos2d::Sprite* _frontNode;
};

#endif
