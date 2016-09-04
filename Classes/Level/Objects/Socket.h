#ifndef _CHIPS_CHALLENGE_SOCKET_H_
#define _CHIPS_CHALLENGE_SOCKET_H_

#include <cocos2d.h>
#include "LevelObject.h"

class Socket : public LevelObject
{
private:
    bool _open;
    
public:
	static Socket* create(Level* level, const cocos2d::Vec2& coordinate);
    
	Socket(Level* level, const cocos2d::Vec2& coordinate);
    bool isOpenableBy(const Creature* creature, Direction direction) const override;
};

#endif
