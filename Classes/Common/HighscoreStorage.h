#ifndef _CHIPS_CHALLENGE_HIGHSCORES_H_
#define _CHIPS_CHALLENGE_HIGHSCORES_H_

#include <string>
#include <vector>
#include <cocos2d.h>

struct HighscoreRecord
{
    bool completed;
    bool tried;
	int minTime;
	int minDeathCount;
	int maxScore;
	int deathCount;

	HighscoreRecord():
        completed(false),
        minTime(0),
        minDeathCount(0),
        maxScore(0),
        deathCount(0)
	{ }
};

class HighscoreStorage : public cocos2d::Ref
{
public:
	static HighscoreStorage* create(const std::string& filename, const std::vector<char>& secret);
    
	HighscoreStorage(const std::string& filename, const std::vector<char>& secret);
    const HighscoreRecord& getRecord(size_t packIndex, size_t levelIndex);
	void saveRecord(size_t packIndex, size_t levelIndex, const HighscoreRecord& record);

private:
	void load();
	void save();

	std::string _filename;
	std::vector<char> _secret;
	std::vector<std::vector<HighscoreRecord>> _recordPacks;
};

#endif
