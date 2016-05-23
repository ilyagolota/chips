#ifndef _CHIPS_CHALLENGE_TILED_SOUND_ENVIRONMENT_H_
#define _CHIPS_CHALLENGE_TILED_SOUND_ENVIRONMENT_H_

#include <vector>
#include <string>
#include <unordered_map>
#include <cocos2d.h>
#include "AudioEngine.h"

class TiledPhysicsWorld;
class TileSoundEmitter;
class TileMicrophone;

class TiledSoundEnvironment : public cocos2d::Ref
{
private:
    TiledPhysicsWorld* _physicsWorld;
    cocos2d::Vector<TileSoundEmitter*> _emitters;
    TileMicrophone* _microphone;
    std::unordered_map<std::string, int> _playingSounds;
    int _physicsLayerMask;
    
public:
    static TiledSoundEnvironment* create(TiledPhysicsWorld* physicsWorld);
    
    TiledSoundEnvironment(TiledPhysicsWorld* physicsWorld);
    virtual ~TiledSoundEnvironment();
    TileMicrophone* getMicrophone() const;
    void setMicrophone(TileMicrophone* microphone);
    int getPhysicsLayerMask() const;
    void setPhysicsLayerMask(int physicsLayerMask);
    void addEmitter(TileSoundEmitter* emitter);
    void clearEmitters();
    void removeEmitter(TileSoundEmitter* emitter);
    void update(float dt);
    
protected:
    std::vector<int> _buildDistanceMap() const;
    std::unordered_map<std::string, float> _aggregateVolumes(const std::vector<int>& distanceMap) const;
    void _setSoundVolumes(const std::unordered_map<std::string, float>& soundVolumes);
};

#endif
