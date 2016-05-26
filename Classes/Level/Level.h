#ifndef _CHIPS_CHALLENGE_LEVEL_H_
#define _CHIPS_CHALLENGE_LEVEL_H_

#include <cocos2d.h>
#include <vector>
#include "LevelData/TileType.h"

class LevelData;
class TiledPhysicsWorld;
class TiledSoundEnvironment;
class TiledProjector;
class Inventory;
class LevelObject;
class Item;
class Creature;

struct LevelWire;

class Level : public cocos2d::Ref
{
private:
    LevelData* _levelData;
    Inventory* _inventory;
    
    cocos2d::Node* _stage;
    TiledPhysicsWorld* _physicsWorld;
    TiledSoundEnvironment* _soundEnvironment;
    TiledProjector* _projector;
    
    float _timeMultiplier;
    
    std::vector<LevelObject*> _objects;
    std::vector<Item*> _items;
    cocos2d::Vector<Creature*> _creatures;
    Creature* _playerCreature;
    
public:
    enum
    {
        BACKGROUND_Z_ORDER,
        CREATURE_SMALL_Z_ORDER,
        CREATURE_MEDIUM_Z_ORDER,
        CREATURE_BIG_Z_ORDER,
        WALL_Z_ORDER,
        FRONT_Z_ORDER,
    };
    
    static Level* create(cocos2d::Node* stage);
    
    Level(cocos2d::Node* stage);
    ~Level();
    
    const LevelData* getLevelData() const;
    void start(LevelData* levelData);
    void restart();
    void makeTurn(float dt);
    
    cocos2d::Node* getStage() const;
    TiledPhysicsWorld* getPhysicsWorld() const;
    TiledSoundEnvironment* getSoundEnvironment() const;
    TiledProjector* getProjector() const;
    Inventory* getInventory() const;
    float getTurnDuration();
    
    void addObject(LevelObject* object);
    LevelObject* getObjectAt(const cocos2d::Vec2& coordinate) const;
    void addItem(Item* item);
    Item* getItemAt(const cocos2d::Vec2& coordinate) const;
    void addCreature(Creature* creature);
    void removeCreature(Creature* creature);
    Creature* getCreatureAt(const cocos2d::Vec2& coordinate) const;
	Creature* getPlayerCreature() const;

protected:
    void _rebuild();
    void _reset();
	bool _tryBuildTile(TileType tileType, const cocos2d::Vec2& coordinate);
	bool _tryBuildCreature(TileType tileType, const cocos2d::Vec2& coordinate);
    void _buildFloor(const cocos2d::Vec2& coordinate);
	void _buildWall(const cocos2d::Vec2& coordinate);
	void _buildGravel(const cocos2d::Vec2& coordinate);
    size_t _coordinateToIndex(const cocos2d::Vec2& coordinate) const;
};

#endif
