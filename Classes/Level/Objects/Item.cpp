#include "Item.h"
#include "../Level.h"

Item* Item::create(Level* level, const cocos2d::Vec2& coordinate, TileType type)
{
    auto instance = new Item(level, coordinate, type);
    instance->autorelease();
    return instance;
}

Item::Item(Level* level, const cocos2d::Vec2& coordinate, TileType type) : LevelObject(level, coordinate)
{
    _type = type;
    _picked = false;
}

void Item::onAdd()
{
    _node = cocos2d::Sprite::createWithSpriteFrameName(getSpriteFrameName());
    _node->setAnchorPoint(cocos2d::Vec2::ZERO);
    _node->setPosition(_level->getProjector()->coordinateToPoint(_coordinate));
    _node->setLocalZOrder(_level->getProjector()->coordinateToZOrder(_coordinate) + Level::ITEM_Z_ORDER);
    _level->getStage()->addChild(_node);
    
    _highlightNode = cocos2d::Sprite::createWithSpriteFrameName("highlight.png");
    _highlightNode->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
    _highlightNode->setPosition(cocos2d::Vec2(_node->getContentSize()) * 0.5f);
    _highlightNode->setLocalZOrder(-1);
    _highlightNode->runAction(cocos2d::RepeatForever::create(
        cocos2d::Sequence::create(
            cocos2d::ScaleTo::create(0.65f, 0.75f),
            cocos2d::ScaleTo::create(0.65f, 1.0f),
            nullptr
        )
    ));
    _node->addChild(_highlightNode);
}

void Item::reset()
{
    _picked = false;
    _node->setVisible(true);
}

bool Item::isEnterableBy(const Creature *creature, Direction /*direction*/) const
{
    return _picked || _type == TileType::KEY_BLUE || creature->getType() == CreatureType::CHIP;
}

void Item::afterEnter(Creature* creature)
{
    if (_picked)
    {
        return;
    }
    
    _picked = true;
    
    _node->setVisible(false);
    _highlightNode->stopAllActions();
    
    if (creature->getType() == CreatureType::CHIP)
    {
        _level->getInventory()->addItem(_type, 1);
    }
    
    auto bonusSprite = cocos2d::Sprite::createWithSpriteFrameName(getSpriteFrameName());
    bonusSprite->setPosition(_node->getPosition() + cocos2d::Vec2(0.5f * _node->getContentSize().width, 128));
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
        cocos2d::CallFuncN::create([](cocos2d::Node* bonusSprite){
            bonusSprite->removeFromParent();
        }),
        nullptr
    ));
}

std::string& Item::getSpriteFrameName() const
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
