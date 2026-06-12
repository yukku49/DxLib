#include "ItemManagiment.h"
#include "BackScreenManagiment.h"
#include "PlayerManagiment.h"

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
            int gx = 0, gy = 0;
            bool foundLocation = false;

            const int MAX_ATTEMPTS = 1000;
            int attempts = 0;
            const int TILE_SIZE = 32;

            while (!foundLocation && attempts < MAX_ATTEMPTS)
            {
                attempts++;
                gx = GetRand(stage.MAP_Get_SizeX() - 1);
                gy = GetRand(stage.MAP_Get_SizeY() - 1);

                if (stage.GetMapvalue(gx, gy) != 1) continue;

                double worldX = gx * TILE_SIZE + TILE_SIZE / 2.0;
                double worldY = gy * TILE_SIZE + TILE_SIZE / 2.0;
                if (stage.CheckCollision(worldX, worldY)) continue;

                bool overlapWithItem = false;
                for (int j = 0; j < MAX_SPAWN; j++)
                {
                    if (m_items[j].isActive && m_items[j].x == gx && m_items[j].y == gy)
                    {
                        overlapWithItem = true;
                        break;
                    }
                }
                if (overlapWithItem) continue;

                foundLocation = true;
            }

            if (!foundLocation) return;

            m_items[i].x = gx;
            m_items[i].y = gy;
            m_items[i].type = type;
            m_items[i].isActive = true;
            m_items[i].aliveTimer = 300;
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
        if (!m_items[i].isActive) continue;

        if (abs(m_items[i].x - player.GetX()) <= 1 &&
            abs(m_items[i].y - player.GetY()) <= 1)
        {
            // 【修正】type を先に保存してから無効化する
            Item_number pickedType = m_items[i].type;
            m_items[i].isActive = false;
            return pickedType;
        }
    }

    return ITEM_MAX;
}