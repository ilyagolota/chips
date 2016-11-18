#ifndef _CHIPS_CHALLENGE_BEARTRAP_H_
#define _CHIPS_CHALLENGE_BEARTRAP_H_

#include <cocos2d.h>
#include "LevelObject.h"

class Level;

class Beartrap : public LevelObject
{   
public:
    static Beartrap* create(Level* level, const cocos2d::Vec2& coordinate);
    
	Beartrap(Level* level, const cocos2d::Vec2& coordinate);
    
    bool isOpen() const;
    void setOpen(bool open);
    
    void onAdd() override;
    void reset() override;
    
    bool isEscapableBy(const Creature* creature, Direction direction) const override;
	void beforeEnter(Creature* creature) override;
    void afterEnter(Creature* creature) override;

private:
	static cocos2d::Vec2 BOTTOM_POSITION;

	bool _open;
	cocos2d::Sprite* _rootNode;
	cocos2d::Sprite* _frontNode;
	cocos2d::Sprite* _pistonNode;
};

#endif
