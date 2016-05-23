#include "SwitchWall.h"

SwitchWall* SwitchWall::create(const cocos2d::Vec2& coordinate, bool initiallyOpen)
{
    auto instance = new SwitchWall(coordinate, initiallyOpen);
    instance->autorelease();
    return instance;
}

SwitchWall::SwitchWall(const cocos2d::Vec2& coordinate, bool initiallyOpen) : LevelObject(coordinate)
{
    _open = _initiallyOpen = initiallyOpen;

    /*auto sprite = cocos2d::Sprite::createWithSpriteFrameName("toggle-floor.png");
    _sprites->pushBack(sprite);
    
    _doorSprite = cocos2d::Sprite::createWithSpriteFrameName("toggle-wall.png");
    _doorSprite->setAnchorPoint(cocos2d::Vec2::ZERO);
    _doorSprite->setPosition(cocos2d::Vec2::ZERO);
    _doorSprite->setVisible(!_open);
    sprite->addChild(_doorSprite);
    
    */
}

bool SwitchWall::isOpen()
{
    return _open;
}

void SwitchWall::open()
{
    if (!_open)
    {
        _open = true;
        
        /*auto animation = cocos2d::AnimationCache::getInstance()->getAnimation("door-" + getColorName() + "-open");
        
        Vector<FiniteTimeAction*> actions;
        actions.pushBack(cocos2d::Animate::create(animation));
        actions.pushBack();
        
        _sprites[0]->runAction(cocos2d::Sequence::create(actions));*/
    }
}

void SwitchWall::close()
{
    if (_open)
    {
        _open = false;
        
        /*auto animation = cocos2d::AnimationCache::getInstance()->getAnimation("door-" + getColorName() + "-open");
        
        Vector<FiniteTimeAction*> actions;
        actions.pushBack(cocos2d::Animate::create(animation));
        actions.pushBack();
        
        _sprites[0]->runAction(cocos2d::Sequence::create(actions));*/
    }
}

/*
ToggleWall::reset()
{
    _open = _initiallyOpen;
    _doorSprite->setVisible(!_open);
}*/
