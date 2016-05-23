#ifndef  _CHIPS_CHALLENGE_LEVEL_BUNDLE_H_
#define  _CHIPS_CHALLENGE_LEVEL_BUNDLE_H_

#include <string>
#include <vector>
#include <cocos2d.h>
#include "LevelData.h"

struct LevelInfo
{
    size_t offset;
    size_t size;
    std::string title;
};

struct LevelPack
{
    std::string filename;
    std::vector<char> hash;
    std::vector<LevelInfo> levelInfo;
};

class LevelBundle : public cocos2d::Ref
{
public:
    static LevelBundle* create();
    
    void preloadLevelPack(const std::string& filename, const std::vector<char>& hash);
    const std::vector<LevelPack>& getLevelPacks() const;
    LevelData* readLevelData(size_t packIndex, size_t levelIndex);

protected:
    std::vector<LevelPack> _levelPacks;
};

#endif
