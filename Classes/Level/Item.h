#ifndef _CHIPS_CHALLENGE_ITEM_H_
#define _CHIPS_CHALLENGE_ITEM_H_

#include <cocos2d.h>
#include "LevelData/TileType.h"

class Level;

class Item : public cocos2d::Ref
{
private:
	Level* _level;
    TileType _type;
    cocos2d::Sprite* _sprite;
    cocos2d::Vec2 _coordinate;
    
public:
    static Item* create(const cocos2d::Vec2& coordinate, TileType type);
    
    Item(const cocos2d::Vec2& coordinate, TileType type);
    cocos2d::Vec2 getCoordinate() const;
    Level* getLevel() const;
    void setLevel(Level* level);
};

#endif
