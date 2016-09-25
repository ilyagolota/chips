#include <ui/CocosGUI.h>
#include <Level/Level.h>

#include "InventoryPanel.h"

InventoryPanel* InventoryPanel::create(Inventory* inventory)
{
	auto instance = new InventoryPanel(inventory);
    instance->autorelease();
    return instance;
}

InventoryPanel::InventoryPanel(Inventory* inventory)
{
    Node::init();

	_inventory = inventory;
	_inventory->retain();

	_build();
	_update();

	auto eventDispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
	_inventoryChangedListener = eventDispatcher->addCustomEventListener(Inventory::ITEM_COUNT_CHANGED, [this](cocos2d::EventCustom* e) {
		_update();
	});
}

InventoryPanel::~InventoryPanel()
{
	auto eventDispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
	eventDispatcher->removeEventListener(_inventoryChangedListener);
}

void InventoryPanel::_build()
{
	cocos2d::Size cellSize(54, 48);

	setContentSize(cocos2d::Size(cellSize.width * 8 + 8, cellSize.height + 16));

	_background = cocos2d::ui::Scale9Sprite::createWithSpriteFrameName("ui-panel-transparent.png");
	_background->setContentSize(getContentSize());
	_background->setAnchorPoint(cocos2d::Vec2::ZERO);
	addChild(_background);

	float x = 8;
	for (size_t i = 0; i < 8; i++)
	{
		auto cell = cocos2d::Sprite::createWithSpriteFrameName("ui-inventory-cell.png");
		cell->setAnchorPoint(cocos2d::Vec2::ZERO);
		cell->setPosition(cocos2d::Vec2(x, 8));
		_background->addChild(cell);

		x += cellSize.width;
	}
}

void InventoryPanel::_update()
{
	TileType itemTypes[8] = 
	{
		TileType::KEY_BLUE,
		TileType::KEY_RED,
		TileType::KEY_GREEN,
		TileType::KEY_YELLOW,
		TileType::BOOTS_WATER,
		TileType::BOOTS_FIRE,
		TileType::BOOTS_ICE,
		TileType::BOOTS_SLIDE,
	};

	auto& cells = _background->getChildren();

	for (auto cell : cells)
	{
		cell->removeAllChildren();
	}

	auto cellIter = cells.begin();
	for (size_t i = 0; i < 8; i++)
	{
		int itemCount = _inventory->getItemCount(itemTypes[i]);
		if (itemCount != 0)
		{
			if (cellIter != cells.end())
			{
				auto cell = *cellIter;

				auto icon = cocos2d::Sprite::createWithSpriteFrameName(_getSpriteFrameName(itemTypes[i]));
				icon->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
				icon->setPosition(cocos2d::Vec2(cell->getContentSize()) * 0.5f + cocos2d::Vec2(-1, 0));
				cell->addChild(icon);

				auto countLabel = cocos2d::Label::create(std::to_string(itemCount), "fonts/Marker Felt.ttf", 10);
				countLabel->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
				countLabel->setPosition(cocos2d::Vec2(cell->getContentSize()) * 0.5f + cocos2d::Vec2(23, -22));
				cell->addChild(countLabel);

				cellIter++;
			}
		}
	}
}

std::string& InventoryPanel::_getSpriteFrameName(TileType itemType) const
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
	switch (itemType)
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
