#ifndef _CHIPS_CHALLENGE_DOOR_H_
#define _CHIPS_CHALLENGE_DOOR_H_

#include <cocos2d.h>
#include "AudioEngine.h"
#include "LevelData/TileType.h"
#include "LevelObject.h"

class Door : public LevelObject
{
public:
	static Door* create(Level* level, const cocos2d::Vec2& coordinate, TileType type);
    
	Door(Level* level, const cocos2d::Vec2& coordinate, TileType type);
    
    bool isOpenableBy(const Creature *creature, Direction direction) const override;
    
    void beforeEnter(Creature* creature) override;
    
    void setOpen(bool open);
    void reset() override;
    
	TileType getKeyType() const;
	std::string& getColorName() const;

private:
	bool _open;
	TileType _type;
	cocos2d::Sprite* _floor;
	cocos2d::Sprite* _door;
	cocos2d::Sprite* _cover;
};

#endif
