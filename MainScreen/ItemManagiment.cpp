#include "ItemManagiment.h"
#include "BackScreenManagiment.h"
#include"PlayerManagiment.h"

void Item_Managiment::ItemManagiment()
{
	for (int i = 0; i < MAX_SPAWN; i++)
	{

		m_items[i].isActive = false;
	}
}

void Item_Managiment::Load()
{
	Item_Handle[TOMATO] = LoadGraph("Image/TOMATO.png");
	Item_Handle[BASIL] = LoadGraph("Image/Basil.png");
	Item_Handle[CHEESE] = LoadGraph("Image/cheese.png");
	Item_Handle[GORGONZOLA] = LoadGraph("Image/gorgonzola.png");
	Item_Handle[PIZZADOUGH] = LoadGraph("Image/pizza_Dough.png");

}

void Item_Managiment::Spawn(const BackScreen& stage, Item_number type)
{
	for (int i = 0; i < MAX_SPAWN; i++)
	{
		if (!m_items[i].isActive)
		{
			int gx, gy;
			bool foundLocation = false;

			while (!foundLocation)
			{
				gx = GetRand(stage.MAP_Get_SizeX() - 1);
				gy = GetRand(stage.MAP_Get_SizeY() - 1);
				if (stage.GetMapvalue(gx, gy) == 1)
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

Item_number Item_Managiment::CheckPickUp(Player_Managiment& player)
{
	for (int i = 0; i < MAX_SPAWN; i++)
	{
		if (m_items[i].x == player.GetX() && m_items[i].y == player.GetY())
		{
			return Item_number();
		}
	}

}
