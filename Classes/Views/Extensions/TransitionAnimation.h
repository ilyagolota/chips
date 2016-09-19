#ifndef  _CHIPS_CHALLENGE_TRANSITION_ANIMATION_H_
#define  _CHIPS_CHALLENGE_TRANSITION_ANIMATION_H_

#include <cocos2d.h>

class TransitionAnimation : public cocos2d::TransitionScene
{
public:
    static TransitionAnimation* create(float duration, cocos2d::Scene* scene);
    virtual ~TransitionAnimation();
    virtual void onEnter() override;
    virtual void onExit() override;
    
protected:
    TransitionAnimation();
};

#endif
