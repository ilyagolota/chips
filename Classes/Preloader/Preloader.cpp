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

void Preloader::addTask(const std::function<void()>& task)
{
    _asyncTasks.push_back([task](const std::function<void()>& callback)
    {
        task();
        callback();
    });
}

void Preloader::addTask(const std::function<void(const std::function<void()>&)>& asyncTask)
{
    _asyncTasks.push_back(asyncTask);
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
    return _asyncTasks.size();
}

void Preloader::update(float /*dt*/)
{
    if (_loaded < _asyncTasks.size())
    {
        if (!_hasActiveTask)
        {
            _hasActiveTask = true;
            _asyncTasks[_loaded]([this] () {
                _hasActiveTask = false;
                _loaded++;
            });
        }
    }
}
