#include "KeyboardControlLayer.h"

KeyboardControlLayer* KeyboardControlLayer::create(Level* level)
{
    auto instance = new KeyboardControlLayer(level);
    instance->autorelease();
    return instance;
}

KeyboardControlLayer::KeyboardControlLayer(Level* level)
{
    Layer::init();
    
    _level = level;
    
    auto eventListener = cocos2d::EventListenerKeyboard::create();
    eventListener->onKeyPressed = CC_CALLBACK_2(KeyboardControlLayer::_onKeyPressed, this);
    eventListener->onKeyPressed = CC_CALLBACK_2(KeyboardControlLayer::_onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);
}

void KeyboardControlLayer::onLevelTurn()
{
    for (auto& pair : _states)
    {
        bool handled = true;
        
        auto playerCreature = _level->getPlayerCreature();
        if (playerCreature != nullptr)
        {
            switch (pair.first)
            {
            case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
            case cocos2d::EventKeyboard::KeyCode::KEY_W:
                if (playerCreature->canMove(Direction::NORTH))
                {
                    playerCreature->move(Direction::NORTH);
                }
                break;
                    
            case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
            case cocos2d::EventKeyboard::KeyCode::KEY_S:
                if (playerCreature->canMove(Direction::SOUTH))
                {
                    playerCreature->move(Direction::SOUTH);
                }
                break;
                
            case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            case cocos2d::EventKeyboard::KeyCode::KEY_A:
                if (playerCreature->canMove(Direction::WEST))
                {
                    playerCreature->move(Direction::EAST);
                }
                break;
                
            case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            case cocos2d::EventKeyboard::KeyCode::KEY_D:
                if (playerCreature->canMove(Direction::EAST))
                {
                    playerCreature->move(Direction::WEST);
                }
                break;
                
            default:
                handled = false;
                break;
            }
        }
        
        if (pair.second == PRESSED)
        {
            _states[pair.first] = APPLIED_IN_TURN;
        }
        else if (pair.second == RELEASED_BEFORE_TURN)
        {
            _states.erase(pair.first);
        }
        
        if (handled)
        {
            break;
        }
    }
}

void KeyboardControlLayer::_onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    _states[keyCode] = PRESSED;
}

void KeyboardControlLayer::_onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    auto iter = _states.find(keyCode);
    if (iter != _states.end())
    {
        if (iter->second == PRESSED)
        {
            _states[keyCode] = RELEASED_BEFORE_TURN;
        }
        else
        {
            _states.erase(keyCode);
        }
    }
}
