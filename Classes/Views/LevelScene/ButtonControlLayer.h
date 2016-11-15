#ifndef _CHIPS_CHALLENGE_BUTTON_CONTROL_LAYER_H_
#define _CHIPS_CHALLENGE_BUTTON_CONTROL_LAYER_H_

#include <cocos2d.h>
#include "IPlayerControl.h"

class ButtonControlLayer : public cocos2d::Layer, public IPlayerControl
{
public:
    static ButtonControlLayer* create();
    ButtonControlLayer();
    
    bool isPressed() override;
    Direction getSelectedDirection() override;
    
protected:
    cocos2d::Menu* _menu;
};

#endif
