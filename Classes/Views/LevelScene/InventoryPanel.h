#ifndef _CHIPS_CHALLENGE_SMART_INVENTORY_PANEL_H_
#define _CHIPS_CHALLENGE_SMART_INVENTORY_PANEL_H_

#include <cocos2d.h>
#include <ui/CocosGUI.h>

#include <Level/TileType.h>
#include <Level/Inventory.h>

class InventoryPanel : public cocos2d::Node
{
public:
	static InventoryPanel* create(Inventory* inventory);
	InventoryPanel(Inventory* inventory);
	virtual ~InventoryPanel();

private:
	void _build();
	void _update();
	std::string& _getSpriteFrameName(TileType itemType) const;

	Inventory* _inventory;
	cocos2d::Node* _background;
	cocos2d::EventListenerCustom* _inventoryChangedListener;
};
    
#endif
