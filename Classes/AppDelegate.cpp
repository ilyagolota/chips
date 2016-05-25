#include "AppDelegate.h"
#include <Preloader/Preloader.h>
#include <LevelData/LoadLevelListTask.h>
#include "ChipsChallengeGame.h"
#include "LoadingScene.h"

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("Chips", Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("Chips");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // Set the design resolution
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
    Size frameSize = glview->getFrameSize();
    if (frameSize.height > mediumResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(largeResolutionSize.height / designResolutionSize.height, largeResolutionSize.width / designResolutionSize.width));
    }
    else if (frameSize.height > smallResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(mediumResolutionSize.height / designResolutionSize.height, mediumResolutionSize.width / designResolutionSize.width));
    }
    else
    {        
        director->setContentScaleFactor(MIN(smallResolutionSize.height / designResolutionSize.height, smallResolutionSize.width / designResolutionSize.width));
    }

    register_all_packages();

    auto game = ChipsChallengeGame::create();
    
    auto preloader = Preloader::create();
    
    preloader->addTask(LoadTextureTask::create("sheets/creatures.png"));
    preloader->addTask(LoadSpriteSheetTask::create("sheets/creatures.plist"));
    preloader->addTask(LoadTextureTask::create("sheets/tiles-1.png"));
    preloader->addTask(LoadSpriteSheetTask::create("sheets/tiles-1.plist"));
    preloader->addTask(LoadAnimationsTask::create("animations.plist"));
    
    std::vector<char> hash(16);
    memcpy(&hash[0], "\x93<\x02\xdf\x89I/\xed\x9d\xf9\0\x89\xc5#\xf6T", 16);
    preloader->addTask(LoadLevelListTask::create(game->getLevelBundle(), "levels/cclp1.dat", hash));
    memcpy(&hash[0], "\n\x8e\xae\x8c/\fh\xc2/\xae\xfa\x90+\xf3\xc9\xc7", 16);
    preloader->addTask(LoadLevelListTask::create(game->getLevelBundle(), "levels/cclp2.dat", hash));
    memcpy(&hash[0], " k\xfe!\xea\x95\xf0\xe3\x8b\xb6Y/\x9bz(\x16", 16);
    preloader->addTask(LoadLevelListTask::create(game->getLevelBundle(), "levels/cclp3.dat", hash));
    
    auto loadingScene = LoadingScene::create(game, preloader);
    director->runWithScene(loadingScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
