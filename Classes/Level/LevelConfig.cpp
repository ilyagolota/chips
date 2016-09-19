#include "LevelConfig.h"

const int LevelConfig::LAYER_SIZE = 1024;

LevelConfig* LevelConfig::create()
{
    auto instance = new (std::nothrow) LevelConfig();
    instance->autorelease();
    return instance;
}

LevelConfig::LevelConfig()
:_title(), _hint(), _password(), _number(0), _time(0), _chipsRequired(0), _wires()
{ }

const std::string& LevelConfig::getTitle() const
{
    return _title;
}

void LevelConfig::setTitle(const std::string& title)
{
    _title = title;
}

const std::string& LevelConfig::getHint() const
{
    return _hint;
}

void LevelConfig::setHint(const std::string& hint)
{
    _hint = hint;
}

const std::string& LevelConfig::getPassword() const
{
    return _password;
}

void LevelConfig::setPassword(const std::string& password)
{
    _password = password;
}

int LevelConfig::getNumber() const
{
    return _number;
}

void LevelConfig::setNumber(int number)
{
    _number = number;
}

int LevelConfig::getTime() const
{
    return _time;
}

void LevelConfig::setTime(int time)
{
    _time = time;
}

int LevelConfig::getChipsRequired() const
{
    return _chipsRequired;
}

void LevelConfig::setChipsRequired(int chipsRequired)
{
    _chipsRequired = chipsRequired;
}

std::vector<std::vector<TileType>>& LevelConfig::getLayers()
{
    return _layers;
}

std::vector<LevelWire>& LevelConfig::getWires()
{
    return _wires;
}

const std::vector<LevelWire>& LevelConfig::getWires() const
{
    return _wires;
}
