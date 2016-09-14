#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "cocos2d.h"
#include "AudioEngine.h"

/**
@brief    The cocos2d Application.

The reason for implement as private inheritance is to hide some interface call by Director.
*/
class ChipsChallengeGame;

class  AppDelegate : private cocos2d::Application
{
public:
    AppDelegate();
    virtual ~AppDelegate();

    virtual void initGLContextAttrs();

    /**
    @brief    Implement Director and Scene init code here.
    @return true    Initialize success, app continue.
    @return false   Initialize failed, app terminate.
    */
    virtual bool applicationDidFinishLaunching();

    /**
    @brief  The function be called when the application enter background
    @param  the pointer of the application
    */
    virtual void applicationDidEnterBackground();

    /**
    @brief  The function be called when the application enter foreground
    @param  the pointer of the application
    */
    virtual void applicationWillEnterForeground();
    
private:
    std::function<void()> _loadSpriteSheet(const std::string& filename);
    std::function<void()> _loadAnimations(const std::string& filename);
    std::function<void(const std::function<void()>&)> _loadSound(const std::string& filename);
    std::function<void()> _loadLevelPack(ChipsChallengeGame* game, const std::string& filename, const std::vector<char>& hash);
};

#endif // _APP_DELEGATE_H_
