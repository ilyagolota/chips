#include "ButtonControlLayer.h"

ButtonControlLayer* ButtonControlLayer::create(Level* level)
{
    auto instance = new ButtonControlLayer(level);
    instance->autorelease();
    return instance;
}

ButtonControlLayer::ButtonControlLayer(Level* level)
{
    Layer::init();
    
    _level = level;
    _selectedItem = nullptr;
    _selectedWithCamera = nullptr;
    memset(_states, 0, sizeof(ButtonState) * 4);
    
    auto director = cocos2d::Director::getInstance();
    auto winSize = director->getWinSize();
    cocos2d::Rect screenBounds(director->getVisibleOrigin(), director->getVisibleSize());
    
    auto southItem = cocos2d::MenuItemSprite::create(cocos2d::Sprite::createWithSpriteFrameName("glider-stay-east.png"), cocos2d::Sprite::createWithSpriteFrameName("glider-stay-east.png"));
    southItem->setScale(-1, 1);
    southItem->setTag(static_cast<int>(Direction::SOUTH));
    southItem->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
    southItem->setPosition(cocos2d::Vec2(screenBounds.origin.x + 95, screenBounds.origin.y + 100));
    addChild(southItem);
    
    auto westItem = cocos2d::MenuItemSprite::create(cocos2d::Sprite::createWithSpriteFrameName("glider-stay-west.png"), cocos2d::Sprite::createWithSpriteFrameName("glider-stay-west.png"));
    westItem->setTag(static_cast<int>(Direction::WEST));
    westItem->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
    westItem->setPosition(cocos2d::Vec2(screenBounds.origin.x + 95, screenBounds.origin.y + 228));
    addChild(westItem);
    
    auto northItem = cocos2d::MenuItemSprite::create(cocos2d::Sprite::createWithSpriteFrameName("glider-stay-west.png"), cocos2d::Sprite::createWithSpriteFrameName("glider-stay-west.png"));
    northItem->setScale(-1, 1);
    northItem->setTag(static_cast<int>(Direction::NORTH));
    northItem->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
    northItem->setPosition(cocos2d::Vec2(screenBounds.origin.x + screenBounds.size.width - 95, screenBounds.origin.y + 228));
    addChild(northItem);
    
    auto eastItem = cocos2d::MenuItemSprite::create(cocos2d::Sprite::createWithSpriteFrameName("glider-stay-east.png"), cocos2d::Sprite::createWithSpriteFrameName("glider-stay-east.png"));
    eastItem->setTag(static_cast<int>(Direction::EAST));
    eastItem->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
    eastItem->setPosition(cocos2d::Vec2(screenBounds.origin.x + screenBounds.size.width - 95, screenBounds.origin.y + 100));
    addChild(eastItem);
    
    auto touchListener = cocos2d::EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(ButtonControlLayer::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(ButtonControlLayer::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(ButtonControlLayer::onTouchEnded, this);
    touchListener->onTouchCancelled = CC_CALLBACK_2(ButtonControlLayer::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void ButtonControlLayer::onLevelTurn()
{
    for (size_t i = 0; i < 4; i++)
    {
        if (_states[i] != NONE)
        {
            auto playerCreature = _level->getPlayerCreature();
            if (playerCreature != nullptr)
            {
                if (playerCreature->canMove(static_cast<Direction>(i)))
                {
                    playerCreature->move(static_cast<Direction>(i));
                }
            }
            
            if (_states[i] == PRESSED)
            {
                _states[i] = APPLIED_IN_TURN;
            }
            else if (_states[i] == RELEASED_BEFORE_TURN)
            {
                _states[i] = NONE;
            }
            
            break;
        }
    }
}

void ButtonControlLayer::onButtonPressed(int directionTag)
{
    for (size_t i = 0; i < 4; i++)
    {
        if (_states[i] == RELEASED_BEFORE_TURN)
        {
            _states[i] = NONE;
        }
    }
    _states[directionTag] = PRESSED;
}

void ButtonControlLayer::onButtonReleased(int directionTag)
{
    if (_states[directionTag] == PRESSED)
    {
        _states[directionTag] = RELEASED_BEFORE_TURN;
    }
    else
    {
        _states[directionTag] = NONE;
    }
}

bool ButtonControlLayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    auto camera = cocos2d::Camera::getVisitingCamera();
    if (_selectedItem != nullptr || !_visible || !camera)
    {
        return false;
    }
    
    for (Node *c = this->_parent; c != nullptr; c = c->getParent())
    {
        if (c->isVisible() == false)
        {
            return false;
        }
    }
    
    _selectedItem = this->getItemForTouch(touch, camera);
    if (_selectedItem)
    {
        _selectedWithCamera = camera;
        _selectedItem->selected();
        onButtonPressed(_selectedItem->getTag());
        return true;
    }
    
    return false;
}

void ButtonControlLayer::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
    this->retain();
    if (_selectedItem)
    {
        onButtonReleased(_selectedItem->getTag());
        _selectedItem->unselected();
        _selectedItem = nullptr;
    }
    _selectedWithCamera = nullptr;
    this->release();
}

void ButtonControlLayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
    auto currentItem = this->getItemForTouch(touch, _selectedWithCamera);
    if (currentItem != _selectedItem)
    {
        if (_selectedItem != nullptr)
        {
            onButtonReleased(_selectedItem->getTag());
            _selectedItem->unselected();
        }
        _selectedItem = currentItem;
        if (_selectedItem != nullptr)
        {
            _selectedItem->selected();
            onButtonPressed(_selectedItem->getTag());
        }
    }
}

void ButtonControlLayer::onExit()
{
    if (_selectedItem)
    {
        onButtonReleased(_selectedItem->getTag());
        _selectedItem->unselected();
        _selectedItem = nullptr;
    }
    Layer::onExit();
}

cocos2d::MenuItem* ButtonControlLayer::getItemForTouch(cocos2d::Touch *touch, const cocos2d::Camera *camera)
{
    cocos2d::Vec2 touchLocation = touch->getLocation();
    for (const auto &item: _children)
    {
        auto child = dynamic_cast<cocos2d::MenuItem*>(item);
        if (child == nullptr || !child->isVisible() || !child->isEnabled())
        {
            continue;
        }
        cocos2d::Rect rect;
        rect.size = child->getContentSize();
        if (isScreenPointInRect(touchLocation, camera, child->getWorldToNodeTransform(), rect, nullptr))
        {
            return child;
        }
    }
    return nullptr;
}
