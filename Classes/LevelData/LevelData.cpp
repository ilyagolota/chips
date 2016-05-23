#include "LevelData.h"

const int LevelData::LAYER_SIZE = 1024;

LevelData* LevelData::create()
{
    auto instance = new (std::nothrow) LevelData();
    instance->autorelease();
    return instance;
}

LevelData::LevelData()
:_title(), _hint(), _password(), _number(0), _time(0), _chipsRequired(0), _wires()
{ }

const std::string& LevelData::getTitle() const
{
    return _title;
}

void LevelData::setTitle(const std::string& title)
{
    _title = title;
}

const std::string& LevelData::getHint() const
{
    return _hint;
}

void LevelData::setHint(const std::string& hint)
{
    _hint = hint;
}

const std::string& LevelData::getPassword() const
{
    return _password;
}

void LevelData::setPassword(const std::string& password)
{
    _password = password;
}

int LevelData::getNumber() const
{
    return _number;
}

void LevelData::setNumber(int number)
{
    _number = number;
}

int LevelData::getTime() const
{
    return _time;
}

void LevelData::setTime(int time)
{
    _time = time;
}

int LevelData::getChipsRequired() const
{
    return _chipsRequired;
}

void LevelData::setChipsRequired(int chipsRequired)
{
    _chipsRequired = chipsRequired;
}

std::vector<std::vector<TileType>>& LevelData::getLayers()
{
    return _layers;
}

std::vector<LevelWire>& LevelData::getWires()
{
    return _wires;
}

const std::vector<LevelWire>& LevelData::getWires() const
{
    return _wires;
}
