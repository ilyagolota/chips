#include <cocos2d.h>

#include "LevelScene.h"
#include "PacksScene.h"

LevelScene* LevelScene::create(ChipsChallengeGame* game, size_t packIndex, size_t levelIndex)
{
	auto instance = new (std::nothrow) LevelScene(game, packIndex, levelIndex);
    instance->autorelease();
    return instance;
}

LevelScene::LevelScene(ChipsChallengeGame* game, size_t packIndex, size_t levelIndex)
{
    auto director = cocos2d::Director::getInstance();
	auto winSize = director->getWinSize();
    
    _game = game;
    _game->retain();
    
    _packIndex = packIndex;
    _levelIndex = levelIndex;
    
    _paused = false;

    _stage = cocos2d::Node::create();
    addChild(_stage);

	_level = Level::create(_stage, this);
	_level->retain();
    
    _controlLayer = SmartControlLayer::create(_level);
    addChild(_controlLayer);
    _level->setPlayerControl(_controlLayer);

    auto uiLayer = cocos2d::Node::create();
    addChild(uiLayer);
    
	_inventoryPanel = InventoryPanel::create(_level->getInventory());
	_inventoryPanel->setAnchorPoint(cocos2d::Vec2(0.5f, 0));
	_inventoryPanel->setPosition(cocos2d::Vec2(0.5f * winSize.width, 8));
	uiLayer->addChild(_inventoryPanel);
    
    auto menuButton = cocos2d::ui::Button::create("ui-inventory-cell.png", "", "", cocos2d::ui::Widget::TextureResType::PLIST);
    menuButton->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
    menuButton->setPosition(cocos2d::Vec2(winSize) + cocos2d::Vec2(-50, -50));
    menuButton->addTouchEventListener([this](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
    {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
        {
            pauseLevel();
            _topLayer->addChild(PauseMenuWindow::create(this));
        }
    });
    uiLayer->addChild(menuButton);
    
    auto restartButton = cocos2d::ui::Button::create("ui-inventory-cell.png", "", "", cocos2d::ui::Widget::TextureResType::PLIST);
    restartButton->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
    restartButton->setPosition(cocos2d::Vec2(winSize) + cocos2d::Vec2(-150, -50));
    restartButton->addTouchEventListener([this](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
    {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
        {
            restartLevel();
        }
    });
    uiLayer->addChild(restartButton);

    _topLayer = cocos2d::Node::create();
    addChild(_topLayer);
    
    _preloaderLayer = cocos2d::LayerColor::create(cocos2d::Color4B(128, 192, 255, 255), winSize.width, winSize.height);
    _preloaderLayer->setAnchorPoint(cocos2d::Vec2::ZERO);
    _preloaderLayer->setPosition(cocos2d::Vec2::ZERO);
    addChild(_preloaderLayer);
    
    _fadeLayer = cocos2d::LayerColor::create(cocos2d::Color4B::WHITE, winSize.width, winSize.height);
    _fadeLayer->setAnchorPoint(cocos2d::Vec2::ZERO);
    _fadeLayer->setPosition(cocos2d::Vec2::ZERO);
    _fadeLayer->setVisible(false);
    addChild(_fadeLayer);
    
    _level->makeTurn(0);
    director->getScheduler()->schedule(CC_SCHEDULE_SELECTOR(Level::makeTurn), _level, _level->getTurnDuration(), CC_REPEAT_FOREVER, 0, false);
    director->getScheduler()->schedule(CC_SCHEDULE_SELECTOR(LevelScene::update), this, 0, CC_REPEAT_FOREVER, 0, false);
    
	/*
	
	_keyboardListener = cocos2d::EventListenerKeyboard::create();
	_keyboardListener->retain();
	_keyboardListener->onKeyPressed = CC_CALLBACK_2(LevelScene::_keyPressedCallback, this);
	_keyboardListener->onKeyReleased = CC_CALLBACK_2(LevelScene::_keyReleasedCallback, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_keyboardListener, this);;*/
}

LevelScene::~LevelScene()
{
    auto director = cocos2d::Director::getInstance();
    director->getScheduler()->unscheduleAllForTarget(_level);
    director->getScheduler()->unscheduleAllForTarget(this);
    
	//cocos2d::NotificationCenter::getInstance()->removeAllObservers(this);
    
	//_keyboardListener->release();
	//_eventDispatcher->removeEventListener(_keyboardListener);
    
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

	/*if (_titleNode)
	if (_controlLayer->isPressed())
	{

	}*/
}

void LevelScene::onLevelWin()
{
    
}

void LevelScene::onLevelFail(const std::string& message)
{
    cocos2d::Size winSize = cocos2d::Director::getInstance()->getWinSize();
    
    auto messageLabel = cocos2d::Label::createWithTTF(message, "fonts/Marker Felt.ttf", 18, cocos2d::Size(314, 0), cocos2d::TextHAlignment::CENTER);
    
    auto messagePanel = cocos2d::ui::Scale9Sprite::createWithSpriteFrameName("ui-panel-black-transparent.png");
    messagePanel->setContentSize(cocos2d::Size(360, messageLabel->getContentSize().height + 46));
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
            _onLevelStart();
        }),
        nullptr
    ));
}

