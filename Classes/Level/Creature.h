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
    const int SLIDE_FLAG = 0x01;
    const int RELEASE_FLAG = 0x02;
    const int MOVE_ACTION_TAG = 2005;
    const int ANIMATE_ACTION_TAG = 2006;
    
    static Creature* create(CreatureType type);
    
    Creature(CreatureType type);
    ~Creature();
    void move();
    bool isMoving() const;
    int getTurnsToNextMove() const;
    int getTurnsPerMove() const;
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
    bool canMove() const;
    
private:
    void _tryMoveNext();
    void _move();
	void _updateAnimation(bool wasModing, Direction wasDirection);
};

#endif
