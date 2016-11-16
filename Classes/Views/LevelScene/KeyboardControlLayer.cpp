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
    auto playerCreature = _level->getPlayerCreature();
    if (playerCreature)
    {
        for (size_t i = 0; i < 4; i++)
        {
            if (_directions[i])
            {
                if (playerCreature->canMove(static_cast<Direction>(i)))
                {
                    playerCreature->move(static_cast<Direction>(i));
                    break;
                }
            }
        }
    }
}

void KeyboardControlLayer::_onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    switch (keyCode)
    {
    case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
    case cocos2d::EventKeyboard::KeyCode::KEY_A:
        _directions[static_cast<size_t>(Direction::EAST)] = true;
        break;
        
    case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
    case cocos2d::EventKeyboard::KeyCode::KEY_D:
        _directions[static_cast<size_t>(Direction::WEST)] = true;
        break;
            
    case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
    case cocos2d::EventKeyboard::KeyCode::KEY_W:
        _directions[static_cast<size_t>(Direction::NORTH)] = true;
        break;
            
    case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
    case cocos2d::EventKeyboard::KeyCode::KEY_S:
        _directions[static_cast<size_t>(Direction::SOUTH)] = true;
        break;
            
    default:
        break;
    }
    
    onLevelTurn();
}

void KeyboardControlLayer::_onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    switch (keyCode)
    {
        case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        case cocos2d::EventKeyboard::KeyCode::KEY_A:
            _directions[static_cast<size_t>(Direction::EAST)] = false;
            break;
            
        case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        case cocos2d::EventKeyboard::KeyCode::KEY_D:
            _directions[static_cast<size_t>(Direction::WEST)] = false;
            break;
            
        case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
        case cocos2d::EventKeyboard::KeyCode::KEY_W:
            _directions[static_cast<size_t>(Direction::NORTH)] = false;
            break;
            
        case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
        case cocos2d::EventKeyboard::KeyCode::KEY_S:
            _directions[static_cast<size_t>(Direction::SOUTH)] = false;
            break;
            
        default:
            break;
    }
}
