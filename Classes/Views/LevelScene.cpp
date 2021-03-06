#include <cocos2d.h>
#include <limits>

#include "LevelScene.h"
#include "LevelScene/SmartControlLayer.h"
#include "LevelScene/KeyboardControlLayer.h"
#include "LevelScene/ButtonControlLayer.h"
#include "PacksScene.h"

size_t LevelScene::__tilesetIndex = 0;

LevelScene* LevelScene::create(ChipsChallengeGame* game, size_t packIndex, size_t levelIndex)
{
    auto instance = new (std::nothrow) LevelScene(game, packIndex, levelIndex);
    instance->autorelease();
    return instance;
}

LevelScene::LevelScene(ChipsChallengeGame* game, size_t packIndex, size_t levelIndex):
    _game(game),
    _packIndex(packIndex),
    _levelIndex(levelIndex),
    _paused(false),
    _lastUnpauseTime(0.0),
    _elapsedTimeBeforeLastPause(0.0)
{
    auto director = cocos2d::Director::getInstance();
    auto winSize = director->getWinSize();
    cocos2d::Rect screenBounds(director->getVisibleOrigin(), director->getVisibleSize());

    _game->retain();

    _stage = cocos2d::Node::create();
    addChild(_stage);

    _level = Level::create(_stage, this);
    _level->retain();

    auto buttonControlLayer = ButtonControlLayer::create(_level);
    addChild(buttonControlLayer);
    _controlLayers.push_back(buttonControlLayer);
    
    auto keyboardControlLayer = KeyboardControlLayer::create(_level);
    addChild(keyboardControlLayer);
    _controlLayers.push_back(keyboardControlLayer);
    
    auto uiLayer = cocos2d::Node::create();
    addChild(uiLayer);
    
    _inventoryPanel = InventoryPanel::create(_level->getInventory());
    _inventoryPanel->setAnchorPoint(cocos2d::Vec2(0.5f, 0));
    _inventoryPanel->setPosition(screenBounds.origin + cocos2d::Vec2(0.5f * screenBounds.size.width, 8));
    uiLayer->addChild(_inventoryPanel);
    
    auto menuButton = cocos2d::ui::Button::create("ui-button-pause.png", "ui-button-pause-on.png", "", cocos2d::ui::Widget::TextureResType::PLIST);
    menuButton->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
    menuButton->setPosition(screenBounds.origin + screenBounds.size + cocos2d::Vec2(-31, -32));
    menuButton->addTouchEventListener([this](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
    {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
        {
            pauseLevel();
            _topLayer->addChild(PauseMenuWindow::create(this));
        }
    });
    uiLayer->addChild(menuButton);
    
    auto restartButton = cocos2d::ui::Button::create("ui-button-restart.png", "ui-button-restart-on.png", "", cocos2d::ui::Widget::TextureResType::PLIST);
    restartButton->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
    restartButton->setPosition(screenBounds.origin + screenBounds.size + cocos2d::Vec2(-85, -32));
    restartButton->addTouchEventListener([this](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
    {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
        {
            restartLevel();
        }
    });
    uiLayer->addChild(restartButton);

    _preloaderLayer = cocos2d::LayerColor::create(cocos2d::Color4B(128, 192, 255, 255), winSize.width, winSize.height);
    _preloaderLayer->setAnchorPoint(cocos2d::Vec2::ZERO);
    _preloaderLayer->setPosition(cocos2d::Vec2::ZERO);
    addChild(_preloaderLayer);
    
    _fadeLayer = cocos2d::LayerColor::create(cocos2d::Color4B::WHITE, winSize.width, winSize.height);
    _fadeLayer->setAnchorPoint(cocos2d::Vec2::ZERO);
    _fadeLayer->setPosition(cocos2d::Vec2::ZERO);
    _fadeLayer->setVisible(false);
    addChild(_fadeLayer);

    _topLayer = cocos2d::Node::create();
    addChild(_topLayer);
    
    _level->makeTurn(0);
    director->getScheduler()->schedule(CC_SCHEDULE_SELECTOR(LevelScene::updatePerTurn), this, _level->getTurnDuration(), CC_REPEAT_FOREVER, 0, false);
    director->getScheduler()->schedule(CC_SCHEDULE_SELECTOR(LevelScene::update), this, 0, CC_REPEAT_FOREVER, 0, false);
}

LevelScene::~LevelScene()
{
    auto director = cocos2d::Director::getInstance();
    director->getScheduler()->unscheduleAllForTarget(this);
    
    _game->release();
    _level->release();
}

void LevelScene::onEnter()
{
    cocos2d::Scene::onEnter();
    gotoLevel(_levelIndex);
}

void LevelScene::update(float dt)
{
    cocos2d::Scene::update(dt);
    
    auto playerCreature = _level->getPlayerCreature();
    if (playerCreature != nullptr)
    {
        auto playerBounds = playerCreature->getSprite()->getBoundingBox();
        auto playerCenter = playerBounds.origin + playerBounds.size * 0.5f;

        auto playerPosition = playerCreature->getSprite()->getPosition();
        auto playerLastPosition = _level->getProjector()->coordinateToPoint(playerCreature->getCoordinate() - toVec2(playerCreature->getDirection()));
        auto playerNextPosition = _level->getProjector()->coordinateToPoint(playerCreature->getCoordinate());
        auto t = (playerPosition.x - playerLastPosition.x) / (playerNextPosition.x - playerLastPosition.x);
        auto stabilization = cocos2d::Vec2(0, playerPosition.y - (playerNextPosition.y * t + playerLastPosition.y * (1.0f - t)));
        
        auto winSize = cocos2d::Director::getInstance()->getWinSize();
        _stage->setPosition(-playerCenter + winSize * 0.5f + stabilization);
    }
}

void LevelScene::updatePerTurn(float dt)
{
    for (auto controlLayer : _controlLayers)
    {
        controlLayer->onLevelTurn();
    }
    
    _level->makeTurn(dt);
}

void LevelScene::onLevelWin()
{
    double time = _elapsedTimeBeforeLastPause + (cocos2d::utils::gettime() - _lastUnpauseTime);
    
    auto& record = _game->getHighscores()->getRecord(_packIndex, _levelIndex);
    
    int levelBonus = static_cast<int>(500 * (_levelIndex + 1) * std::round(std::pow(0.8f, record.deathCount)));
    if (levelBonus < 500)
    {
        levelBonus = 500;
    }
    
    int timeBonus = 10 * (_level->getConfig()->getTime() - static_cast<int>(time / _level->getTimeMultiplier()));
    if (timeBonus < 0)
    {
        timeBonus = 0;
    }
    
    HighscoreRecord newRecord;
    newRecord.completed = true;
    newRecord.deathCount = 0;
    newRecord.minDeathCount = record.deathCount;
    newRecord.minTime = time;
    newRecord.maxScore = timeBonus + levelBonus;
    
    if (record.completed)
    {
        newRecord.minDeathCount = std::min(newRecord.minDeathCount, record.minDeathCount);
        newRecord.minTime = std::min(newRecord.minTime, record.minTime);
        newRecord.maxScore = std::max(newRecord.maxScore, record.maxScore);
    }
    
    _game->getHighscores()->saveRecord(_packIndex, _levelIndex, newRecord);
}

void LevelScene::onLevelFail(const std::string& message)
{
    auto record = _game->getHighscores()->getRecord(_packIndex, _levelIndex);
    record.deathCount += 1;
    _game->getHighscores()->saveRecord(_packIndex, _levelIndex, record);
    
    cocos2d::Size winSize = cocos2d::Director::getInstance()->getWinSize();
    
    auto messageLabel = cocos2d::Label::createWithTTF(message, "fonts/Marker Felt.ttf", 24, cocos2d::Size(460, 0), cocos2d::TextHAlignment::CENTER);
    
    auto messagePanel = cocos2d::ui::Scale9Sprite::createWithSpriteFrameName("ui-panel-transparent.png");
    messagePanel->setContentSize(cocos2d::Size(512, messageLabel->getContentSize().height + 46));
    messagePanel->setPosition(cocos2d::Vec2(winSize) * 0.5f);
    messagePanel->setOpacity(0);
    messagePanel->runAction(cocos2d::Sequence::create(
        cocos2d::FadeIn::create(0.5f),
        cocos2d::DelayTime::create(1.0f),
        cocos2d::CallFuncN::create([](cocos2d::Node* messagePanel)
        {
             messagePanel->removeFromParentAndCleanup(true);
        }),
        nullptr
    ));
    _topLayer->addChild(messagePanel);
    
    messageLabel->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
    messageLabel->setPosition(cocos2d::Vec2(messagePanel->getContentSize()) * 0.5f);
    messageLabel->setOpacity(0);
    messageLabel->enableOutline(cocos2d::Color4B::BLACK, 1);
    messageLabel->runAction(cocos2d::FadeIn::create(0.5f));
    messagePanel->addChild(messageLabel);
    
    _fadeLayer->stopAllActions();
    _fadeLayer->setVisible(true);
    _fadeLayer->setOpacity(0);
    _fadeLayer->runAction(cocos2d::Sequence::create(
        cocos2d::DelayTime::create(1.0f),
        cocos2d::FadeOut::create(0.5f),
        cocos2d::CallFunc::create([this]()
        {
            _level->restart();
            onLevelStart();
        }),
        nullptr
    ));
}

void LevelScene::showHint(const std::string &hint, const cocos2d::Vec2 &position)
{
    auto director = cocos2d::Director::getInstance();
    auto screenOrigin = director->getVisibleOrigin();
    auto screenSize = director->getVisibleSize();
    
    auto existingPanel = _topLayer->getChildByTag(HINT_TAG);
    if (existingPanel != nullptr) {
        _topLayer->removeChild(existingPanel);
    }
    
    auto hintLabel = cocos2d::Label::createWithTTF(hint, "fonts/Marker Felt.ttf", 24, cocos2d::Size(480, 0), cocos2d::TextHAlignment::CENTER);
    hintLabel->enableOutline(cocos2d::Color4B::BLACK, 1);
    hintLabel->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
    hintLabel->setOpacity(0);
    
    auto hintPanel = cocos2d::ui::Scale9Sprite::createWithSpriteFrameName("ui-panel-transparent.png");
    hintPanel->setContentSize(cocos2d::Size(520, hintLabel->getContentSize().height + 40.0f));
    hintPanel->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
    hintPanel->setPosition(position);
    hintPanel->setScale(0.2f);
    hintPanel->setOpacity(0);
    
    hintLabel->setPosition(cocos2d::Vec2(hintPanel->getContentSize()) * 0.5f);
    hintPanel->addChild(hintLabel);
    
    float duration = _level->getTurnDuration();
    hintPanel->runAction(cocos2d::Spawn::create(
        cocos2d::MoveTo::create(duration, cocos2d::Vec2(screenOrigin.x + 0.5f * screenSize.width, screenOrigin.y + 0.3f * screenSize.height)),
        cocos2d::ScaleTo::create(duration, 1.0f),
        cocos2d::FadeTo::create(duration, 255),
        nullptr
    ));
    
    hintLabel->runAction(cocos2d::FadeTo::create(duration, 255));
    
    hintPanel->setTag(HINT_TAG);
    _topLayer->addChild(hintPanel);
}

void LevelScene::hideHint(const cocos2d::Vec2 &position)
{
    auto hintPanel = _topLayer->getChildByTag(HINT_TAG);
    if (hintPanel != nullptr) {
        float duration = _level->getTurnDuration();
        hintPanel->runAction(cocos2d::Sequence::create(
            cocos2d::Spawn::create(
                cocos2d::MoveTo::create(duration, position),
                cocos2d::ScaleTo::create(duration, 0.2f),
                cocos2d::FadeTo::create(duration, 0),
                nullptr
            ),
            cocos2d::RemoveSelf::create(),
            nullptr
        ));
        
        if (!hintPanel->getChildren().empty()) {
            auto hintLabel = hintPanel->getChildren().front();
            hintLabel->runAction(cocos2d::FadeTo::create(duration, 0));
        }
    }
}

void LevelScene::pauseLevel()
{
    if (!_paused)
    {
        _paused = true;
        
        auto director = cocos2d::Director::getInstance();
        director->getScheduler()->pauseTarget(this);
        
        pauseRecursive(this);
        
        _elapsedTimeBeforeLastPause += cocos2d::utils::gettime() - _lastUnpauseTime;
    }
}

void LevelScene::resumeLevel()
{
    if (_paused)
    {
        _paused = false;
        
        auto director = cocos2d::Director::getInstance();
        director->getScheduler()->resumeTarget(this);
        
        resumeRecursive(this);
        
        _lastUnpauseTime = cocos2d::utils::gettime();
    }
}

void LevelScene::restartLevel()
{
    if (_paused)
    {
        resumeLevel();
    }
    
    _level->restart();
    onLevelStart();
}

void LevelScene::gotoLevel(size_t levelIndex)
{
    if (_paused)
    {
        resumeLevel();
    }
    
    _levelIndex = levelIndex;
    
    _preloaderLayer->setVisible(true);
    cocos2d::Director::getInstance()->drawScene();
    
    size_t tilesetIndex = (_levelIndex % 2) + 1;
    if (tilesetIndex != __tilesetIndex)
    {
        if (__tilesetIndex != 0)
        {
            std::string oldSheetFile = cocos2d::StringUtils::format("sheets/walls-%02ld.plist", __tilesetIndex);
            cocos2d::SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(oldSheetFile);
            
            std::string oldTextureFile = cocos2d::StringUtils::format("sheets/walls-%02ld.png", __tilesetIndex);
            cocos2d::Director::getInstance()->getTextureCache()->removeTextureForKey(oldTextureFile);
        }
        
        __tilesetIndex = tilesetIndex;
        
        std::string sheetFile = cocos2d::StringUtils::format("sheets/walls-%02ld.plist", __tilesetIndex);
        cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile(sheetFile);
    }
    
    try
    {
        auto levelConfig = _game->getLevelPack(_packIndex)->readLevelConfig(_levelIndex);
        _level->start(levelConfig);
    }
    catch (...)
    {
        // show error
    }
    
    auto record = _game->getHighscores()->getRecord(_packIndex, _levelIndex);
    if (!record.tried)
    {
        record.tried = true;
        _game->getHighscores()->saveRecord(_packIndex, _levelIndex, record);
    }
    
    _preloaderLayer->setVisible(false);
    onLevelStart();
}

void LevelScene::onLevelStart()
{
    cocos2d::Size winSize = cocos2d::Director::getInstance()->getWinSize();
    
    _fadeLayer->stopAllActions();
    _fadeLayer->setVisible(true);
    _fadeLayer->setOpacity(255);
    _fadeLayer->runAction(cocos2d::Sequence::create(
        cocos2d::FadeOut::create(0.5f),
        cocos2d::CallFuncN::create([](cocos2d::Node* fadeLayer)
        {
            fadeLayer->setVisible(false);
        }),
        nullptr
    ));

    auto titleLabel = cocos2d::Label::createWithTTF(_level->getConfig()->getTitle(), "fonts/Marker Felt.ttf", 24, cocos2d::Size(314, 0), cocos2d::TextHAlignment::CENTER);
    titleLabel->enableOutline(cocos2d::Color4B::BLACK, 1);

    auto titlePanel = cocos2d::ui::Scale9Sprite::createWithSpriteFrameName("ui-panel-transparent.png");
    titlePanel->setContentSize(cocos2d::Size(360, titleLabel->getContentSize().height + 46));
    titlePanel->setPosition(cocos2d::Vec2(winSize) * 0.5f);
    
    titleLabel->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
    titleLabel->setPosition(cocos2d::Vec2(titlePanel->getContentSize()) * 0.5f);
    titlePanel->addChild(titleLabel);
    
    _topLayer->addChild(titlePanel);
    
    titleLabel->runAction(cocos2d::Sequence::create(
        cocos2d::DelayTime::create(0.7f),
        cocos2d::FadeOut::create(0.5f),
        nullptr
    ));

    titlePanel->runAction(cocos2d::Sequence::create(
        cocos2d::DelayTime::create(0.7f),
        cocos2d::FadeOut::create(0.5f),
        cocos2d::CallFuncN::create([](cocos2d::Node* titlePanel)
        {
            titlePanel->removeFromParentAndCleanup(true);
        }),
        nullptr
    ));
    
    _elapsedTimeBeforeLastPause = 0.0;
    _lastUnpauseTime = cocos2d::utils::gettime();
}

void LevelScene::pauseRecursive(cocos2d::Node* node)
{
    node->pause();
    for (auto child : node->getChildren())
    {
        pauseRecursive(child);
    }
}

void LevelScene::resumeRecursive(cocos2d::Node* node)
{
    node->resume();
    for (auto child : node->getChildren())
    {
        resumeRecursive(child);
    }
}
