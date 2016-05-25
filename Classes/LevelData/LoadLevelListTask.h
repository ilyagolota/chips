#ifndef  _CHIPS_CHALLENGE_PRELOAD_LEVEL_LIST_TASK_H_
#define  _CHIPS_CHALLENGE_PRELOAD_LEVEL_LIST_TASK_H_

#include <string>
#include <vector>
#include <Preloader/Preloader.h>
#include "LevelBundle.h"

class LoadLevelListTask : public Task
{
protected:
    LevelBundle* _levelBundle;
    std::string _filename;
    std::vector<char> _hash;
    
public:
    static LoadLevelListTask* create(LevelBundle* levelBundle, const std::string& filename, const std::vector<char>& hash);
    
    LoadLevelListTask(LevelBundle* levelBundle, const std::string& filename, const std::vector<char>& hash);
    void exec() override;
};

#endif