void LevelScene::pauseLevel()
{
    if (!_paused)
    {
        _paused = true;
        
        auto director = cocos2d::Director::getInstance();
        director->getScheduler()->pauseTarget(_level);
        
        _pauseRecursive(this);
    }
}

void LevelScene::resumeLevel()
{
    if (_paused)
    {
        _paused = false;
        
        auto director = cocos2d::Director::getInstance();
        director->getScheduler()->resumeTarget(_level);
        
        _resumeRecursive(this);
    }
}

void LevelScene::restartLevel()
{
    if (_paused)
    {
        resumeLevel();
    }
    
    _level->restart();
    _onLevelStart();
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
    
    try
    {
        auto levelConfig = _game->getLevelPack(_packIndex)->readLevelConfig(_levelIndex);
        _level->start(levelConfig);
    }
    catch (...)
    {
        // show error
    }
    
    _preloaderLayer->setVisible(false);
    _onLevelStart();
}

void LevelScene::_onLevelStart()
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

    auto titleLabel = cocos2d::Label::createWithTTF(_level->getConfig()->getTitle(), "fonts/Marker Felt.ttf", 18, cocos2d::Size(314, 0), cocos2d::TextHAlignment::CENTER);
    
    auto titlePanel = cocos2d::ui::Scale9Sprite::createWithSpriteFrameName("ui-panel-black-transparent.png");
    titlePanel->setContentSize(cocos2d::Size(360, titleLabel->getContentSize().height + 46));
    titlePanel->setPosition(cocos2d::Vec2(winSize) * 0.5f);
    
    titleLabel->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
    titleLabel->setPosition(cocos2d::Vec2(titlePanel->getContentSize()) * 0.5f);
    titlePanel->addChild(titleLabel);
    
    _topLayer->addChild(titlePanel);
    
    titlePanel->runAction(cocos2d::Sequence::create(
        cocos2d::DelayTime::create(0.7f),
        cocos2d::FadeOut::create(0.5f),
        cocos2d::CallFuncN::create([](cocos2d::Node* titlePanel)
        {
            titlePanel->removeFromParentAndCleanup(true);
        }),
        nullptr
    ));
}

void LevelScene::_pauseRecursive(cocos2d::Node* node)
{
    node->pause();
    for (auto child : node->getChildren())
    {
        _pauseRecursive(child);
    }
}

void LevelScene::_resumeRecursive(cocos2d::Node* node)
{
    node->resume();
    for (auto child : node->getChildren())
    {
        _resumeRecursive(child);
    }
}

