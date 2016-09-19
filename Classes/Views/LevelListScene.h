#ifndef  _CHIPS_CHALLENGE_LEVELS_SCENE_H_
#define  _CHIPS_CHALLENGE_LEVELS_SCENE_H_

#include <cocos2d.h>
#include <ChipsChallengeGame.h>

class LevelListScene : public cocos2d::Scene
{
protected:
    int _packIndex;
    int _startLevelIndex;
    
public:
    static LevelListScene* create(ChipsChallengeGame* game, int packIndex, int startLevelIndex);
    LevelListScene(ChipsChallengeGame* game, int packIndex, int startLevelIndex);
    ~LevelListScene();
    virtual void onEnter() override;
    virtual void onExit() override;
    
private:
    void _build();
    
    ChipsChallengeGame* _game;
};

#endif
