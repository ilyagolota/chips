#include "ChipsChallengeGame.h"

ChipsChallengeGame* ChipsChallengeGame::create()
{
    auto instance = new ChipsChallengeGame();
    instance->autorelease();
    return instance;
}

ChipsChallengeGame::ChipsChallengeGame():
    _highscores(nullptr)
{ }

ChipsChallengeGame::~ChipsChallengeGame()
{
    CC_SAFE_RELEASE(_highscores);
}

cocos2d::Vector<LevelPack*>& ChipsChallengeGame::getLevelPacks()
{
    return _levelPacks;
}

LevelPack* ChipsChallengeGame::getLevelPack(size_t i)
{
    return _levelPacks.at(i);
}

HighscoreStorage* ChipsChallengeGame::getHighscores()
{
    return _highscores;
}

void ChipsChallengeGame::setHighscores(HighscoreStorage* highscores)
{
    if (_highscores != highscores)
    {
        CC_SAFE_RELEASE(_highscores);
        _highscores = highscores;
        CC_SAFE_RETAIN(_highscores);
    }
}
