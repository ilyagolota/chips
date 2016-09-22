#ifndef _CHIPS_CHALLENGE_PAUSE_MENU_WINDOW_H_
#define _CHIPS_CHALLENGE_PAUSE_MENU_WINDOW_H_

#include <cocos2d.h>
#include <ui/CocosGUI.h>

class LevelScene;

class PauseMenuWindow : public cocos2d::Node
{
public:
    static PauseMenuWindow* create(LevelScene* levelScene);
    
    PauseMenuWindow(LevelScene* levelScene);
    
private:
    LevelScene* _levelScene;
};

#endif
