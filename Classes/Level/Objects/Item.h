#ifndef _CHIPS_CHALLENGE_ITEM_H_
#define _CHIPS_CHALLENGE_ITEM_H_

#include <cocos2d.h>
#include "LevelObject.h"
#include <Level/TileType.h>

class Level;

class Item : public LevelObject
{
public:
    static Item* create(Level* level, const cocos2d::Vec2& coordinate, TileType type);
    
    Item(Level* level, const cocos2d::Vec2& coordinate, TileType type);
    
    void onAdd() override;
    void reset() override;
    
    bool isEnterableBy(const Creature *creature, Direction direction) const override;
    void afterEnter(Creature *creature) override;

private:
    std::string& getSpriteFrameName() const;
    
    TileType _type;
    bool _picked;
    cocos2d::Sprite* _node;
    cocos2d::Sprite* _highlightNode;
};

#endif
