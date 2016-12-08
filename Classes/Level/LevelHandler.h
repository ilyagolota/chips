#ifndef _CHIPS_CHALLENGE_LEVEL_HANDLER_H_
#define _CHIPS_CHALLENGE_LEVEL_HANDLER_H_

class LevelHandler
{
public:
    virtual void onLevelWin() = 0;
    virtual void onLevelFail(const std::string& message) = 0;
    virtual void showHint(const std::string& hint, const cocos2d::Vec2& position) = 0;
    virtual void hideHint(const cocos2d::Vec2& position) = 0;
};

#endif
