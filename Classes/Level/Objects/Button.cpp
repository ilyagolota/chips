#include "Button.h"
#include <Level/Level.h>
#include "SwitchWall.h"
#include "Beartrap.h"
#include "CloneMachine.h"

Button* Button::create(Level* level, const cocos2d::Vec2& coordinate, TileType type)
{
	auto instance = new Button(level, coordinate, type);
    instance->autorelease();
    return instance;
}

Button::Button(Level* level, const cocos2d::Vec2& coordinate, TileType type) : LevelObject(level, coordinate)
{
	_type = type;
    
    _floor = cocos2d::Sprite::createWithSpriteFrameName("button-floor.png");
    _floor->setAnchorPoint(cocos2d::Vec2::ZERO);
    _floor->setPosition(_level->getProjector()->coordinateToPoint(_coordinate) + cocos2d::Vec2(0, -12));
    _floor->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate));
    _level->getStage()->addChild(_floor);
    
    _button = cocos2d::Sprite::createWithSpriteFrameName("button-floor.png");
    _button->setAnchorPoint(cocos2d::Vec2::ZERO);
    _button->setPosition(cocos2d::Vec2::ZERO);
    _floor->addChild(_button);
}

void Button::beforeEnter(Creature *creature)
{
    auto duration = creature->getTurnsPerMove() * _level->getTurnDuration();
    _button->runAction(cocos2d::Sequence::create(
        cocos2d::DelayTime::create(0.75f * duration),
        cocos2d::CallFunc::create([this]() {
            _button->setSpriteFrame("button-" + getColorName() + "-on.png");
        }),
        nullptr
    ));
}

void Button::afterEnter(Creature* creature)
{
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
        for (auto& wire : _level->getConfig()->getWires())
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
    auto duration = creature->getTurnsPerMove() * _level->getTurnDuration();
    _button->runAction(cocos2d::Sequence::create(
        cocos2d::DelayTime::create(0.25f * duration),
        cocos2d::CallFunc::create([this]() {
            _button->setSpriteFrame("button-" + getColorName() + ".png");
        }),
        nullptr
    ));
    
    CC_UNUSED_PARAM(creature);
    
    if (_type == TileType::BUTTON_BROWN)
    {
        const LevelWire* thisWire = nullptr;
        for (auto& wire : _level->getConfig()->getWires())
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

void Button::reset()
{
    _button->stopAllActions();
    _button->setSpriteFrame("button-" + getColorName() + ".png");
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

void Button::_toggleSwitchWalls()
{
    for (int x = 0, width = _level->getConfig()->getWidth(), height = _level->getConfig()->getHeight(); x < width; x++)
    {
        for (int y = 0; y < height; y++)
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
    
    cocos2d::experimental::AudioEngine::play2d("sounds/door.mp3");
}

void Button::_turnTanks()
{
    for (auto creature : _level->getCreatures())
    {
        if (creature->getType() == CreatureType::TANK)
        {
            creature->setDirection(inverse(creature->getDirection()));
        }
    }
}
