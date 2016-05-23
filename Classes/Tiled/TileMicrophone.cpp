#include "TileMicrophone.h"

TileMicrophone* TileMicrophone::create()
{
    auto instance = new TileMicrophone();
    instance->autorelease();
    return instance;
}

TileMicrophone::TileMicrophone()
: _maxDistance(5)
{ }

cocos2d::Vec2 TileMicrophone::getCoordinate() const
{
    return _coordinate;
}

void TileMicrophone::setCoordinate(const cocos2d::Vec2& coordinate)
{
    _coordinate = coordinate;
}

int TileMicrophone::getMaxDistance() const
{
    return _maxDistance;
}

void TileMicrophone::setMaxDistance(int maxDistance)
{
    _maxDistance = maxDistance;
}
