#include "TileSoundEmitter.h"

TileSoundEmitter* TileSoundEmitter::create(std::string&& soundName)
{
    auto instance = new TileSoundEmitter(std::move(soundName));
    instance->autorelease();
    return instance;
}

TileSoundEmitter* TileSoundEmitter::create(const std::string& soundName)
{
    auto instance = new TileSoundEmitter(soundName);
    instance->autorelease();
    return instance;
}

TileSoundEmitter::TileSoundEmitter(std::string&& soundName)
: _soundName(std::move(soundName))
{ }

TileSoundEmitter::TileSoundEmitter(const std::string& soundName)
: _soundName(soundName)
{ }

cocos2d::Vec2 TileSoundEmitter::getCoordinate() const
{
    return _coordinate;
}

void TileSoundEmitter::setCoordinate(const cocos2d::Vec2& coordinate)
{
    _coordinate = coordinate;
}

const std::string& TileSoundEmitter::getSoundName() const
{
    return _soundName;
}

void TileSoundEmitter::setSoundName(std::string&& soundName)
{
    _soundName = std::move(soundName);
}

void TileSoundEmitter::setSoundName(const std::string& soundName)
{
    _soundName = soundName;
}
