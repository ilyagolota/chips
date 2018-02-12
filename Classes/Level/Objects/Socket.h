#ifndef _CHIPS_CHALLENGE_SOCKET_H_
#define _CHIPS_CHALLENGE_SOCKET_H_

#include <cocos2d.h>
#include "LevelObject.h"

class Socket : public LevelObject
{
public:
    static Socket* create(Level* level, const cocos2d::Vec2& coordinate);
    
    Socket(Level* level, const cocos2d::Vec2& coordinate);
    
    void onAdd() override;
    void reset() override;
    
    bool isOpenableBy(const Creature* creature, Direction direction) const override;
    
    void beforeEnter(Creature* creature) override;
    
private:
    bool _open;
    cocos2d::Sprite* _node;
    cocos2d::Sprite* _wallNode;
    cocos2d::Sprite* _frontNode;
};

#endif
