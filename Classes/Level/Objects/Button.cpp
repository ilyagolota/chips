#include "Button.h"
#include "LevelData/LevelData.h"
#include "Level/Level.h"
#include "Level/Creature.h"
#include "Beartrap.h"
#include "CloneMachine.h"

Button* Button::create(const cocos2d::Vec2& coordinate, TileType type)
{
	auto instance = new Button(coordinate, type);
    instance->autorelease();
    return instance;
}

Button::Button(const cocos2d::Vec2& coordinate, TileType type) : LevelObject(coordinate)
{
	_type = type;
}

std::string& Button::getColorName()
{
    static std::string names[] = { "green", "red", "brown", "blue" };
    if (_type == TileType::BUTTON_GREEN)
    {
        return names[0];
    }
    else if (_type == TileType::BUTTON_RED)
    {
        return names[1];
    }
    else if (_type == TileType::BUTTON_BROWN)
    {
        return names[2];
    }
    else
    {
        return names[3];
    }
}

void Button::afterEnter(Creature* creature)
{
    //_buttonSprite->setSpriteFrame("button-" + getColorName() + "-2.png");
    
    if (_type == TileType::BUTTON_GREEN)
    {
        _toggleSwitchWalls();
    }
    else if (_type == TileType::BUTTON_BLUE)
    {
        _turnTanks();
    }
    else
    {
        const LevelWire* thisWire = nullptr;
        for (auto& wire : _level->getLevelData()->getWires())
        {
            if (wire.startCoordinate == _coordinate)
            {
                thisWire = &wire;
            }
        }
        
        if (thisWire != nullptr)
        {
            auto object = _level->getObjectAt(thisWire->endCoordinate);
            if (_type == TileType::BUTTON_RED)
            {
                auto cloneMachine = dynamic_cast<CloneMachine*>(object);
                if (cloneMachine != nullptr)
                {
                    cloneMachine->performCloning();
                }
            }
            else if (_type == TileType::BUTTON_BROWN)
            {
                auto beartrap = dynamic_cast<Beartrap*>(object);
                if (beartrap != nullptr)
                {
                    beartrap->setOpen(true);
                }
            }
        }
    }
}

void Button::beforeEscape(Creature* creature)
{
    CC_UNUSED_PARAM(creature);
    
    if (_type == TileType::BUTTON_BROWN)
    {
        const LevelWire* thisWire = nullptr;
        for (auto& wire : _level->getLevelData()->getWires())
        {
            if (wire.startCoordinate == _coordinate)
            {
                thisWire = &wire;
            }
        }
        
        if (thisWire != nullptr)
        {
            auto beartrap = dynamic_cast<Beartrap*>(_level->getObjectAt(thisWire->endCoordinate));
            if (beartrap != nullptr)
            {
                beartrap->setOpen(false);
            }
        }
    }
}

void Button::_toggleSwitchWalls()
{
    /*for (int x = 0, width = _level->getWidth(), height = _level->getHeight(); x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            if (_level->getLayer1()[x][y] == TileType::SWITCH_WALL_CLOSED)
            {
                auto wall = dynamic_cast<SwitchWall*>(_level->getObjectAt(cocos2d::Vec2(x, y)));
                if (wall != nullptr)
                {
                    if (wall->isOpen())
                    {
                        wall->close();
                    }
                    else
                    {
                        wall->open();
                    }
                }
            }
        }
    }*/
}

void Button::_turnTanks()
{
    /*for (auto creature : _level->getCreatures())
    {
        if (creature->getType() == CreatureType::TANK)
        {
            creature->setDirection(inverse(creature->getDirection()));
        }
    }*/
}
