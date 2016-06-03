#ifndef _CHIPS_CHALLENGE_CREATURE_H_
#define _CHIPS_CHALLENGE_CREATURE_H_

#include <cocos2d.h>
#include "Tiled/Direction.h"

class Level;
class TileSoundEmitter;

enum class CreatureType
{
    BLOCK,
    BUG,
    FIREBALL,
    BALL,
    TANK,
    GLIDER,
    TEETH,
    WALKER,
    BLOB,
    PARAMECIUM,
    CHIP,
};

namespace std
{
	string &to_string(CreatureType type);
}

class Creature : public cocos2d::Ref
{
protected:
    Level* _level;
    cocos2d::Vec2 _coordinate;
    Direction _direction;
    CreatureType _type;
    int _turnsToNextMove;
    cocos2d::Sprite* _sprite;
    TileSoundEmitter* _soundEmitter;
    int _animationState;
    
public:
    static const int MOVE_ACTION_TAG = 2005;
    static const int ANIMATE_ACTION_TAG = 2006;
    
    static Creature* create(CreatureType type);
    
    Creature(CreatureType type);
    ~Creature();
    void move(Direction direction);
    bool isMoving() const;
    int getTurnsToNextMove() const;
    int getTurnsPerMove() const;
    int getZOrderDelta() const;
    int getPhysicsLayerMask() const;
    cocos2d::Sprite *getSprite() const;
    CreatureType getType() const;
    cocos2d::Vec2 getCoordinate() const;
    void setCoordinate(const cocos2d::Vec2& coordinate);
    Direction getDirection() const;
    void setDirection(Direction value);
    Level* getLevel() const;
    void setLevel(Level* level);
    void onTurn(float dt);
    void update(float dt);
    void touch(Direction direction);
    bool canMove(Direction direction) const;
    
private:
    void _tryMoveNext();
    void _move(Direction direction);
    void _updatePosition();
	void _updateAnimation(bool wasModing, Direction wasDirection);
};

#endif
