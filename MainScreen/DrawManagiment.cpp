#include "DrawManagiment.h"
#include "BackScreenManagiment.h"
#include "EnemyManagiment.h"
#include "BllentManagiment.h"
#include"DxLib.h"

// Tile drawing unit（タイル描画単位）
const int TILE_SIZE = 32; // Match map grid size if needed（必要ならマップのグリッド単位に合わせる）

// Draw player sprite（プレイヤー画像を描画）
void DrawManager::Player_Draw(const BackScreen& stage, const Player_Managiment& player)const
{
	// 1. 画像の実際のサイズ(w, h)を取得する
	int w, h;
	GetGraphSize(player.Get_PlayerHanadle(), &w, &h);

	// 2. 基準となる「幅」をプレイヤー設定から取得
	const int CHARA_WIDTH = player.Get_PlayerDisplaySize();

	// 3. 画像の比率(w:h)を維持したまま、描画時の「高さ」を計算する
	// 計算式：drawH = CHARA_WIDTH * (元の高さ / 元の幅)
	int drawH = static_cast<int>(CHARA_WIDTH * (static_cast<float>(h) / w));

	// 4. 描画座標の計算
	// 横：タイルの中心にキャラの幅(CHARA_WIDTH)を合わせる
	int x1 = static_cast<int>(player.GetX() * TILE_SIZE) + (TILE_SIZE - CHARA_WIDTH) / 2;
	// 縦：キャラの「足元」がタイルの底に付くように配置する
	int y1 = static_cast<int>(player.GetY() * TILE_SIZE) + (TILE_SIZE - drawH);

	int x2 = x1 + CHARA_WIDTH;
	int y2 = y1 + drawH;

	DrawExtendGraph(x1,y1,x2,y2, player.Get_PlayerHanadle(), true);
}

// Draw map and obstacles（マップと障害物を表示）
void DrawManager::Map_Draw(const BackScreen& object)const
{	
	// Full background（背景全体）
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
		if (b.isActive&&b.using_handle>=0)
		{
			DrawExtendGraph(static_cast<int>(b.x), static_cast<int>(b.y),static_cast<int>(b.x)+32,static_cast<int>(b.y)+32, b.using_handle, true);
		}
	}
}

void DrawManager::Enemy_Draw(const Enemy_Managiment& enemy, const BackScreen& stage) const
{
	DrawGraph(enemy.Get_enemyPoint().enemy_X, enemy.Get_enemyPoint().enemy_Y,
           enemy.Get_enemyPoint().Enemy_Eye_handlbe[0], TRUE);
}
