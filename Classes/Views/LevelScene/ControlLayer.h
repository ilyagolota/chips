#ifndef _CHIPS_CHALLENGE_CONTROL_LAYER_H_
#define _CHIPS_CHALLENGE_CONTROL_LAYER_H_

#include <cocos2d.h>

class ControlLayer : public cocos2d::Layer
{
public:
    virtual void onLevelTurn() = 0;
};

#endif
