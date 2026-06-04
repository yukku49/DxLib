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
	Item_Handle[TOMATO] = LoadGraph("../Pizza_Image/TOMATO.png");
	Item_Handle[BASIL] = LoadGraph("../Pizza_Image/Basil.png");
	Item_Handle[CHEESE] = LoadGraph("../Pizza_Image/cheese.png");
	Item_Handle[GORGONZOLA] = LoadGraph("../Pizza_Image/gorgonzola.png");
	Item_Handle[PIZZADOUGH] = LoadGraph("../Pizza_Image/pizza_Dough.png");
	
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
			m_items[i].type =type;
			m_items[i].isActive = true;
			m_items[i].aliveTimer = 600;
			break;
		}
	}

}

void Item_Managiment::Updata()
{
	for (int i = 0; i < MAX_SPAWN; i++)
	{
		if (m_items[i].isActive)
		{
			m_items[i].aliveTimer--;
			if (m_items[i].aliveTimer <= 0)
			{
				m_items[i].isActive = false;
			}
		}
	}
}



Item_number Item_Managiment::CheckPickUp(Player_Managiment& player)
{
	for (int i = 0; i < MAX_SPAWN; i++)
	{
		//アイテムが有効でない場合はスキップ
		if (!m_items[i].isActive)continue;

		//プレイヤーとアイテムの距離が1マス以内かを判定（タイル単位で比較）
		if (abs(m_items[i].x-player.GetX())<=1 && abs(m_items[i].y -player.GetY())<=1)
		{
			//アイテムを無効化
			m_items[i].isActive = false;

			//アイテムtypeを返す
			return m_items[i].type;
		}
	}

	return ITEM_MAX;
}




