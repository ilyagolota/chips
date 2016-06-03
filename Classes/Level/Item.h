#ifndef _CHIPS_CHALLENGE_ITEM_H_
#define _CHIPS_CHALLENGE_ITEM_H_

#include <cocos2d.h>
#include "LevelData/TileType.h"

class Level;
class Creature;

class Item : public cocos2d::Ref
{
private:
	Level* _level;
    TileType _type;
    cocos2d::Sprite* _sprite;
	cocos2d::Sprite* _highlight;
    cocos2d::Vec2 _coordinate;
    bool _picked;
    
public:
    static Item* create(const cocos2d::Vec2& coordinate, TileType type);
    
    Item(const cocos2d::Vec2& coordinate, TileType type);
	~Item();
    cocos2d::Vec2 getCoordinate() const;
    Level* getLevel() const;
    void setLevel(Level* level);
    bool isPickableBy(const Creature* creature) const;
    void pick(Creature* creature);
    
protected:
    std::string& _getSpriteFrameName() const;
};

#endif
