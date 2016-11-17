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
    
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) override;
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) override;
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event) override;
    void onExit() override;
    
protected:
    void onButtonPressed(int directionTag);
    void onButtonReleased(int directionTag);
    cocos2d::MenuItem* getItemForTouch(cocos2d::Touch *touch, const cocos2d::Camera *camera);
    
    enum ButtonState
    {
        NONE,
        PRESSED,
        RELEASED_BEFORE_TURN,
        APPLIED_IN_TURN
    };
    
    Level* _level;
    cocos2d::MenuItem* _selectedItem;
    const cocos2d::Camera* _selectedWithCamera;
    ButtonState _states[4];
};

#endif
