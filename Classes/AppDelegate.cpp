#include "AppDelegate.h"
#include <Preloader/Preloader.h>
#include <Views/LoadingScene.h>
#include "ChipsChallengeGame.h"

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
    /*Size frameSize = glview->getFrameSize();
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
    }*/

    register_all_packages();
    
    const char* hash1 = "\x93<\x02\xdf\x89I/\xed\x9d\xf9\0\x89\xc5#\xf6T";
    const char* hash2 = "\n\x8e\xae\x8c/\fh\xc2/\xae\xfa\x90+\xf3\xc9\xc7";
    const char* hash3 = " k\xfe!\xea\x95\xf0\xe3\x8b\xb6Y/\x9bz(\x16";

    auto game = ChipsChallengeGame::create();
    
    auto preloader = Preloader::create();
	preloader->addTask(_loadSpriteSheet("sheets/ui.plist"));
    preloader->addTask(_loadSpriteSheet("sheets/creatures.plist"));
    preloader->addTask(_loadSpriteSheet("sheets/tiles-1.plist"));
	preloader->addTask(_loadSpriteSheet("sheets/tiles-2.plist"));
	preloader->addTask(_loadSpriteSheet("sheets/tiles-3.plist"));
    preloader->addTask(_loadAnimations("animations.plist"));
    preloader->addTask(_loadSound("sounds/door.mp3"));
	preloader->addTask(_loadSound("sounds/water.mp3"));
    preloader->addTask(_loadSound("sounds/fire.mp3"));
    preloader->addTask(_loadSound("sounds/glider.wav"));
    preloader->addTask(_loadLevelPack(game, "levels/cclp1.dat", std::vector<char>(hash1, hash1 + 16)));
    preloader->addTask(_loadLevelPack(game, "levels/cclp2.dat", std::vector<char>(hash2, hash2 + 16)));
    preloader->addTask(_loadLevelPack(game, "levels/cclp3.dat", std::vector<char>(hash3, hash3 + 16)));
    
    std::vector<char> highscoreSecret(hash3, hash3 + 16);
	preloader->addTask(std::function<void()>([game, highscoreSecret]() {
        game->setHighscores(HighscoreStorage::create("highscores.sav", highscoreSecret));
    }));
    
    auto loadingScene = LoadingScene::create(game, preloader);
    director->runWithScene(loadingScene);

    return true;
}

void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();
	cocos2d::experimental::AudioEngine::pauseAll();
}

void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();
	cocos2d::experimental::AudioEngine::resumeAll();
}

std::function<void()> AppDelegate::_loadSpriteSheet(const std::string& filename)
{
    return [filename]()
    {
        cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile(filename);
    };
}

std::function<void()> AppDelegate::_loadAnimations(const std::string& filename)
{
    return [filename]()
    {
        cocos2d::AnimationCache::getInstance()->addAnimationsWithFile(filename);
    };
}

std::function<void(const std::function<void()>&)> AppDelegate::_loadSound(const std::string& filename)
{
    return [filename](const std::function<void()>& callback)
    {
        cocos2d::experimental::AudioEngine::preload(filename, [callback](bool /*success*/)
        {
            callback();
        });
    };
}

std::function<void()> AppDelegate::_loadLevelPack(ChipsChallengeGame* game, const std::string& filename, const std::vector<char>& hash)
{
    return [game, filename, hash]()
    {
        game->getLevelPacks().pushBack(LevelPack::create(filename, hash));
    };
}
