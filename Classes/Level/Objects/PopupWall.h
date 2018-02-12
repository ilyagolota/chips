#ifndef _CHIPS_CHALLENGE_POPUP_WALL_H_
#define _CHIPS_CHALLENGE_POPUP_WALL_H_

#include <cocos2d.h>
#include "LevelObject.h"

class PopupWall : public LevelObject
{
public:
    static PopupWall* create(Level* level, const cocos2d::Vec2& coordinate);
    
    PopupWall(Level* level, const cocos2d::Vec2& coordinate);

    void onAdd() override;
    void reset() override;
    
    void afterEnter(Creature* creature) override;
    void beforeEscape(Creature* creature) override;

private:
    static cocos2d::Vec2 BOTTOM_POSITION;
    static cocos2d::Vec2 TOP_POSITION;

    bool _open;
    cocos2d::Sprite* _node;
    cocos2d::Sprite* _pistonNode;
    cocos2d::Sprite* _frontNode;
};

#endif
