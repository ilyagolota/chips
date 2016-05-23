#ifndef  _CHIPS_CHALLENGE_PRELOADER_H_
#define  _CHIPS_CHALLENGE_PRELOADER_H_

#include <cocos2d.h>
#include <string>

class Task : public cocos2d::Ref
{
public:
    virtual void exec() = 0;
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

#endif
