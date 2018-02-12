#ifndef  _CHIPS_CHALLENGE_LEVEL_SCENE_H_
#define  _CHIPS_CHALLENGE_LEVEL_SCENE_H_

#include <cocos2d.h>
#include <ui/CocosGUI.h>

#include "LevelScene/InventoryPanel.h"
#include "LevelScene/PauseMenuWindow.h"
#include "LevelScene/ControlLayer.h"
#include <Level/Level.h>
#include <ChipsChallengeGame.h>

class LevelScene : public cocos2d::Scene, LevelHandler
{
public:
    static LevelScene* create(ChipsChallengeGame* game, size_t packIndex, size_t levelIndex);

    LevelScene(ChipsChallengeGame* game, size_t packIndex, size_t levelIndex);
    virtual ~LevelScene() override;

    void onEnter() override;
    void update(float dt) override;
    void updatePerTurn(float dt);

    void onLevelWin() override;
    void onLevelFail(const std::string& message) override;
    void showHint(const std::string& hint, const cocos2d::Vec2& position) override;
    void hideHint(const cocos2d::Vec2& position) override;
    
    void pauseLevel();
    void resumeLevel();
    void restartLevel();
    void gotoLevel(size_t levelIndex);
    
protected:
    static const int HINT_TAG = 1997;
    static size_t __tilesetIndex;
    
    void onLevelStart();
    void pauseRecursive(cocos2d::Node* node);
    void resumeRecursive(cocos2d::Node* node);
    
    ChipsChallengeGame* _game;
    Level *_level;
    size_t _packIndex;
    size_t _levelIndex;
    bool _paused;
    
    double _lastUnpauseTime;
    double _elapsedTimeBeforeLastPause;

    cocos2d::Node* _stage;
    std::vector<ControlLayer*> _controlLayers;
    cocos2d::Node *_topLayer;
    cocos2d::Layer *_preloaderLayer;
    cocos2d::Layer *_fadeLayer;
    InventoryPanel* _inventoryPanel;
    
    cocos2d::Label *_chipsLabel;
    cocos2d::Node *_inventoryNode;
    cocos2d::Menu *_controlsMenu;
    cocos2d::Menu *_levelMenu;
    cocos2d::Menu *_mainMenu;
};

#endif
