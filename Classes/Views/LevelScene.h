#ifndef  _CHIPS_CHALLENGE_LEVEL_SCENE_H_
#define  _CHIPS_CHALLENGE_LEVEL_SCENE_H_

#include <cocos2d.h>

class ChipsChallengeGame;
class Level;
class ControlLayer;

class LevelScene : public cocos2d::Scene
{
protected:
    ChipsChallengeGame* _game;
    Level *_level;
    size_t _packIndex;
    size_t _levelIndex;
    bool _paused;

    cocos2d::Node* _stage;
    
    //ControlLayer* _controlLayer;
    
    cocos2d::Label *_chipsLabel;
    cocos2d::Node *_inventoryNode;
    
    cocos2d::Menu *_controlsMenu;
    cocos2d::Menu *_levelMenu;
    cocos2d::Menu *_mainMenu;
    cocos2d::Node *_topNode;
    cocos2d::Layer *_preloaderLayer;
    cocos2d::Layer *_fadeLayer;
    
    //cocos2d::EventListenerKeyboard *_keyboardListener;
    //char _keys[256];
    
public:
    static LevelScene* create(ChipsChallengeGame* game, size_t packIndex, size_t levelIndex);
    
    LevelScene(ChipsChallengeGame* game, size_t packIndex, size_t levelIndex);
    virtual ~LevelScene() override;
	
    void onEnter() override;
    void update(float dt) override;
    
    /*TiledMap* getTiledMap();
    Direction getSelectedDirection();
	void showPauseMenu();
	void showLevelTitle(const std::string &title);
	void showAlert(const std::string& message);
	void showHint(const std::string& hint);
    void show(cocos2d::Node* node);
    void clear();

	void levelCompleted(int score, int duration);
	void levelFailed(const std::string& message);*/

private:
    void _loadLevel();
	
	/*
	void _start();

	void _pauseNodeRecursive(cocos2d::Node* node);
	void _resumeNodeRecursive(cocos2d::Node* node);
	
	void _build();
	void _buildBar();
	void _updateBar();

	void _scheduledLoadLevelCallback(float dt);
	void _updateCallback(float dt);
	void _pauseCallback(cocos2d::Ref* sender);
	void _restartCallback(cocos2d::Ref* sender);
	void _exitCallback(cocos2d::Ref* sender);
	void _resumeCallback(cocos2d::Ref* sender);
    void _cheatCallback(cocos2d::Ref* sender);

	void _keyPressedCallback(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* e);
	void _keyReleasedCallback(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* e);

	void _playerStateChangedCallback(cocos2d::Ref* sender);*/
};

#endif
