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

    void onAdd() override;
    void reset() override;
    
    void beforeEnter(Creature* creature) override;
    void afterEnter(Creature* creature) override;
    void beforeEscape(Creature* creature) override;
    
    std::string& getColorName();
    
protected:
    void toggleSwitchWalls();
    void turnTanks();
    
    cocos2d::Sprite* _rootNode;
    cocos2d::Sprite* _buttonNode;
	size_t _pressCount;
};

#endif
