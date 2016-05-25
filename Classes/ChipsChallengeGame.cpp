#include "ChipsChallengeGame.h"

ChipsChallengeGame* ChipsChallengeGame::create()
{
    auto instance = new ChipsChallengeGame();
    instance->autorelease();
    return instance;
}

ChipsChallengeGame::ChipsChallengeGame()
{
    _levelBundle = LevelBundle::create();
    _levelBundle->retain();
}

ChipsChallengeGame::~ChipsChallengeGame()
{
    _levelBundle->release();
}

LevelBundle* ChipsChallengeGame::getLevelBundle()
{
    return _levelBundle;
}
