#include "Preloader.h"

Preloader* Preloader::create()
{
    auto instance = new Preloader();
    instance->autorelease();
    return instance;
}

Preloader::Preloader():
    _loaded(0),
    _hasActiveTask(false)
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
    if (_loaded < _tasks.size())
    {
        auto task = _tasks.at(_loaded);
        
        if (!_hasActiveTask)
        {
            task->exec();
            _hasActiveTask = true;
        }
    
        if (task->isCompleted())
        {
            _hasActiveTask = false;
            _loaded++;
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
    _completed = true;
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
    _completed = true;
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
    _completed = true;
}

LoadSoundTask* LoadSoundTask::create(const std::string& filename)
{
    auto instance = new LoadSoundTask(filename);
    instance->autorelease();
    return instance;
}

LoadSoundTask::LoadSoundTask(const std::string& filename)
: _filename(filename)
{ }

void LoadSoundTask::exec()
{
    cocos2d::experimental::AudioEngine::preload(_filename, std::bind(&LoadSoundTask::_onLoad, this, std::placeholders::_1));
}

void LoadSoundTask::_onLoad(bool loaded)
{
    _completed = true;
}
