#include "ItemManagiment.h"
#include "BackGrandManagiment.h"
#include"PlayerManagiment.h"

ItemManagiment::ItemManagiment()
{
	for (int i = 0; i < MAX_SPAWN; i++)
	{

		m_items[i].isActive = false;
	}
}

void ItemManagiment::Load()
{
	Item_Handle[TOMATO] = LoadGraph("Image/TOMATO.png");
	Item_Handle[BASIL] = LoadGraph("Image/Basil.png");
	Item_Handle[CHEESE] = LoadGraph("Image/cheese.png");
	Item_Handle[GORGONZOLA] = LoadGraph("Image/gorgonzola.png");
	Item_Handle[PIZZADOUGH] = LoadGraph("Image/pizza_Dough.png");

}

void ItemManagiment::Spawn(const BackGrandManagiment& stage, Item_number type)
{
	for (int i = 0; i < MAX_SPAWN; i++)
	{
		if (!m_items[i].isActive)
		{
			int gx, gy;
			bool foundLocation = false;

			while (!foundLocation)
			{
				gx = GetRand(stage.StageManeger.MAP_WIDTH - 1);
				gy = GetRand(stage.StageManeger.MAP_HEIGHT - 1);
				if (stage.GetMapValue(gx, gy) == 1)
				{
					foundLocation = true;
				}

			}
			m_items[i].x = gx;
			m_items[i].y = gy;
			m_items[i].type = type;
			m_items[i].isActive = true;
			m_items[i].aliveTimer = 600;
			break;
		}
	}
}

Item_number ItemManagiment::CheckPickUp(PlayerManagiment& player)
{
	for (int i = 0; i < MAX_SPAWN; i++)
	{
		if (m_items[i].x == player.PlayerMovePointX && m_items->y == player.PlayerMovePointY)
		{
			return Item_number();
		}
	}

}
