#ifndef _CHIPS_CHALLENGE_HINT_BUTTON_H_
#define _CHIPS_CHALLENGE_HINT_BUTTON_H_

#include <cocos2d.h>
#include "LevelObject.h"

class HintButton : public LevelObject
{
private:
    
public:
    static HintButton* create(Level* level, const cocos2d::Vec2& coordinate);
    HintButton(Level* level, const cocos2d::Vec2& coordinate);

    void onAdd() override;
    
    void beforeEnter(Creature *creature) override;
    void beforeEscape(Creature *creature) override;
    
private:
    cocos2d::Sprite* _node;
};

#endif
