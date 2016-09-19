#include "LevelScene.h"
#include <cocos2d.h>
#include <ChipsChallengeGame.h>
#include <Tiled/TiledProjector.h>
#include <Level/Level.h>
#include <Level/Creature.h>
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

	_level = Level::create(_stage);
	_level->retain();

	_topNode = cocos2d::Node::create();
	addChild(_topNode);
    
  
	_inventoryPanel = InventoryPanel::create(_level->getInventory());
	_inventoryPanel->setAnchorPoint(cocos2d::Vec2(0.5f, 0));
	_inventoryPanel->setPosition(cocos2d::Vec2(0.5f * winSize.width, 8));
	addChild(_inventoryPanel);

    _controlLayer = SmartControlLayer::create(_level);
    addChild(_controlLayer);
	_level->setPlayerControl(_controlLayer);
	
	_level->makeTurn(0);
	director->getScheduler()->schedule(CC_SCHEDULE_SELECTOR(Level::makeTurn), _level, _level->getTurnDuration(), CC_REPEAT_FOREVER, 0, false);
    director->getScheduler()->schedule(CC_SCHEDULE_SELECTOR(LevelScene::update), this, 0, CC_REPEAT_FOREVER, 0, false);
    
	/*_build();
	
	_keyboardListener = cocos2d::EventListenerKeyboard::create();
	_keyboardListener->retain();
	_keyboardListener->onKeyPressed = CC_CALLBACK_2(LevelScene::_keyPressedCallback, this);
	_keyboardListener->onKeyReleased = CC_CALLBACK_2(LevelScene::_keyReleasedCallback, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_keyboardListener, this);

	auto center = cocos2d::NotificationCenter::getInstance();
	center->addObserver(this, CC_CALLFUNCO_SELECTOR(LevelScene::_playerStateChangedCallback), PlayerState::CHIPS_COLLECTED_CHANGED, _level->getPlayerState());
	center->addObserver(this, CC_CALLFUNCO_SELECTOR(LevelScene::_playerStateChangedCallback), PlayerState::RESOURCE_CHANGED, _level->getPlayerState());*/
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
	_loadLevel();
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

void LevelScene::_loadLevel()
{
    auto levelConfig = _game->getLevelPack(_packIndex)->readLevelConfig(_levelIndex);
    _level->start(levelConfig);
	_showTitle();
}

void LevelScene::_showTitle()
{
	cocos2d::Size winSize = cocos2d::Director::getInstance()->getWinSize();

	auto titleLabel = cocos2d::Label::createWithTTF(_level->getConfig()->getTitle(), "fonts/Marker Felt.ttf", 16, cocos2d::Size(314, 0), cocos2d::TextHAlignment::CENTER);

    auto titlePanel = cocos2d::ui::Scale9Sprite::createWithSpriteFrameName("ui-panel-black-transparent.png");
	titlePanel->setContentSize(cocos2d::Size(360, titleLabel->getContentSize().height + 46));
	titlePanel->setPosition(cocos2d::Vec2(winSize) * 0.5f);

	titleLabel->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
	titleLabel->setPosition(cocos2d::Vec2(titlePanel->getContentSize()) * 0.5f);
	titlePanel->addChild(titleLabel);

	_topNode->addChild(titlePanel);

	titlePanel->runAction(cocos2d::Sequence::create(
		cocos2d::DelayTime::create(0.5f),
		cocos2d::FadeOut::create(0.5f),
		cocos2d::CallFuncN::create([](cocos2d::Node* titlePanel)
		{
			titlePanel->removeFromParentAndCleanup(true);
		}),
		nullptr
	));
}

