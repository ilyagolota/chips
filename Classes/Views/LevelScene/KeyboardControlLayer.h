#ifndef _CHIPS_CHALLENGE_KEYBOARD_CONTROL_LAYER_H_
#define _CHIPS_CHALLENGE_KEYBOARD_CONTROL_LAYER_H_

#include <cocos2d.h>
#include <bitset>
#include <Level/Level.h>
#include "ControlLayer.h"

class KeyboardControlLayer : public ControlLayer
{
public:
    static KeyboardControlLayer* create(Level* level);
    KeyboardControlLayer(Level* level);
    
    void onLevelTurn() override;
    
protected:
    void _onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void _onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    
    Level* _level;
    std::bitset<4> _directions;
};

#endif
