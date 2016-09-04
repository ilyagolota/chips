#ifndef _CHIPS_CHALLENGE_HIGHSCORES_H_
#define _CHIPS_CHALLENGE_HIGHSCORES_H_

#include <string>
#include <vector>
#include <cocos2d.h>

struct HighscoreRecord
{
	int minTime;
	int minTryCount;
	int maxScore;
	int currentTryCount;

	HighscoreRecord() : minTime(0), minTryCount(0), maxScore(0), currentTryCount(0)
	{ }
};

class Highscores : public cocos2d::Ref
{
public:
	static Highscores* create(const std::string& filename, const std::string& secret);
    
	Highscores(const std::string& filename, const std::string& secret);
	virtual ~Highscores();
	HighscoreRecord get(size_t packIndex, size_t levelIndex);
	void set(size_t packIndex, size_t levelIndex, const HighscoreRecord& record);

private:
	void _load();
	void _save();

	std::string _filename;
	std::string _secret;
	std::vector<std::vector<HighscoreRecord>> _recordPacks;
};

#endif
