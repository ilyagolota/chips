#ifndef _CHIPS_CHALLENGE_SMART_CONTROL_LAYER_H_
#define _CHIPS_CHALLENGE_SMART_CONTROL_LAYER_H_

#include <cocos2d.h>
#include <Tiled/Direction.h>

class Level;

class SmartControlLayer : public cocos2d::Layer
{
private:
    Level* _level;
    cocos2d::Vec2 _startCoordinate;
    cocos2d::Vec2 _targetCoordinate;
    cocos2d::Vec2 _playerCoordinate;
    bool _hasTarget;
    Direction _selectedDirection;
    
public:
	static SmartControlLayer* create(Level* level);
	SmartControlLayer(Level* level);
    virtual ~SmartControlLayer();

    bool isPressed();
    Direction getSelectedDirection();
    
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event) override;
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event) override;
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event) override;
    void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event) override;
    
protected:
    void _update();
    
};
    
#endif
