C++ DrawManagiment.cpp
#include "DrawManagiment.h"

#include "BackScreenManagiment.h"
#include "PlayerManagiment.h"
#include "EnemyManagiment.h"
#include "BllentManagiment.h"
#include "DxLib.h"

// タイル／描画単位（BackScreen の表示単位と合わせる）
constexpr int TILE_SIZE = 48;

// プレイヤー画像を描画（タイル座標 -> ピクセル座標 に変換）
void DrawManager::Player_Draw(const BackScreen& stage, const Player_Managiment& player) const
{
    int px = static_cast<int>(player.GetX() * TILE_SIZE);
    int py = static_cast<int>(player.GetY() * TILE_SIZE);
    DrawGraph(px, py, player.Get_PlayerHanadle(), TRUE);
}

// マップと障害物を表示（既存のまま、TILE_SIZE を使用）
void DrawManager::Map_Draw(const BackScreen& object) const
{
    DrawExtendGraph(0, 0, 1280, 736, object.Get_Maphandle(), FALSE);
    for (int y = 0; y < object.MAP_Get_SizeY(); ++y)
    {
        for (int x = 0; x < object.MAP_Get_SizeX(); ++x)
        {
            if (object.GetMapvalue(x, y) == 0)
            {
                DrawExtendGraph(x * TILE_SIZE, y * TILE_SIZE,
                                x * TILE_SIZE + TILE_SIZE, y * TILE_SIZE + TILE_SIZE,
                                object.Get_ObjectHanadle(), TRUE);
            }
        }
    }
}

void DrawManager::Bullets_Draw(const Bllent_Managiment& bullets) const
{
    for (int i = 0; i < bullets.GetMaxBullets(); ++i)
    {
        const auto& b = bullets.Get_Bullethandle(i);
        if (b.isActive)
        {
            DrawGraph(static_cast<int>(b.x * TILE_SIZE), static_cast<int>(b.y * TILE_SIZE), b.using_handle, TRUE);
        }
    }
}

void DrawManager::Enemy_Draw(const Enemy_Managiment& enemy, const BackScreen& stage) const
{
    auto e = enemy.Get_enemyPoint();
    DrawGraph(static_cast<int>(e.enemy_X * TILE_SIZE), static_cast<int>(e.enemy_Y * TILE_SIZE),
              e.Enemy_Eye_handlbe[0], TRUE);
}