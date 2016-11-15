#ifndef _CHIPS_CHALLENGE_I_PLAYER_CONTROL_H_
#define _CHIPS_CHALLENGE_I_PLAYER_CONTROL_H_

#include <Tiled/Direction.h>

class IPlayerControl
{
public:
    virtual ~IPlayerControl() { }
    virtual bool isPressed() = 0;
    virtual Direction getSelectedDirection() = 0;
};

#endif
