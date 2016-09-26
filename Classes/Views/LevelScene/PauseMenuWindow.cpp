#include "PauseMenuWindow.h"
#include <Views/LevelScene.h>

PauseMenuWindow* PauseMenuWindow::create(LevelScene* levelScene)
{
    auto instance = new PauseMenuWindow(levelScene);
    instance->autorelease();
    return instance;
}

PauseMenuWindow::PauseMenuWindow(LevelScene* levelScene)
{
    _levelScene = levelScene;
    
    cocos2d::Size winSize = cocos2d::Director::getInstance()->getWinSize();
    setContentSize(winSize);
    
    auto panel = cocos2d::ui::Scale9Sprite::createWithSpriteFrameName("slide-north-0001.png");
    panel->setContentSize(cocos2d::Size(320, 400));
    panel->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
    panel->setPosition(cocos2d::Vec2(winSize) * 0.5);
    addChild(panel);
    
    auto resumeButton = cocos2d::ui::Button::create("ui-button-default.png", "", "", cocos2d::ui::Widget::TextureResType::PLIST);
    resumeButton->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
    resumeButton->setPosition(cocos2d::Vec2(0.5f * panel->getContentSize().width, panel->getContentSize().height - 50));
    resumeButton->addTouchEventListener([this](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
    {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
        {
            removeFromParentAndCleanup(true);
            _levelScene->resumeLevel();
        }
    });
    panel->addChild(resumeButton);
    
    auto resumeButtonLabel = cocos2d::Label::createWithTTF("Continue", "fonts/Marker Felt.ttf", 24);
    resumeButtonLabel->setPosition(cocos2d::Vec2(resumeButton->getContentSize()) * 0.5f);
    resumeButtonLabel->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
	resumeButtonLabel->enableOutline(cocos2d::Color4B(0xAD, 0x72, 0x14, 0xFF), 1);
    resumeButton->addChild(resumeButtonLabel);
    
    auto restartButton = cocos2d::ui::Button::create("ui-button-default.png", "", "", cocos2d::ui::Widget::TextureResType::PLIST);
    restartButton->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
    restartButton->setPosition(cocos2d::Vec2(0.5f * panel->getContentSize().width, panel->getContentSize().height - 150));
    restartButton->addTouchEventListener([this](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
    {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
        {
            removeFromParentAndCleanup(true);
            _levelScene->restartLevel();
        }
    });
    panel->addChild(restartButton);
    
    auto restartButtonLabel = cocos2d::Label::createWithTTF("Restart", "fonts/Marker Felt.ttf", 24);
    restartButtonLabel->setPosition(cocos2d::Vec2(restartButton->getContentSize()) * 0.5f);
    restartButtonLabel->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
	restartButtonLabel->enableOutline(cocos2d::Color4B(0xAD, 0x72, 0x14, 0xFF), 1);
    restartButton->addChild(restartButtonLabel);
    
    auto exitButton = cocos2d::ui::Button::create("ball-stay-east.png", "", "", cocos2d::ui::Widget::TextureResType::PLIST);
    exitButton->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
    exitButton->setPosition(cocos2d::Vec2(0.5f * panel->getContentSize().width, panel->getContentSize().height - 250));
    exitButton->addTouchEventListener([this](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
    {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
        {
            cocos2d::Director::getInstance()->popScene();
        }
    });
    panel->addChild(exitButton);
    
    auto exitButtonLabel = cocos2d::Label::createWithTTF("Exit", "fonts/Marker Felt.ttf", 24);
    exitButtonLabel->setPosition(cocos2d::Vec2(exitButton->getContentSize()) * 0.5f);
    exitButtonLabel->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
    exitButtonLabel->enableOutline(cocos2d::Color4B::BLACK, 2);
    exitButton->addChild(exitButtonLabel);
}
