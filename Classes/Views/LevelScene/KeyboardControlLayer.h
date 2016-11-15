#ifndef _CHIPS_CHALLENGE_KEYBOARD_CONTROL_LAYER_H_
#define _CHIPS_CHALLENGE_KEYBOARD_CONTROL_LAYER_H_

#include <cocos2d.h>
#include <bitset>
#include "IPlayerControl.h"

class KeyboardControlLayer : public cocos2d::Layer, public IPlayerControl
{
public:
    static KeyboardControlLayer* create();
    KeyboardControlLayer();
    
    bool isPressed() override;
    Direction getSelectedDirection() override;
    
protected:
    void _onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void _onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    
    std::bitset<4> _directions;
};

#endif
