#include "TransitionAnimation.h"

TransitionAnimation* TransitionAnimation::create(float duration, cocos2d::Scene* scene)
{
    auto instance = new TransitionAnimation();
    instance->initWithDuration(duration, scene);
    instance->autorelease();
    return instance;
}

TransitionAnimation::~TransitionAnimation()
{ }

void TransitionAnimation::onEnter()
{
    TransitionScene::onEnter();
    
    _inScene->setVisible(true);
    
    auto delay = cocos2d::DelayTime::create(_duration);
    auto finish = cocos2d::CallFunc::create(CC_CALLBACK_0(TransitionScene::finish, this));
    _inScene->runAction(cocos2d::Sequence::createWithTwoActions(delay, finish));
}

void TransitionAnimation::onExit()
{
    TransitionScene::onExit();
}

TransitionAnimation::TransitionAnimation()
{ }
