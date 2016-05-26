#ifndef _CHIPS_CHALLENGE_BEARTRAP_H_
#define _CHIPS_CHALLENGE_BEARTRAP_H_

#include <cocos2d.h>
#include "LevelObject.h"

class Beartrap : public LevelObject
{
private:
    bool _open;
    cocos2d::Sprite* _sprite;
    
public:
    static Beartrap* create(const cocos2d::Vec2& coordinate);
    
    Beartrap(const cocos2d::Vec2& coordinate);
    
    bool isOpen() const;
    void setOpen(bool open);
    
    bool isEscapableBy(const Creature* creature, Direction direction) const override;
    
    void afterEnter(Creature* creature) override;
    void reset() override;
    
protected:
    void build() override;
    void destroy() override;

};

#endif
