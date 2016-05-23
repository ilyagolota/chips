#ifndef _CHIPS_CHALLENGE_DOOR_H_
#define _CHIPS_CHALLENGE_DOOR_H_

#include <cocos2d.h>
#include "LevelData/TileType.h"
#include "LevelObject.h"

class Door : public LevelObject
{
private:
    bool _open;
	TileType _type;
    
public:
    static Door* create(const cocos2d::Vec2& coordinate, TileType type);
    
    Door(const cocos2d::Vec2& coordinate, TileType type);
    
    bool isOpenableBy(const Creature *creature, Direction direction) const override;
    
    void beforeEnter(Creature* creature) override;
    
    void setOpen(bool open);
    void reset() override;
    
	TileType getKeyType() const;
	std::string& getColorName() const;

protected:
    void buildNodes() override;
    void destroyNodes() override;
};

#endif
