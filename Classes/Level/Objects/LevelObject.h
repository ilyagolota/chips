#ifndef _CHIPS_CHALLENGE_LEVEL_OBJECT_H_
#define _CHIPS_CHALLENGE_LEVEL_OBJECT_H_

#include <cocos2d.h>
#include <Tiled/Direction.h>
#include <Tiled/TiledPhysicsWorld.h>

class Level;
class Creature;

class LevelObject : public cocos2d::Ref
{
protected:
    Level* _level;
    cocos2d::Vec2 _coordinate;
    
private:
    cocos2d::Node* _mainNode;
    cocos2d::Vector<cocos2d::Node*> _additionalNodes;
    TileBody _body;
    
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
    void addNode(cocos2d::Node* node);
    void removeNode(cocos2d::Node* node);
    void setBody(TileBody body, int layerMask);
};

#endif
