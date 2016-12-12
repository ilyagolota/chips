#ifndef _CHIPS_CHALLENGE_LEVEL_OBJECT_H_
#define _CHIPS_CHALLENGE_LEVEL_OBJECT_H_

#include <cocos2d.h>
#include <Tiled/Direction.h>
#include <Tiled/TiledPhysicsWorld.h>
#include <Tiled/TiledSoundEnvironment.h>

class Level;
class Creature;

class LevelObject : public cocos2d::Ref
{
public:
    LevelObject(Level* level, const cocos2d::Vec2& coordinate);
    
    cocos2d::Vec2 getCoordinate() const;
    Level* getLevel() const;

    virtual void onAdd();
    virtual void reset();
    
    virtual bool isOpenableBy(const Creature* creature, Direction direction) const;
    virtual bool isEnterableBy(const Creature* creature, Direction direction) const;
    virtual bool isEscapableBy(const Creature* creature, Direction direction) const;
    
    virtual void beforeEnter(Creature* creature);
    virtual void afterEnter(Creature* creature);
    virtual void beforeEscape(Creature* creature);
    virtual void afterEscape(Creature* creature);
    
protected:
	Level* _level;
	cocos2d::Vec2 _coordinate;
    TileSoundEmitter* _soundEmitter;
};

#endif
