#ifndef _CHIPS_CHALLENGE_BUTTON_CONTROL_LAYER_H_
#define _CHIPS_CHALLENGE_BUTTON_CONTROL_LAYER_H_

#include <cocos2d.h>
#include "ControlLayer.h"
#include <Level/Level.h>

class ButtonControlLayer : public ControlLayer
{
public:
    static ButtonControlLayer* create(Level* level);
    ButtonControlLayer(Level* level);
    
    void onLevelTurn() override;
    
protected:
    cocos2d::Menu* _menu;
    Level* _level;
};

#endif
