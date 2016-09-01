#include "TiledSoundEnvironment.h"
#include "TiledPhysicsWorld.h"
#include "TileSoundEmitter.h"
#include "TileMicrophone.h"

TiledSoundEnvironment* TiledSoundEnvironment::create(TiledPhysicsWorld* physicsWorld)
{
    auto instance = new TiledSoundEnvironment(physicsWorld);
    instance->autorelease();
    return instance;
}

TiledSoundEnvironment::TiledSoundEnvironment(TiledPhysicsWorld* physicsWorld)
{
    _physicsWorld = physicsWorld;
    _physicsWorld->retain();
    
    _microphone = TileMicrophone::create();
    _microphone->retain();
}

TiledSoundEnvironment::~TiledSoundEnvironment()
{
    for (auto& pair : _playingSounds)
    {
        cocos2d::experimental::AudioEngine::stop(pair.second);
    }
}

TileMicrophone* TiledSoundEnvironment::getMicrophone() const
{
    return _microphone;
}

void TiledSoundEnvironment::setMicrophone(TileMicrophone* microphone)
{
    if (microphone != _microphone)
    {
        if (_microphone != nullptr)
        {
            _microphone->release();
        }
        _microphone = microphone;
        if (_microphone != nullptr)
        {
            _microphone->retain();
        }
    }
}

int TiledSoundEnvironment::getPhysicsLayerMask() const
{
    return _physicsLayerMask;
}

void TiledSoundEnvironment::setPhysicsLayerMask(int physicsLayerMask)
{
    _physicsLayerMask = physicsLayerMask;
}

void TiledSoundEnvironment::addEmitter(TileSoundEmitter* emitter)
{
    _emitters.pushBack(emitter);
}

void TiledSoundEnvironment::removeEmitter(TileSoundEmitter* emitter)
{
    _emitters.eraseObject(emitter);
}

void TiledSoundEnvironment::clearEmitters()
{
    _emitters.clear();
}

void TiledSoundEnvironment::update(float dt)
{
    auto distanceMap = _buildDistanceMap();
    auto soundVolumes = _aggregateVolumes(distanceMap);
    _setSoundVolumes(soundVolumes);
}

std::vector<int> TiledSoundEnvironment::_buildDistanceMap() const
{
    auto maxDistance = _microphone->getMaxDistance();
    size_t mapSize = (maxDistance * 2 + 1) * (maxDistance * 2 + 1);
    std::vector<int> distanceMap(mapSize);
    std::fill(distanceMap.begin(), distanceMap.end(), maxDistance + 1);
    
    auto microphoneCoordinate = _microphone->getCoordinate();
    int startX = microphoneCoordinate.x - maxDistance;
    int startY = microphoneCoordinate.y - maxDistance;
    int endX = microphoneCoordinate.x + maxDistance + 1;
    int endY = microphoneCoordinate.y + maxDistance + 1;
    
    std::vector<cocos2d::Vec2> currentCoordinates;
    std::vector<cocos2d::Vec2> nextCoordinates;
    currentCoordinates.push_back(microphoneCoordinate);
    for (int distance = 1; distance <= maxDistance && !currentCoordinates.empty(); distance++)
    {
        for (auto& coordinate : currentCoordinates)
        {
            for (int dirIndex = 0; dirIndex < 4; dirIndex++)
            {
                auto direction = static_cast<Direction>(dirIndex);
                
                auto nextCoordinate = coordinate + toVec2(direction);
                if (nextCoordinate.x < startX || nextCoordinate.y < startY || nextCoordinate.x >= endX || nextCoordinate.y >= endY)
                {
                    continue;
                }
                
                int nextTileIndex = (nextCoordinate.x - startX) + (endX - startX) * (nextCoordinate.y - startY);
                if (distanceMap[nextTileIndex] < distance)
                {
                    continue;
                }
                
                if (_physicsWorld->rayCast(coordinate, direction, _physicsLayerMask))
                {
                    continue;
                }
                
                distanceMap[nextTileIndex] = distance;
                nextCoordinates.push_back(nextCoordinate);
            }
        }
        currentCoordinates.clear();
        currentCoordinates.insert(currentCoordinates.end(), nextCoordinates.begin(), nextCoordinates.end());
        nextCoordinates.clear();
    }
    
    return distanceMap;
}

std::unordered_map<std::string, float> TiledSoundEnvironment::_aggregateVolumes(const std::vector<int>& distanceMap) const
{
    auto maxDistance = _microphone->getMaxDistance();
    auto microphoneCoordinate = _microphone->getCoordinate();
    int startX = microphoneCoordinate.x - maxDistance;
    int startY = microphoneCoordinate.x - maxDistance;
    int endX = microphoneCoordinate.x + maxDistance + 1;
    int endY = microphoneCoordinate.y + maxDistance + 1;
    
    std::unordered_map<std::string, float> volumes;
    for (auto emitter : _emitters)
    {
        auto emitterCoordinate = emitter->getCoordinate();
        if (emitterCoordinate.x < startX || emitterCoordinate.y < startY || emitterCoordinate.x >= endX || emitterCoordinate.y >= endY)
        {
            continue;
        }
        
        int emitterTileIndex = (emitterCoordinate.x - startX) + (endX - startX) * (emitterCoordinate.y - startY);
        auto distance = distanceMap[emitterTileIndex];
        if (distance > maxDistance)
        {
            continue;
        }
        
        auto volume = 0.6f * (maxDistance - distance + 1) / (maxDistance + 1);
        
        auto volumeIter = volumes.find(emitter->getSoundName());
        if (volumeIter == volumes.end())
        {
            volumes[emitter->getSoundName()] = std::min(volume, 1.0f);
        }
        else
        {
            volumeIter->second = std::min(volume + volumeIter->second, 1.0f);
        }
    }
    return volumes;
}

void TiledSoundEnvironment::_setSoundVolumes(const std::unordered_map<std::string, float>& soundVolumes)
{
    for (auto& soundPair : _playingSounds)
    {
        auto volumeIter = soundVolumes.find(soundPair.first);
        if (volumeIter == soundVolumes.end() || volumeIter->second == 0)
        {
            auto state = cocos2d::experimental::AudioEngine::getState(soundPair.second);
            if (state == cocos2d::experimental::AudioEngine::AudioState::PLAYING)
            {
                cocos2d::experimental::AudioEngine::pause(soundPair.second);
            }
        }
    }

    for (auto& volumePair : soundVolumes)
    {
        auto& fileName = volumePair.first;
        auto volume = volumePair.second;
        
        if (volume != 0)
        {
            auto soundIter = _playingSounds.find(fileName);
            if (soundIter == _playingSounds.end())
            {
                _playingSounds[fileName] = cocos2d::experimental::AudioEngine::play2d(fileName, true, volume);
            }
            else
            {
                auto soundId = soundIter->second;
                float wasVolume = cocos2d::experimental::AudioEngine::getVolume(soundId);
                if (volume != wasVolume)
                {
                    cocos2d::experimental::AudioEngine::setVolume(soundId, volume);
                }
                
                auto state = cocos2d::experimental::AudioEngine::getState(soundId);
                if (state == cocos2d::experimental::AudioEngine::AudioState::PAUSED)
                {
                    cocos2d::experimental::AudioEngine::resume(soundId);
                }
            }
        }
    }
}
