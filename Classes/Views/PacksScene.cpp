#include "PacksScene.h"
#include <LevelData/LevelBundle.h>
#include "ChipsChallengeGame.h"
#include "LevelScene.h"

PacksScene* PacksScene::create(ChipsChallengeGame* game)
{
	auto instance = new (std::nothrow) PacksScene(game);
	instance->autorelease();
	return instance;
}

PacksScene::PacksScene(ChipsChallengeGame* game)
{
    _game = game;
    _game->retain();
}

PacksScene::~PacksScene()
{
    _game->release();
}

void PacksScene::onEnter()
{
	Scene::onEnter();
	_build();
	_animateEntering();
}

void PacksScene::onExitTransitionDidStart()
{
	Scene::onExitTransitionDidStart();
	_animateExiting();
}

void PacksScene::onExit()
{
	Scene::onExit();
}

void PacksScene::_build()
{
	cocos2d::Size winSize = cocos2d::Director::getInstance()->getWinSize();

    float buttonWidth = 200;
    
    _buttonsNode = cocos2d::Node::create();
    _buttonsNode->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
    _buttonsNode->setContentSize(cocos2d::Size(winSize.width - buttonWidth * _game->getLevelBundle()->getLevelPacks().size(), winSize.height));
    _buttonsNode->setPosition(cocos2d::Vec2(0.5f * winSize.width, 0.5f * winSize.height));
    addChild(_buttonsNode);
    
    float x = 0.5f * (_buttonsNode->getContentSize().width - buttonWidth * (_game->getLevelBundle()->getLevelPacks().size() - 1));
	for (auto pack : _game->getLevelBundle()->getLevelPacks())
	{
        auto button = cocos2d::ui::Button::create("CloseSelected.png");
        button->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
        button->setPosition(cocos2d::Vec2(x, 0.5f * _buttonsNode->getContentSize().height));
        button->addTouchEventListener([this, pack](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
        {
            if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
            {
                cocos2d::Director::getInstance()->pushScene(LevelScene::create(_game, 0, 0));
            }
        });
        x += buttonWidth;
        _buttonsNode->addChild(button);
	}
}

void PacksScene::_animateEntering()
{
    float duration = 0.4;

    //_background->setOpacity(0);
    //_background->runAction(cocos2d::FadeIn::create(duration));

    float delay = 0;
    float spawnDuration = 0.5f * duration;
    float delayStep = (duration - spawnDuration) / _buttonsNode->getChildren().size();
    for (auto button : _buttonsNode->getChildren())
    {
        auto position = button->getPosition();
        button->setPosition(position - cocos2d::Vec2(40, 0));
        button->setOpacity(0);
    
        auto moveAction = cocos2d::EaseOut::create(cocos2d::MoveTo::create(spawnDuration, position), 0.5f);
        auto fadeInAction = cocos2d::FadeIn::create(spawnDuration);
        auto scaleAction = cocos2d::ScaleTo::create(spawnDuration, 1);
        auto spawnAction = cocos2d::Spawn::create(moveAction, fadeInAction, scaleAction, nullptr);

        if (delay == 0)
        {
            button->runAction(spawnAction);
        }
        else
        {
            auto delayAction = cocos2d::DelayTime::create(delay);
            button->runAction(cocos2d::Sequence::createWithTwoActions(delayAction, spawnAction));
        }
        delay += delayStep;
    }
    
    _buttonsNode->setScale(0.7);
    _buttonsNode->runAction(cocos2d::ScaleTo::create(duration, 1));
}

void PacksScene::_animateExiting()
{

}
