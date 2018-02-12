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

enum class CreatureState
{
    NORMAL,
    SLIDING,
    SWIMMING,
};

namespace std
{
    string &to_string(CreatureType type);
}

class Creature : public cocos2d::Ref
{
public:
    static const int MOVE_ACTION_TAG = 2005;
    static const int ANIMATE_ACTION_TAG = 2006;
    static const int CHANGE_STATE_ACTION_TAG = 2007;
    
    static Creature* create(Level* level, CreatureType type);
    
    Creature(Level* level, CreatureType type);
    ~Creature();
    void queueMove(Direction direction);
    bool hasQueuedMove();
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
    CreatureState getState() const;
    void setState(CreatureState state);
    Level* getLevel() const;
    void die();
    bool isDead();
    void onTurn(float dt);
    void onAdd();
    void onRemove();
    void update(float dt);
    bool canMove(Direction direction) const;
    void updateAnimation();
    
private:
    void chooseNextMove();
    void move(Direction direction);
    void updatePosition();
    void applyAnimationParams();
    void updateAnimationInternal();
    void updateFlip();

    Level* _level;
    cocos2d::Vec2 _coordinate;
    Direction _direction;
    CreatureState _state;
    int _turnsToNextMove;
    bool _queuedMove;
    bool _dead;
    Direction _queuedDirection;
    CreatureType _type;
    cocos2d::Sprite* _sprite;
    TileSoundEmitter* _soundEmitter;
    Direction _animatedDirection;
    CreatureState _animatedState;
    bool _animatedMoving;
    bool _animatedFlipped;
};

#endif
