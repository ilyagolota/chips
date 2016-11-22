#ifndef _CHIPS_CHALLENGE_GAME_H_
#define _CHIPS_CHALLENGE_GAME_H_

#include <cocos2d.h>
#include <Level/LevelPack.h>
#include <Common/HighscoreStorage.h>

class ChipsChallengeGame : public cocos2d::Ref
{
public:
    static ChipsChallengeGame* create();
    
    ChipsChallengeGame();
    virtual ~ChipsChallengeGame();
    
    cocos2d::Vector<LevelPack*>& getLevelPacks();
    LevelPack* getLevelPack(size_t i);
    
    HighscoreStorage* getHighscores();
    void setHighscores(HighscoreStorage* highscores);
    
private:
    cocos2d::Vector<LevelPack*> _levelPacks;
    HighscoreStorage* _highscores;
};

#endif
