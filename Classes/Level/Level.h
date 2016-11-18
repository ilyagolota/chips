#ifndef _CHIPS_CHALLENGE_LEVEL_H_
#define _CHIPS_CHALLENGE_LEVEL_H_

#include <cocos2d.h>
#include <vector>

#include <Tiled/Direction.h>
#include <Tiled/TiledPhysicsWorld.h>
#include <Tiled/TiledProjector.h>
#include <Tiled/TiledSoundEnvironment.h>
#include <Tiled/TileSoundEmitter.h>

#include "TileType.h"
#include "LevelConfig.h"
#include "Inventory.h"
#include "Creature.h"
#include "Objects/LevelObject.h"
#include "Objects/Item.h"
#include "LevelHandler.h"

class IPlayerControl;

class Level : public cocos2d::Ref
{
public:
    enum
    {
        BACK_Z_ORDER,
        ITEM_Z_ORDER,
        CREATURE_SMALL_Z_ORDER,
        CREATURE_MEDIUM_Z_ORDER,
        CREATURE_BIG_Z_ORDER,
        WALL_Z_ORDER,
        Z_ORDER_PER_TILE,
        FRONT_Z_ORDER,
    };
    
    static Level* create(cocos2d::Node* stage, LevelHandler* handler);
    
    Level(cocos2d::Node* stage, LevelHandler* handler);
    ~Level();
    
    LevelConfig* getConfig() const;
    void start(LevelConfig* config);
    void restart();
    void win();
    void fail(const std::string& message);
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
    const cocos2d::Vector<Creature*>& getCreatures() const;
    Creature* getCreatureAt(const cocos2d::Vec2& coordinate) const;
	Creature* getPlayerCreature() const;
    
	int getWallShape(const cocos2d::Vec2& coordinate) const;

protected:
    void _rebuild();
    void _reset();
	bool _tryBuildTile(TileType tileType, const cocos2d::Vec2& coordinate);
	bool _tryBuildCreature(TileType tileType, const cocos2d::Vec2& coordinate);
    void _buildFloor(const cocos2d::Vec2& coordinate);
	void _buildWall(const cocos2d::Vec2& coordinate);
	void _buildGravel(const cocos2d::Vec2& coordinate);
    size_t _coordinateToIndex(const cocos2d::Vec2& coordinate) const;
    
private:
    LevelConfig* _config;
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
    
    LevelHandler* _handler;
};

#endif
