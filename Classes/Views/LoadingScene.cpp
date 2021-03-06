#include "LoadingScene.h"
#include <Preloader/Preloader.h>
#include "PacksScene.h"
#include "ChipsChallengeGame.h"

using namespace cocos2d;

LoadingScene* LoadingScene::create(ChipsChallengeGame* game, Preloader *preloader)
{
    auto instance = new LoadingScene(game, preloader);
    instance->autorelease();
    return instance;
}

LoadingScene::LoadingScene(ChipsChallengeGame* game, Preloader *preloader)
{
    _game = game;
    _game->retain();
    
    _preloader = preloader;
    _preloader->retain();
    
    _build();
}

LoadingScene::~LoadingScene()
{
    _game->release();
    _preloader->release();
}

void LoadingScene::_build()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    //auto background = Sprite::create("background.png");
    //background->setAnchorPoint(Vec2::ZERO);
    //background->setPosition(Vec2::ZERO);
    //addChild(background);
    
    _progressBar = ProgressTimer::create(Sprite::create("HelloWorld.png"));
    _progressBar->setAnchorPoint(Vec2(0, 0));
    _progressBar->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    _progressBar->setType(ProgressTimer::Type::BAR);
    _progressBar->setPercentage(0);
    addChild(_progressBar);
    
    cocos2d::Director::getInstance()->getScheduler()->schedule(CC_SCHEDULE_SELECTOR(LoadingScene::_update), this, 0, CC_REPEAT_FOREVER, 0, false);
}

void LoadingScene::_update(float dt)
{
    _preloader->update(dt);
    
    size_t loaded = _preloader->getLoaded();
    size_t total = _preloader->getTotal();
    _progressBar->setPercentage(100.0f * loaded / total);
    
    if (_preloader->isComplete())
    {
        cocos2d::Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
        cocos2d::Director::getInstance()->pushScene(PacksScene::create(_game));
    }
}
