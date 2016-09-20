#ifndef _CHIPS_CHALLENGE_BUTTON_H_
#define _CHIPS_CHALLENGE_BUTTON_H_

#include <cocos2d.h>
#include <Level/TileType.h>
#include "LevelObject.h"

class Button : public LevelObject
{
private:
	TileType _type;

public:
	static Button* create(Level* level, const cocos2d::Vec2& coordinate, TileType type);
    
	Button(Level* level, const cocos2d::Vec2& coordinate, TileType type);

    void beforeEnter(Creature* creature) override;
    void afterEnter(Creature* creature) override;
    void beforeEscape(Creature* creature) override;
    void reset() override;
    
    std::string& getColorName();
    
protected:
    void _toggleSwitchWalls();
    void _turnTanks();
    
    cocos2d::Sprite* _floor;
    cocos2d::Sprite* _button;
};

#endif
