#include "Preloader.h"

Preloader* Preloader::create()
{
    auto instance = new Preloader();
    instance->autorelease();
    return instance;
}

Preloader::Preloader()
: _loaded(0)
{ }

void Preloader::addTask(Task* task)
{
    _tasks.pushBack(task);
}

bool Preloader::isComplete() const
{
    return getLoaded() >= getTotal();
}

size_t Preloader::getLoaded() const
{
    return _loaded;
}

size_t Preloader::getTotal() const
{
    return _tasks.size();
}

void Preloader::process()
{
    double startTime = _getCurrentTime();
    float maxDuration = 0.25 * cocos2d::Director::getInstance()->getAnimationInterval();
    
    while (_loaded < _tasks.size())
    {
        _tasks.at(_loaded)->exec();
        _loaded++;
        
        double now = _getCurrentTime();
        if (now - startTime >= maxDuration)
        {
            break;
        }
    }
}

double Preloader::_getCurrentTime()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    timeb buffer;
    ftime(&buffer);
    return (double)buffer.time + buffer.millitm * 0.001;
#else
    timeval buffer;
    gettimeofday(&buffer, NULL);
    return (double)buffer.tv_sec + buffer.tv_usec * 0.000001;
#endif
}

LoadTextureTask* LoadTextureTask::create(const std::string& filename)
{
    auto instance = new LoadTextureTask(filename);
    instance->autorelease();
    return instance;
}

LoadTextureTask::LoadTextureTask(const std::string& filename)
: _filename(filename)
{ }

void LoadTextureTask::exec()
{
    cocos2d::Director::getInstance()->getTextureCache()->addImage(_filename);
}

LoadSpriteSheetTask* LoadSpriteSheetTask::create(const std::string& filename)
{
    auto instance = new LoadSpriteSheetTask(filename);
    instance->autorelease();
    return instance;
}

LoadSpriteSheetTask::LoadSpriteSheetTask(const std::string& filename)
: _filename(filename)
{ }

void LoadSpriteSheetTask::exec()
{
    cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile(_filename);
}

LoadAnimationsTask* LoadAnimationsTask::create(const std::string& filename)
{
    auto instance = new LoadAnimationsTask(filename);
    instance->autorelease();
    return instance;
}

LoadAnimationsTask::LoadAnimationsTask(const std::string& filename)
: _filename(filename)
{ }

void LoadAnimationsTask::exec()
{
    cocos2d::AnimationCache::getInstance()->addAnimationsWithFile(_filename);
}
