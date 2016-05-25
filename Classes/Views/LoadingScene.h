#ifndef  _CHIPS_CHALLENGE_LOADING_SCENE_H_
#define  _CHIPS_CHALLENGE_LOADING_SCENE_H_

#include <cocos2d.h>

class Preloader;
class ChipsChallengeGame;

class LoadingScene : public cocos2d::Scene
{
private:
    ChipsChallengeGame* _game;
    cocos2d::ProgressTimer* _progressBar;
    Preloader* _preloader;
    
public:
    static LoadingScene* create(ChipsChallengeGame* game, Preloader *preloader);
    
    LoadingScene(ChipsChallengeGame* game, Preloader *preloader);
    virtual ~LoadingScene();
    
protected:
    void _build();
    void _update(float dt);
};

#endif
