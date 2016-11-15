#include "ButtonControlLayer.h"

ButtonControlLayer* ButtonControlLayer::create()
{
    auto instance = new ButtonControlLayer();
    instance->autorelease();
    return instance;
}

ButtonControlLayer::ButtonControlLayer()
{
    Layer::init();
    
    auto director = cocos2d::Director::getInstance();
    auto winSize = director->getWinSize();
    cocos2d::Rect screenBounds(director->getVisibleOrigin(), director->getVisibleSize());
    
    _menu = cocos2d::Menu::create();
    _menu->setAnchorPoint(cocos2d::Vec2::ZERO);
    _menu->setPosition(cocos2d::Vec2::ZERO);
    addChild(_menu);
    
    auto southItem = cocos2d::MenuItemSprite::create(cocos2d::Sprite::createWithSpriteFrameName("glider-stay-east.png"), cocos2d::Sprite::createWithSpriteFrameName("glider-stay-east.png"));
    southItem->setScale(-1, 1);
    southItem->setTag(static_cast<int>(Direction::SOUTH));
    southItem->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
    southItem->setPosition(cocos2d::Vec2(screenBounds.origin.x + 95, screenBounds.origin.y + 100));
    _menu->addChild(southItem);
    
    auto westItem = cocos2d::MenuItemSprite::create(cocos2d::Sprite::createWithSpriteFrameName("glider-stay-west.png"), cocos2d::Sprite::createWithSpriteFrameName("glider-stay-west.png"));
    westItem->setTag(static_cast<int>(Direction::WEST));
    westItem->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
    westItem->setPosition(cocos2d::Vec2(screenBounds.origin.x + 95, screenBounds.origin.y + 228));
    _menu->addChild(westItem);
    
    auto northItem = cocos2d::MenuItemSprite::create(cocos2d::Sprite::createWithSpriteFrameName("glider-stay-west.png"), cocos2d::Sprite::createWithSpriteFrameName("glider-stay-west.png"));
    northItem->setScale(-1, 1);
    northItem->setTag(static_cast<int>(Direction::SOUTH));
    northItem->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
    northItem->setPosition(cocos2d::Vec2(screenBounds.origin.x + screenBounds.size.width - 95, screenBounds.origin.y + 228));
    _menu->addChild(northItem);
    
    auto eastItem = cocos2d::MenuItemSprite::create(cocos2d::Sprite::createWithSpriteFrameName("glider-stay-east.png"), cocos2d::Sprite::createWithSpriteFrameName("glider-stay-east.png"));
    eastItem->setTag(static_cast<int>(Direction::EAST));
    eastItem->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
    eastItem->setPosition(cocos2d::Vec2(screenBounds.origin.x + screenBounds.size.width - 95, screenBounds.origin.y + 100));
    _menu->addChild(eastItem);
}

bool ButtonControlLayer::isPressed()
{
    for (auto child : _menu->getChildren())
    {
        auto menuItem = dynamic_cast<cocos2d::MenuItem*>(child);
        if (menuItem != nullptr)
        {
            if (menuItem->isSelected())
            {
                return true;
            }
        }
    }
    return false;
}

Direction ButtonControlLayer::getSelectedDirection()
{
    for (auto child : _menu->getChildren())
    {
        auto menuItem = dynamic_cast<cocos2d::MenuItem*>(child);
        if (menuItem != nullptr)
        {
            if (menuItem->isSelected())
            {
                return static_cast<Direction>(menuItem->getTag());
            }
        }
    }
    return Direction::NORTH;
}
