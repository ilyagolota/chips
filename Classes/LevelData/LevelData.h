#ifndef  _CHIPS_CHALLENGE_LEVEL_DATA_H_
#define  _CHIPS_CHALLENGE_LEVEL_DATA_H_

#include <string>
#include <vector>
#include <cocos2d.h>
#include "TileType.h"

struct LevelWire
{
    cocos2d::Vec2 startCoordinate;
    cocos2d::Vec2 endCoordinate;
};

class LevelData : public cocos2d::Ref
{
protected:
    std::string _title;
    std::string _hint;
    std::string _password;
    int _number;
    int _time;
    int _chipsRequired;
    std::vector<std::vector<TileType>> _layers;
    std::vector<LevelWire> _wires;
    
public:
    static const int LAYER_SIZE;
    
    static LevelData* create();
    
    LevelData();
    const size_t getWidth() const { return 32; }
    const size_t getHeight() const { return 32; }
    const std::string& getTitle() const;
    void setTitle(const std::string& title);
    const std::string& getHint() const;
    void setHint(const std::string& hint);
    const std::string& getPassword() const;
    void setPassword(const std::string& password);
    int getNumber() const;
    void setNumber(int number);
    int getTime() const;
    void setTime(int time);
    int getChipsRequired() const;
    void setChipsRequired(int chipsRequired);
    std::vector<std::vector<TileType>>& getLayers();
    std::vector<LevelWire>& getWires();
    const std::vector<LevelWire>& getWires() const;
};

#endif
