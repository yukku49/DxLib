#include "DrawManagiment.h"
#include "BackScreenManagiment.h"
#include "EnemyManagiment.h"
#include "BllentManagiment.h"
#include"DxLib.h"

// タイル/描画単位を一箇所で定義
constexpr int TILE_SIZE = 32; // 必要なら 32 に変える（マップデータの単位に合わせる）

//プレイヤー画像を描画
void DrawManager::Player_Draw(const BackScreen& stage, const Player_Managiment& player)const
{
	//左上の座標
	int x1 = (int)player.GetX() * TILE_SIZE;
	int y1 = (int)player.GetY() * TILE_SIZE;
	//右下の座標
	int x2 = x1 + TILE_SIZE;
	int y2 = y1 + TILE_SIZE;

	DrawExtendGraph(x1,y1 ,x2,y2, player.Get_PlayerHanadle(), true);
}

//マップと障害物を表示
void DrawManager::Map_Draw(const BackScreen& object)const
{	
	// 背景全体
	DrawExtendGraph(0, 0, 1280, 736, object.Get_Maphandle(), false);

	for (int y = 0; y < object.MAP_Get_SizeY(); y++)
	{
		for (int x = 0; x < object.MAP_Get_SizeX(); x++)
		{
			if (object.GetMapvalue(x, y) == 0)
			{
				DrawExtendGraph(x * TILE_SIZE, y * TILE_SIZE,
                    x * TILE_SIZE + TILE_SIZE, y * TILE_SIZE + TILE_SIZE,
                    object.Get_ObjectHanadle(), true);
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
			DrawExtendGraph(static_cast<int>(b.x), static_cast<int>(b.y),static_cast<int>(b.x)+32,static_cast<int>(b.y)+32, b.using_handle, false);
		}
	}
}

void DrawManager::Enemy_Draw(const Enemy_Managiment& enemy, const BackScreen& stage) const
{
	DrawGraph(enemy.Get_enemyPoint().enemy_X, enemy.Get_enemyPoint().enemy_Y,
           enemy.Get_enemyPoint().Enemy_Eye_handlbe[0], TRUE);
}
