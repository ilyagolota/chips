#ifndef _CHIPS_CHALLENGE_TILE_SOUND_EMITTER_H_
#define _CHIPS_CHALLENGE_TILE_SOUND_EMITTER_H_

#include <cocos2d.h>

class TileSoundEmitter : public cocos2d::Ref
{
private:
    cocos2d::Vec2 _coordinate;
    std::string _soundName;
    
public:
    static TileSoundEmitter* create(std::string&& soundName);
    static TileSoundEmitter* create(const std::string& soundName);
    
    TileSoundEmitter(std::string&& soundName);
    TileSoundEmitter(const std::string& soundName);
    cocos2d::Vec2 getCoordinate() const;
    void setCoordinate(const cocos2d::Vec2& coordinate);
    const std::string& getSoundName() const;
    void setSoundName(std::string&& soundName);
    void setSoundName(const std::string& soundName);
};

#endif
