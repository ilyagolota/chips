#ifndef _CHIPS_CHALLENGE_WATER_H_
#define _CHIPS_CHALLENGE_WATER_H_

#include <cocos2d.h>
#include "LevelObject.h"

class Water : public LevelObject
{
private:
    cocos2d::Sprite* _waterSprite;
    
public:
    static Water* create(const cocos2d::Vec2& coordinate);
    
    Water(const cocos2d::Vec2& coordinate);
    virtual ~Water();
    
    bool hasDrawnBlock() const;
    
    void build() override;
    void destroy() override;
};

#endif
