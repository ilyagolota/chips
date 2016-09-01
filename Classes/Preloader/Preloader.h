#ifndef  _CHIPS_CHALLENGE_PRELOADER_H_
#define  _CHIPS_CHALLENGE_PRELOADER_H_

#include <cocos2d.h>
#include <string>
#include "AudioEngine.h"

class Task : public cocos2d::Ref
{
public:
    Task()
    {
        _completed = false;
    }
    
    bool isCompleted()
    {
        return _completed;
    }
    
    virtual void exec() = 0;
    
protected:
    bool _completed;
};

class Preloader : public cocos2d::Ref
{
public:
    static Preloader* create();
    
    Preloader();
    void addTask(Task* task);
    bool isComplete() const;
    size_t getLoaded() const;
    size_t getTotal() const;
    void process();
    
protected:
    double _getCurrentTime();
    
    cocos2d::Vector<Task*> _tasks;
    size_t _loaded;
    bool _hasActiveTask;
};

class LoadTextureTask : public Task
{
private:
    std::string _filename;
    
public:
    static LoadTextureTask* create(const std::string& filename);
    LoadTextureTask(const std::string& filename);
    void exec() override;
};

class LoadSpriteSheetTask : public Task
{
private:
    std::string _filename;
    
public:
    static LoadSpriteSheetTask* create(const std::string& filename);
    LoadSpriteSheetTask(const std::string& filename);
    void exec() override;
};

class LoadAnimationsTask : public Task
{
private:
    std::string _filename;
    
public:
    static LoadAnimationsTask* create(const std::string& filename);
    LoadAnimationsTask(const std::string& filename);
    void exec() override;
};

class LoadSoundTask : public Task
{
public:
    static LoadSoundTask* create(const std::string& filename);
    LoadSoundTask(const std::string& filename);
    void exec() override;
    
private:
    void _onLoad(bool loaded);
    
    std::string _filename;
};

#endif
