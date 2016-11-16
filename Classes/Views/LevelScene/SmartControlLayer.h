#ifndef _CHIPS_CHALLENGE_SMART_CONTROL_LAYER_H_
#define _CHIPS_CHALLENGE_SMART_CONTROL_LAYER_H_

#include <cocos2d.h>
#include "ControlLayer.h"
#include <Level/Level.h>

class SmartControlLayer : public ControlLayer
{
public:
	static SmartControlLayer* create(Level* level);
	SmartControlLayer(Level* level);

    void onLevelTurn() override;
    
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event) override;
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event) override;
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event) override;
    void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event) override;
    
private:
    Level* _level;
    cocos2d::Vec2 _startCoordinate;
    cocos2d::Vec2 _targetCoordinate;
    cocos2d::Vec2 _playerCoordinate;
    bool _hasTarget;
};
    
#endif
