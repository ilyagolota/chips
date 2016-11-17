#ifndef _CHIPS_CHALLENGE_KEYBOARD_CONTROL_LAYER_H_
#define _CHIPS_CHALLENGE_KEYBOARD_CONTROL_LAYER_H_

#include <cocos2d.h>
#include <map>
#include <Level/Level.h>
#include "ControlLayer.h"

class KeyboardControlLayer : public ControlLayer
{
public:
    static KeyboardControlLayer* create(Level* level);
    KeyboardControlLayer(Level* level);
    
    void onLevelTurn() override;
    
protected:
    enum KeyState
    {
        NONE,
        PRESSED,
        RELEASED_BEFORE_TURN,
        APPLIED_IN_TURN
    };
    
    void _onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void _onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    
    Level* _level;
    std::map<cocos2d::EventKeyboard::KeyCode, KeyState> _states;
};

#endif
