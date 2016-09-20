#ifndef _CHIPS_CHALLENGE_SOCKET_H_
#define _CHIPS_CHALLENGE_SOCKET_H_

#include <cocos2d.h>
#include "LevelObject.h"

class Socket : public LevelObject
{
public:
	static Socket* create(Level* level, const cocos2d::Vec2& coordinate);
    
	Socket(Level* level, const cocos2d::Vec2& coordinate);
    bool isOpenableBy(const Creature* creature, Direction direction) const override;
    void reset();
    void beforeEnter(Creature* creature);
    
private:
    bool _open;
    cocos2d::Sprite* _floor;
    cocos2d::Sprite* _wall;
    cocos2d::Sprite* _cover;
};

#endif
