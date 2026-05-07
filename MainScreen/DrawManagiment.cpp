#include "DrawManagiment.h"
#include "BackScreenManagiment.h"
#include "EnemyManagiment.h"
#include "BllentManagiment.h"
#include"DxLib.h"

// Tile drawing unit（タイル描画単位）
constexpr int TILE_SIZE = 32; // Match map grid size if needed（必要ならマップのグリッド単位に合わせる）

// Draw player sprite（プレイヤー画像を描画）
void DrawManager::Player_Draw(const BackScreen& stage, const Player_Managiment& player)const
{
int w,h;
GetGraphSize(player.Get_PlayerHanadle(),&w,&h);
//画面の比率を維持するための倍率を計算
//縦と横、どちらが「よりはみ出しているか」を基準にする
float scale=(w>h)?static_cast<float>(TILE_SIZE)/w:static_cast<float>(TILE_SIZE)/h;
//実際に描画する幅と高さを算出
int drawW=static_cast<int>(w*scale);
int drawH=static_cast<int>(h*scale);
//中央ぞろえにするためのオフセット（ズレ）を計算
int offsetX=(TILE_SIZE-drawW)/2;
int offsetY=(TILE_SIZE-drawH)/2;
	// Top-left position（左上の座標）
	int x1 = (int)player.GetX() * TILE_SIZE+offsetX;
	int y1 = (int)player.GetY() * TILE_SIZE+offsetY;
	// Bottom-right position（右下の座標）
	int x2 = x1 + drawW;
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
