#ifndef  _CHIPS_CHALLENGE_PRELOADER_H_
#define  _CHIPS_CHALLENGE_PRELOADER_H_

#include <cocos2d.h>
#include <string>

class Preloader : public cocos2d::Ref
{
public:
    static Preloader* create();
    
    Preloader();
    void addTask(const std::function<void()>& task);
    void addTask(const std::function<void(const std::function<void()>&)>& asyncTask);
    bool isComplete() const;
    size_t getLoaded() const;
    size_t getTotal() const;
    void update(float dt);
    
protected:
    double _getCurrentTime();
    
    std::vector<std::function<void(const std::function<void()>&)>> _asyncTasks;
    size_t _loaded;
    bool _hasActiveTask;
};

#endif
