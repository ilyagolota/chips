#include "ChipsChallengeGame.h"

ChipsChallengeGame* ChipsChallengeGame::create()
{
    auto instance = new ChipsChallengeGame();
    instance->autorelease();
    return instance;
}

ChipsChallengeGame::ChipsChallengeGame()
{ }

ChipsChallengeGame::~ChipsChallengeGame()
{ }

cocos2d::Vector<LevelPack*>& ChipsChallengeGame::getLevelPacks()
{
    return _levelPacks;
}

LevelPack* ChipsChallengeGame::getLevelPack(size_t i)
{
    return _levelPacks.at(i);
}
