#ifndef _CHIPS_CHALLENGE_GAME_H_
#define _CHIPS_CHALLENGE_GAME_H_

#include <cocos2d.h>
#include <Level/LevelPack.h>

class ChipsChallengeGame : public cocos2d::Ref
{
public:
    static ChipsChallengeGame* create();
    
    ChipsChallengeGame();
    virtual ~ChipsChallengeGame();
    
    cocos2d::Vector<LevelPack*>& getLevelPacks();
    LevelPack* getLevelPack(size_t i);
    
private:
    cocos2d::Vector<LevelPack*> _levelPacks;
};

#endif
