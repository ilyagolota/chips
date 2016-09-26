#include "Item.h"
#include <cocos2d.h>
#include <Tiled/TiledProjector.h>
#include "Level.h"
#include "Creature.h"
#include "Inventory.h"

Item* Item::create(const cocos2d::Vec2& coordinate, TileType type)
{
    auto instance = new Item(coordinate, type);
    instance->autorelease();
    return instance;
}

Item::Item(const cocos2d::Vec2& coordinate, TileType type)
{
    _level = nullptr;
    _coordinate = coordinate;
    _type = type;
    _picked = false;

    _sprite = cocos2d::Sprite::createWithSpriteFrameName(_getSpriteFrameName());
    _sprite->setAnchorPoint(cocos2d::Vec2::ZERO);
    _sprite->retain();

	_highlight = cocos2d::Sprite::createWithSpriteFrameName("highlight.png");
	_highlight->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
	_highlight->setPosition(cocos2d::Vec2(_sprite->getContentSize()) * 0.5f);
	_highlight->setLocalZOrder(-1);
	_highlight->runAction(cocos2d::RepeatForever::create(
		cocos2d::Sequence::create(
			cocos2d::ScaleTo::create(0.65f, 0.75f),
			cocos2d::ScaleTo::create(0.65f, 1.0f),
			nullptr
		)
	));
	_sprite->addChild(_highlight);
}

Item::~Item()
{
	_sprite->release();
}

cocos2d::Vec2 Item::getCoordinate() const
{
    return _coordinate;
}

Level* Item::getLevel() const
{
    return _level;
}

void Item::setLevel(Level* level)
{
    if (_level != level)
    {
        if (_level != nullptr)
        {
            _level->getStage()->removeChild(_sprite);
        }
        _level = level;
        if (_level != nullptr)
        {
            _sprite->setPosition(_level->getProjector()->coordinateToPoint(_coordinate));
            _sprite->setZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::ITEM_Z_ORDER);
            _level->getStage()->addChild(_sprite);
        }
    }
}

bool Item::isPickableBy(const Creature* creature) const
{
    return _picked || _type == TileType::KEY_BLUE || creature->getType() == CreatureType::CHIP;
}

void Item::pick(Creature* creature)
{
    if (!_picked)
    {
        _picked = true;

		_sprite->setVisible(false);
		_highlight->stopAllActions();

        if (creature->getType() == CreatureType::CHIP)
        {
            _level->getInventory()->addItem(_type, 1);
        }
        
		auto bonusSprite = cocos2d::Sprite::createWithSpriteFrameName(_getSpriteFrameName());
		bonusSprite->setPosition(_sprite->getPosition() + cocos2d::Vec2(0.5f * _sprite->getContentSize().width, 128));
		bonusSprite->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
		bonusSprite->setOpacity(192);
		bonusSprite->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::WALL_Z_ORDER);
		_level->getStage()->addChild(bonusSprite);

        auto duration = 1.2f * _level->getTurnDuration();
		bonusSprite->runAction(cocos2d::Sequence::create(
			cocos2d::Spawn::create(
				cocos2d::MoveBy::create(duration, cocos2d::Vec2(0, 44)),
				cocos2d::FadeTo::create(duration, 0),
				cocos2d::ScaleTo::create(duration, 1.5f),
				cocos2d::RotateBy::create(duration, 180.0f),
				nullptr
			),
			cocos2d::CallFunc::create([bonusSprite](){
				bonusSprite->removeFromParent();
			}),
			nullptr
		));
    }
}

std::string& Item::_getSpriteFrameName() const
{
    static std::string names[] =
    {
        "boots-water.png",
		"boots-fire.png",
		"boots-slide.png",
		"boots-ice.png",
		"ic-chip.png",
		"key-blue.png",
		"key-red.png",
		"key-green.png",
		"key-yellow.png"
    };
	switch (_type)
	{
	case TileType::BOOTS_WATER:
		return names[0];
	case TileType::BOOTS_FIRE:
		return names[1];
	case TileType::BOOTS_SLIDE:
		return names[2];
	case TileType::BOOTS_ICE:
		return names[3];
	case TileType::IC_CHIP:
		return names[4];
	case TileType::KEY_BLUE:
		return names[5];
	case TileType::KEY_RED:
		return names[6];
	case TileType::KEY_GREEN:
		return names[7];
	case TileType::KEY_YELLOW:
		return names[8];
	default:
		return names[0];
	}
}
