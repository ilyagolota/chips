#ifndef _CHIPS_CHALLENGE_LEVEL_OBJECT_H_
#define _CHIPS_CHALLENGE_LEVEL_OBJECT_H_

#include <cocos2d.h>
#include "Tiled/Direction.h"

class Level;
class Creature;

class LevelObject : public cocos2d::Ref
{
protected:
    Level* _level;
    cocos2d::Vec2 _coordinate;
    cocos2d::Vector<cocos2d::Node*> _nodes;
    
public:
    LevelObject(const cocos2d::Vec2& coordinate);
    
    cocos2d::Vec2 getCoordinate() const;
    Level* getLevel() const;
    void setLevel(Level* level);

    virtual bool isOpenableBy(const Creature* creature, Direction direction) const;
    virtual bool isEnterableBy(const Creature* creature, Direction direction) const;
    virtual bool isEscapableBy(const Creature* creature, Direction direction) const;
    
    virtual void beforeEnter(Creature* creature);
    virtual void afterEnter(Creature* creature);
    virtual void beforeLeave(Creature* creature);
    virtual void afterLeave(Creature* creature);
    virtual void reset();

protected:
    virtual void buildNodes();
    virtual void destroyNodes();
    
};

#endif