/*
void LevelScene::showPauseMenu()
{
	cocos2d::Size winSize = cocos2d::Director::getInstance()->getWinSize();

	auto menuBox = cocos2d::ui::Scale9Sprite::create("panel-transparent.png");
	menuBox->setContentSize(cocos2d::Size(320, 400));
	menuBox->setPosition(cocos2d::Vec2(winSize) * 0.5);

	auto menu = cocos2d::Menu::create();
	menu->setPosition(cocos2d::Vec2::ZERO);
	menu->setAnchorPoint(cocos2d::Vec2::ZERO);

	auto restartButton = cocos2d::MenuItemSprite::create(cocos2d::Sprite::create("bomb.png"), nullptr, CC_CALLBACK_1(LevelScene::_restartCallback, this));
	restartButton->setPosition(cocos2d::Vec2(0.5f * menuBox->getContentSize().width, 250));
	restartButton->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
	menu->addChild(restartButton);

	auto restartButtonLabel = cocos2d::Label::createWithBMFont("main_font.fnt", "Restart");
	restartButtonLabel->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
	restartButtonLabel->setPosition(0.5f * cocos2d::Vec2(restartButton->getContentSize()));
	restartButton->addChild(restartButtonLabel);

	auto exitButton = cocos2d::MenuItemSprite::create(cocos2d::Sprite::create("bomb.png"), nullptr, CC_CALLBACK_1(LevelScene::_exitCallback, this));
	exitButton->setPosition(cocos2d::Vec2(0.5f * menuBox->getContentSize().width, 150));
	exitButton->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
	menu->addChild(exitButton);

	auto exitButtonLabel = cocos2d::Label::createWithBMFont("main_font.fnt", "Exit");
	exitButtonLabel->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
	exitButtonLabel->setPosition(0.5f * cocos2d::Vec2(exitButton->getContentSize()));
	exitButton->addChild(exitButtonLabel);

	auto closeButton = cocos2d::MenuItemSprite::create(cocos2d::Sprite::create("bomb.png"), nullptr, CC_CALLBACK_1(LevelScene::_resumeCallback, this));
	closeButton->setPosition(cocos2d::Vec2(0.5f * menuBox->getContentSize().width, 50));
	closeButton->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
	menu->addChild(closeButton);

	auto closeButtonLabel = cocos2d::Label::createWithBMFont("main_font.fnt", "Close");
	closeButtonLabel->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
	closeButtonLabel->setPosition(0.5f * cocos2d::Vec2(closeButton->getContentSize()));
	closeButton->addChild(closeButtonLabel);

	menuBox->addChild(menu);
	show(menuBox);
}

void LevelScene::showLevelTitle(const std::string& title)
{
	cocos2d::Size winSize = cocos2d::Director::getInstance()->getWinSize();

	auto label = cocos2d::Label::createWithBMFont("main_font.fnt", title, cocos2d::TextHAlignment::CENTER, 360 - 46);

	auto titleBox = cocos2d::extension::Scale9Sprite::create("panel-transparent.png");
	titleBox->setContentSize(cocos2d::Size(360, label->getContentSize().height + 46));
	titleBox->setPosition(cocos2d::Vec2(winSize) * 0.5f);

	label->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
	label->setPosition(cocos2d::Vec2(titleBox->getContentSize()) * 0.5f);
	titleBox->addChild(label);

	auto delay = cocos2d::DelayTime::create(0.5f);
	auto fadeOut = cocos2d::FadeOut::create(0.5f);
	auto destroy = cocos2d::CallFunc::create(CC_CALLBACK_0(LevelScene::clear, this));
	titleBox->runAction(cocos2d::Sequence::create(delay, fadeOut, destroy, nullptr));

	show(titleBox);
}

void LevelScene::showAlert(const std::string& message)
{
	cocos2d::Size winSize = cocos2d::Director::getInstance()->getWinSize();

	auto label = cocos2d::Label::createWithBMFont("main_font.fnt", message, cocos2d::TextHAlignment::CENTER, 512 - 46);

	auto alertBox = cocos2d::extension::Scale9Sprite::create("panel-transparent.png");
	alertBox->setContentSize(cocos2d::Size(512, label->getContentSize().height + 46));
	alertBox->setPosition(cocos2d::Vec2(winSize) * 0.5f);

	label->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
	label->setPosition(cocos2d::Vec2(alertBox->getContentSize()) * 0.5f);
	alertBox->addChild(label);

	show(alertBox);
}

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

void LevelScene::_loadAndStart()
{
	_preloaderLayer->setVisible(true);

	auto director = cocos2d::Director::getInstance();
	director->drawScene();

	try
	{
		auto levelData = App::getCurrent()->getLevelBundle()->readLevelData(_packIndex, _levelIndex);
		_level->setLevelData(levelData);
	}
	catch (std::runtime_error error)
	{
		// show error
	}
	
	_preloaderLayer->setVisible(false);

	_fadeLayer->setVisible(true);
	_fadeLayer->setOpacity(255);

	_start();
}

void LevelScene::_start()
{
	_fadeLayer->stopAllActions();
	_fadeLayer->setVisible(true);

	if (_fadeLayer->getOpacity() == 255)
	{
		_isActive = true;
		_level->start();

		float duration = 0.2f * _fadeLayer->getOpacity() / 255.0f;
		auto fadeOutAction = cocos2d::FadeTo::create(duration, 0);
		auto hideFadeLayerAction = cocos2d::CallFunc::create([this]() -> void {
			_fadeLayer->setVisible(false);
		});

		_fadeLayer->runAction(cocos2d::Sequence::createWithTwoActions(fadeOutAction, hideFadeLayerAction));
	}
	else
	{
		float duration = 0.4f * (255.f - _fadeLayer->getOpacity()) / 255.0f;
		auto fadeInAction = cocos2d::FadeTo::create(duration, 255);
		auto startGameAction = cocos2d::CallFunc::create([this]() -> void {
			_isActive = true;
			_level->start();
		});
		auto fadeOutAction = cocos2d::FadeTo::create(0.2f, 0);
		auto hideFadeLayerAction = cocos2d::CallFunc::create([this]() -> void {
			_fadeLayer->setVisible(false);
		});

		_fadeLayer->runAction(cocos2d::Sequence::create(fadeInAction, startGameAction, fadeOutAction, hideFadeLayerAction, nullptr));
	}
}

void LevelScene::_pauseNodeRecursive(cocos2d::Node* node)
{
	node->pauseSchedulerAndActions();
	for (auto child : node->getChildren())
	{
		_pauseNodeRecursive(child);
	}
}

void LevelScene::_resumeNodeRecursive(cocos2d::Node* node)
{
	node->resumeSchedulerAndActions();
	for (auto child : node->getChildren())
	{
		_resumeNodeRecursive(child);
	}
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

void LevelScene::_buildBar()
{
	auto winSize = cocos2d::Director::getInstance()->getWinSize();

	auto bar = cocos2d::extension::Scale9Sprite::create("panel-transparent.png");
	bar->setContentSize(cocos2d::Size(180, 50));
	bar->setAnchorPoint(cocos2d::Vec2(0, 1));
	bar->setPosition(cocos2d::Vec2(10, winSize.height - 10));
	addChild(bar);

	auto chipSprite = cocos2d::Sprite::create("icchip.png");
	chipSprite->setPosition(cocos2d::Vec2(30, bar->getContentSize().height * 0.5f));
	chipSprite->setRotation(10);
	chipSprite->setScale(0.6f);
	bar->addChild(chipSprite);

	_chipsLabel = cocos2d::Label::createWithBMFont("main_font.fnt", "0/0", cocos2d::TextHAlignment::LEFT);
	_chipsLabel->setPosition(cocos2d::Vec2(50, bar->getContentSize().height * 0.5f));
	_chipsLabel->setAnchorPoint(cocos2d::Vec2(0, 0.5));
	bar->addChild(_chipsLabel);

	auto bar2 = cocos2d::extension::Scale9Sprite::create("panel-transparent.png");
	bar2->setContentSize(cocos2d::Size(280, 50));
	bar2->setPosition(cocos2d::Vec2(200, winSize.height - 10));
	bar2->setAnchorPoint(cocos2d::Vec2(0, 1));
	addChild(bar2);

	auto blueKeySprite = cocos2d::Sprite::create("key-blue.png");
	blueKeySprite->setTag(static_cast<int>(Tile::KEY_BLUE));
	bar2->addChild(blueKeySprite);

	auto redKeySprite = cocos2d::Sprite::create("key-red.png");
	redKeySprite->setTag(static_cast<int>(Tile::KEY_RED));
	bar2->addChild(redKeySprite);

	auto greenKeySprite = cocos2d::Sprite::create("key-green.png");
	greenKeySprite->setTag(static_cast<int>(Tile::KEY_GREEN));
	bar2->addChild(greenKeySprite);

	auto yellowKeySprite = cocos2d::Sprite::create("key-yellow.png");
	yellowKeySprite->setTag(static_cast<int>(Tile::KEY_YELLOW));
	bar2->addChild(yellowKeySprite);

	auto fireBootsSprite = cocos2d::Sprite::create("boots-fire.png");
	fireBootsSprite->setTag(static_cast<int>(Tile::BOOTS_FIRE));
	bar2->addChild(fireBootsSprite);

	auto waterBootsSprite = cocos2d::Sprite::create("boots-water.png");
	waterBootsSprite->setTag(static_cast<int>(Tile::BOOTS_WATER));
	bar2->addChild(waterBootsSprite);

	auto iceBootsSprite = cocos2d::Sprite::create("boots-ice.png");
	iceBootsSprite->setTag(static_cast<int>(Tile::BOOTS_ICE));
	bar2->addChild(iceBootsSprite);

	auto slideBootsSprite = cocos2d::Sprite::create("boots-slide.png");
	slideBootsSprite->setTag(static_cast<int>(Tile::BOOTS_SLIDE));
	bar2->addChild(slideBootsSprite);

	for (auto itemSprite : bar2->getChildren())
	{
		itemSprite->setScale(0.6f);
		itemSprite->setPosition(cocos2d::Vec2(0, 0.5f * bar2->getContentSize().height));
	}

	_inventoryNode = bar2;
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