/*
 void LevelScene::showHint(const std::string &hint)
{
	cocos2d::Size winSize = cocos2d::Director::getInstance()->getWinSize();

	auto label = cocos2d::Label::createWithBMFont("fatsans.fnt", hint, cocos2d::TextHAlignment::CENTER, 512 - 46);

	auto hintBox = cocos2d::extension::Scale9Sprite::create("panel-transparent.png");
	hintBox->setContentSize(cocos2d::Size(512, label->getContentSize().height + 46));
	hintBox->setPosition(cocos2d::Vec2(0.5f * winSize.width, 0.25f * winSize.height));
	
	label->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
	label->setPosition(cocos2d::Vec2(hintBox->getContentSize()) * 0.5f);
	hintBox->addChild(label);
    
    show(hintBox);
}

void LevelScene::show(cocos2d::Node* node)
{
    clear();
    _topNode->addChild(node);
}

void LevelScene::clear()
{
    _topNode->removeAllChildren();
}

void LevelScene::levelCompleted(int score, int duration)
{
	_topNode->removeAllChildren();

	auto congratsLayer = CongratsLayer::create();
	_topNode->addChild(congratsLayer);

	_isActive = false;

	//cocos2d::Size winSize = cocos2d::Director::getInstance()->getWinSize();
}

void LevelScene::levelFailed(const std::string& message)
{
	_topNode->removeAllChildren();

	_isActive = false;

	cocos2d::Size winSize = cocos2d::Director::getInstance()->getWinSize();

	auto label = cocos2d::Label::createWithBMFont("main_font.fnt", message, cocos2d::TextHAlignment::CENTER, winSize.width * 0.4);
	label->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
	label->setPosition(cocos2d::Vec2(winSize) * 0.5f);
	_topNode->addChild(label);

	auto delayAction = cocos2d::DelayTime::create(0.7f);
	auto fadeOutAction = cocos2d::FadeOut::create(0.3f);
	auto restartLevelAction = cocos2d::CallFunc::create(std::bind(&LevelScene::_start, this));

	label->runAction(cocos2d::Sequence::create(delayAction, fadeOutAction, restartLevelAction, nullptr));
}

void LevelScene::_build()
{
    _controlLayer = ControlLayer::create(_level);
    addChild(_controlLayer);
    
	_buildBar();
    
	auto winSize = cocos2d::Director::getInstance()->getWinSize();

	_controlsMenu = cocos2d::Menu::create();
	_controlsMenu->setAnchorPoint(cocos2d::Vec2::ZERO);
	_controlsMenu->setPosition(cocos2d::Vec2::ZERO);
	addChild(_controlsMenu);

	auto northItem = cocos2d::MenuItemSprite::create(cocos2d::Sprite::create("CloseNormal.png"), cocos2d::Sprite::create("CloseSelected.png"));
	northItem->setAnchorPoint(cocos2d::Vec2(0, 0));
	northItem->setPosition(cocos2d::Vec2(100, 150));
	northItem->setTag(static_cast<int>(Direction::NORTH));
	_controlsMenu->addChild(northItem);

	auto southItem = cocos2d::MenuItemSprite::create(cocos2d::Sprite::create("CloseNormal.png"), cocos2d::Sprite::create("CloseSelected.png"));
	southItem->setAnchorPoint(cocos2d::Vec2(0, 0));
	southItem->setPosition(cocos2d::Vec2(100, 50));
	southItem->setTag(static_cast<int>(Direction::SOUTH));
	_controlsMenu->addChild(southItem);

	auto westItem = cocos2d::MenuItemSprite::create(cocos2d::Sprite::create("CloseNormal.png"), cocos2d::Sprite::create("CloseSelected.png"));
	westItem->setAnchorPoint(cocos2d::Vec2(0, 0));
	westItem->setPosition(cocos2d::Vec2(50, 100));
	westItem->setTag(static_cast<int>(Direction::WEST));
	_controlsMenu->addChild(westItem);

	auto eastItem = cocos2d::MenuItemSprite::create(cocos2d::Sprite::create("CloseNormal.png"), cocos2d::Sprite::create("CloseSelected.png"));
	eastItem->setAnchorPoint(cocos2d::Vec2(0, 0));
	eastItem->setPosition(cocos2d::Vec2(150, 100));
	eastItem->setTag(static_cast<int>(Direction::EAST));
	_controlsMenu->addChild(eastItem);
    
#if COCOS2D_DEBUG
    auto cheatItem = cocos2d::MenuItemSprite::create(cocos2d::Sprite::create("CloseNormal.png"), cocos2d::Sprite::create("CloseSelected.png"), CC_CALLBACK_1(LevelScene::_cheatCallback, this));
    cheatItem->setAnchorPoint(cocos2d::Point(0.5, 0.5));
    cheatItem->setPosition(cocos2d::Vec2(0, 140));
    _controlsMenu->addChild(cheatItem);
#endif

	auto levelMenu = cocos2d::Menu::create();
	levelMenu->setAnchorPoint(cocos2d::Vec2::ZERO);
	levelMenu->setPosition(cocos2d::Vec2::ZERO);
	addChild(levelMenu);

	auto menuButton = cocos2d::MenuItemSprite::create(cocos2d::Sprite::create("CloseNormal.png"), cocos2d::Sprite::create("CloseSelected.png"), CC_CALLBACK_1(LevelScene::_pauseCallback, this));
	menuButton->setAnchorPoint(cocos2d::Vec2(1, 1));
	menuButton->setPosition(cocos2d::Vec2(winSize) - cocos2d::Vec2(2, 2));
	levelMenu->addChild(menuButton);

	_topNode = cocos2d::Node::create();
	_topNode->retain();
	addChild(_topNode);

	_preloaderLayer = cocos2d::LayerColor::create(cocos2d::Color4B(128, 192, 255, 255), winSize.width, winSize.height);
	_preloaderLayer->setAnchorPoint(cocos2d::Vec2::ZERO);
	_preloaderLayer->setPosition(cocos2d::Vec2::ZERO);
	addChild(_preloaderLayer);

	_fadeLayer = cocos2d::LayerColor::create(cocos2d::Color4B::WHITE, winSize.width, winSize.height);
	_fadeLayer->setAnchorPoint(cocos2d::Vec2::ZERO);
	_fadeLayer->setPosition(cocos2d::Vec2::ZERO);
	_fadeLayer->setVisible(false);
	addChild(_fadeLayer);
}


void LevelScene::_updateBar()
{
	int chipsCollected = _level->getPlayerState()->getChipsCollected();
	int chipsRequired = _level->getChipsRequired();
	_chipsLabel->setString(std::to_string(MIN(chipsCollected, chipsRequired)) + "/" + std::to_string(chipsRequired));

	float x = 0;
	for (auto itemSprite : _inventoryNode->getChildren())
	{
		int itemCount = _level->getPlayerState()->getResourceCount(static_cast<Tile>(itemSprite->getTag()));
		itemSprite->setVisible(itemCount > 0);
		if (itemSprite->isVisible())
		{
			cocos2d::Size size = itemSprite->getBoundingBox().size;
			itemSprite->setPosition(cocos2d::Vec2(x + 0.5f * size.width + 15, itemSprite->getPosition().y));
			x += size.width;
		}
	}
}

void LevelScene::_updateCallback(float dt)
{
	_level->update(dt);

	if (_keys[static_cast<int>(cocos2d::EventKeyboard::KeyCode::KEY_CTRL)] &&
		_keys[static_cast<int>(cocos2d::EventKeyboard::KeyCode::KEY_R)])
	{
		_level->start();
	}
}

void LevelScene::_pauseCallback(cocos2d::Ref* sender)
{
	cocos2d::Director::getInstance()->getScheduler()->pauseTarget(_level);
	_pauseNodeRecursive(_tiledMap);

	showPauseMenu();
}

void LevelScene::_restartCallback(cocos2d::Ref* sender)
{
	_level->start();
	_topNode->removeAllChildrenWithCleanup(true);
}

void LevelScene::_exitCallback(cocos2d::Ref* sender)
{
	auto director = cocos2d::Director::getInstance();
	director->popScene();
}

void LevelScene::_resumeCallback(cocos2d::Ref* sender)
{
	cocos2d::Director::getInstance()->getScheduler()->resumeTarget(_level);
	_resumeNodeRecursive(_tiledMap);

	_topNode->removeAllChildrenWithCleanup(true);
}

void LevelScene::_cheatCallback(cocos2d::Ref* sender)
{
#if COCOS2D_DEBUG
    auto playerState = _level->getPlayerState();
    playerState->setChipsCollected(_level->getChipsRequired());
    playerState->setResourceCount(Tile::BOOTS_FIRE, 1);
    playerState->setResourceCount(Tile::BOOTS_WATER, 1);
    playerState->setResourceCount(Tile::BOOTS_SLIDE, 1);
    playerState->setResourceCount(Tile::BOOTS_ICE, 1);
    playerState->setResourceCount(Tile::KEY_BLUE, 5);
    playerState->setResourceCount(Tile::KEY_GREEN, 5);
    playerState->setResourceCount(Tile::KEY_RED, 5);
    playerState->setResourceCount(Tile::KEY_YELLOW, 5);
#endif
}

void LevelScene::_keyPressedCallback(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* e)
{
	_keys[static_cast<int>(keyCode)] = 1;
}

void LevelScene::_keyReleasedCallback(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* e)
{
	_keys[static_cast<int>(keyCode)] = 0;
}

void LevelScene::_playerStateChangedCallback(cocos2d::Ref* sender)
{
	_updateBar();
}*/
