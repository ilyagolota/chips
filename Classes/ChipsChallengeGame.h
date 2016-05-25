#ifndef _CHIPS_CHALLENGE_GAME_H_
#define _CHIPS_CHALLENGE_GAME_H_

#include <cocos2d.h>
#include <LevelData/LevelBundle.h>

class ChipsChallengeGame : public cocos2d::Ref
{
private:
    LevelBundle* _levelBundle;
    
public:
    static ChipsChallengeGame* create();
    
    ChipsChallengeGame();
    virtual ~ChipsChallengeGame();
    
    LevelBundle* getLevelBundle();
};

#endif
