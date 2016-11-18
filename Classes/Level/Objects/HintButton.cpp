#include "HintButton.h"
#include <Tiled/TiledProjector.h>
#include "Level/Level.h"

HintButton* HintButton::create(Level* level, const cocos2d::Vec2& coordinate)
{
	auto instance = new HintButton(level, coordinate);
    instance->autorelease();
    return instance;
}

HintButton::HintButton(Level* level, const cocos2d::Vec2& coordinate) : LevelObject(level, coordinate)
{
}

void HintButton::onAdd()
{
	_node = cocos2d::Sprite::createWithSpriteFrameName("help.png");
	_node->setAnchorPoint(cocos2d::Vec2::ZERO);
	_node->setPosition(_level->getProjector()->coordinateToPoint(_coordinate) + cocos2d::Vec2(0, -12));
	_node->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::BACK_Z_ORDER);
	_level->getStage()->addChild(_node);
}

void HintButton::beforeEnter(Creature *creature)
{
    if (creature->getType() == CreatureType::CHIP)
    {
        
    }
}

void HintButton::beforeEscape(Creature *creature)
{
    if (creature->getType() == CreatureType::CHIP)
    {
        
    }
}
