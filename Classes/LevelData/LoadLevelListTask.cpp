#include "LoadLevelListTask.h"

LoadLevelListTask* LoadLevelListTask::create(LevelBundle* levelBundle, const std::string& filename, const std::vector<char>& hash)
{
    auto instance = new LoadLevelListTask(levelBundle, filename, hash);
    instance->autorelease();
    return instance;
}

LoadLevelListTask::LoadLevelListTask(LevelBundle* levelBundle, const std::string& filename, const std::vector<char>& hash)
{
    _levelBundle = levelBundle;
    _filename = filename;
    _hash = hash;
}

void LoadLevelListTask::exec()
{
    _levelBundle->preloadLevelPack(_filename, _hash);
    _completed = true;
}
