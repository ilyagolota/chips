#ifndef  _CHIPS_CHALLENGE_LEVEL_PACK_H_
#define  _CHIPS_CHALLENGE_LEVEL_PACK_H_

#include <string>
#include <vector>
#include <cocos2d.h>
#include "LevelConfig.h"

struct LevelPackEntry
{
    size_t offset;
    size_t size;
    std::string title;
};

class LevelPack : public cocos2d::Ref
{
public:
    static LevelPack* create(const std::string& filename, const std::vector<char>& hash);
    
    LevelPack(const std::string& filename, const std::vector<char>& hash);
    const std::vector<LevelPackEntry>& getEntries() const;
    LevelConfig* readLevelConfig(size_t entryIndex);

protected:
    std::string _filename;
    std::vector<char> _hash;
    std::vector<LevelPackEntry> _entries;
};

#endif
