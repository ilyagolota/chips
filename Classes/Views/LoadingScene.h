#ifndef  _CHIPS_CHALLENGE_LOADING_SCENE_H_
#define  _CHIPS_CHALLENGE_LOADING_SCENE_H_

#include <cocos2d.h>

class Preloader;

class LoadingScene : public cocos2d::Scene
{
private:
    static LoadingScene* create(Preloader *preloader);
    
    cocos2d::ProgressTimer* _progressBar;
    Preloader* _preloader;
    
public:
    LoadingScene(Preloader *preloader);
    virtual ~LoadingScene();
    
protected:
    void _build();
    void _update(float dt);
};

#endif
