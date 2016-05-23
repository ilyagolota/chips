#ifndef _CHIPS_CHALLENGE_BUTTON_H_
#define _CHIPS_CHALLENGE_BUTTON_H_

#include <cocos2d.h>
#include "LevelObject.h"
#include "LevelData/TileType.h"

class Button : public LevelObject
{
private:
	TileType _type;

public:
    static Button* create(const cocos2d::Vec2& coordinate, TileType type);
    
	Button(const cocos2d::Vec2& coordinate, TileType type);

    void afterEnter(Creature* creature) override;
    void beforeLeave(Creature* creature) override;
    
    std::string& getColorName();
    
protected:
    void _toggleSwitchWalls();
    void _turnTanks();
};

#endif
