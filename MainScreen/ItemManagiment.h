#pragma once
#include "DxLib.h"
#include"PlayerManagiment.h"
// Enum for item types (JP: item shurui)
enum Item_number
{
	TOMATO,
	BASIL,
	CHEESE,
	GORGONZOLA,
	PIZZADOUGH,
	ITEM_MAX
};
struct ItemData
{
	int x, y;// Grid position on stage (JP: stage masu zahyo)
	Item_number type;
	int aliveTimer;// Despawn timer (JP: shometsu timer)
	bool isActive;
};



class Item_Managiment
{

private:
	// Item image handle array (JP: image handle hairetsu)
	int Item_Handle[ITEM_MAX];
	// Maximum simultaneous spawned items (JP: douji saidai)
	static const int MAX_SPAWN = 5;
	// Active item instances (JP: jittai data)
	ItemData m_items[MAX_SPAWN];
public:
	// Initialize item states (JP: state shokika)
	void ItemManagiment();

	// Load item images (JP: image load)
	void Load();

	// Spawn item at random valid tile (JP: random haichi)
	void Spawn(const BackScreen& stage, Item_number type);

	// Update item timers (JP: timer update)
	void Updata();

	// Check player pickup and return item type (JP: shutoku hantei)
	Item_number CheckPickUp(Player_Managiment& player);

	// Getter for item enum max (JP: enum max getter)
	Item_number Get_Item_number()const{ return ITEM_MAX; };

};

