#ifndef _CHIPS_CHALLENGE_DOOR_H_
#define _CHIPS_CHALLENGE_DOOR_H_

#include <cocos2d.h>
#include "AudioEngine.h"
#include <Level/TileType.h>
#include "LevelObject.h"

class Door : public LevelObject
{
public:
	static Door* create(Level* level, const cocos2d::Vec2& coordinate, TileType type);
    
	Door(Level* level, const cocos2d::Vec2& coordinate, TileType type);
    
    void onAdd() override;
    void reset() override;
    
    bool isOpenableBy(const Creature *creature, Direction direction) const override;
    
    void beforeEnter(Creature* creature) override;
    
    void setOpen(bool open);
	TileType getKeyType() const;
	std::string& getColorName() const;

private:
	bool _open;
	TileType _type;
	cocos2d::Sprite* _node;
	cocos2d::Sprite* _doorNode;
	cocos2d::Sprite* _frontNode;
};

#endif
