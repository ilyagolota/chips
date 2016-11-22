#include "LevelListScene.h"
#include "LevelScene.h"

LevelListScene* LevelListScene::create(ChipsChallengeGame* game, int packIndex, int startLevelIndex)
{
    auto instance = new (std::nothrow) LevelListScene(game, packIndex, startLevelIndex);
    instance->autorelease();
    return instance;
}

LevelListScene::LevelListScene(ChipsChallengeGame* game, int packIndex, int startLevelIndex)
{
    _game = game;
    _packIndex = packIndex;
    _startLevelIndex = startLevelIndex;
}

LevelListScene::~LevelListScene()
{ }

void LevelListScene::onEnter()
{
    cocos2d::Scene::onEnter();
    _build();
}

void LevelListScene::onExit()
{
    cocos2d::Scene::onExit();
    removeAllChildren();
    cocos2d::Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

void LevelListScene::_build()
{
    cocos2d::Vec2 visibleOrigin = cocos2d::Director::getInstance()->getVisibleOrigin();
    cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    
    //auto background = cocos2d::Sprite::create("background.png");
    //background->setAnchorPoint(cocos2d::Vec2::ZERO);
    //background->setPosition(cocos2d::Vec2::ZERO);
    //background->setOpacity(0);
    //addChild(background);
    //background->runAction(cocos2d::FadeIn::create(0.5));
    
    auto levelPack = _game->getLevelPack(_packIndex);
    const std::vector<LevelPackEntry>& entries = levelPack->getEntries();
    
    cocos2d::Rect menuRect(visibleOrigin.x + 100, visibleOrigin.y + 100, visibleSize.width - 200, visibleSize.height - 140);
    cocos2d::Size cellSize(menuRect.size.width / 5, menuRect.size.height / 5);
    cocos2d::Vec2 point(0, menuRect.size.height - cellSize.height);
    
    bool prevLevelCompleted = true;
    for (size_t i = 0; i + _startLevelIndex < entries.size() && i < 25; i++)
    {
        int levelIndex =_startLevelIndex + i;
        
        auto& record = _game->getHighscores()->getRecord(_packIndex, levelIndex);
        bool canStart = prevLevelCompleted || record.tried;
        
        auto button = cocos2d::ui::Button::create(canStart ? "slide-north-0001.png" : "floor.png", "", "", cocos2d::ui::Widget::TextureResType::PLIST);
        button->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
        button->setPosition(menuRect.origin + point + cocos2d::Vec2(cellSize) * 0.5f - cocos2d::Vec2(0, 100));
        button->setScale(0);
        button->setEnabled(canStart);
        button->addTouchEventListener([this, levelIndex](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
        {
            if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
            {
                auto director = cocos2d::Director::getInstance();
                director->pushScene(LevelScene::create(_game, _packIndex, _startLevelIndex + levelIndex));
            }
        });
        
        if (canStart)
        {
            auto buttonLabel = cocos2d::Label::createWithTTF(std::to_string(i + 1), "fonts/Marker Felt.ttf", 24);
            buttonLabel->setPosition(cocos2d::Vec2(button->getContentSize()) * 0.5f);
            buttonLabel->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
            buttonLabel->enableOutline(cocos2d::Color4B::BLACK, 2);
            button->addChild(buttonLabel);
        }
        
        addChild(button);
        
        if (record.completed)
        {
            auto star = cocos2d::Sprite::createWithSpriteFrameName("ic-chip.png");
            star->setPosition(cocos2d::Vec2(button->getContentSize().width - 10, 11));
            button->addChild(star);
        }
        
        if (i % 5 != 4)
        {
            point.x += cellSize.width;
        }
        else
        {
            point.x = 0;
            point.y -= cellSize.height;
        }

        button->runAction(cocos2d::Sequence::create(
            cocos2d::DelayTime::create(0.01 * i),
            cocos2d::Spawn::create(
                cocos2d::EaseBounceIn::create(cocos2d::ScaleTo::create(0.25, 1)),
                cocos2d::MoveBy::create(0.25, cocos2d::Vec2(0, 100)),
                nullptr
            ),
            nullptr
        ));
        
        prevLevelCompleted = record.completed;
    }
    
    /*auto backButton = cocos2d::MenuItemSprite::create(cocos2d::Sprite::create("boots-ice.png"), nullptr, CC_CALLBACK_1(LevelsScene::_backCallback, this));
    backButton->setPosition(cocos2d::Vec2(100, 60));
    menu->addChild(backButton);*/
}
