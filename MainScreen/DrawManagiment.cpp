#include "DrawManagiment.h"
#include"BackScreenManagiment.h"
#include"EnemyManagiment.h"
#include"BllentManagiment.h"

//プレイヤー画像を描画
void DrawManager::Player_Draw(const BackScreen& stage, const Player_Managiment& player)
{
	//LoadGraphScreen(int(stage.m_stageMap[player.GetX()]), int(stage.m_stageMap[player.GetY()]), player.Get_PlayerHanadle()), TRUE);
	DrawGraph((int)player.GetX(), (int)player.GetY(), player.Get_PlayerHanadle(), TRUE);
}

//障害物を描画
void DrawManager::Object_Draw(const BackScreen& object)
{
	for (int y = 0; y < object.MAP_Get_SizeX(); y++)
	{
		for (int x = 0; x < object.MAP_Get_SizeY(); x++)
		{
			if (object.GetMapvalue(x,y) == 0)
			{
				DrawGraph(y, x, object.Get_ObjectHanadle(), TRUE);
			}
		}
	}
}

void DrawManager::Bullets_Draw(const Bllent_Managiment& bullets)
{
	for (int i = 0; i < bullets.GetMaxBullets(); i++)
	{
		//i番目の弾のデータを参照として受け取る（コピーしないのから高速）
		const auto& b = bullets.Get_Bullethandle(i);
		if (b.isActive)
		{
			DrawGraph((int)b.x,(int)b.y, bullets.Get_Bullethandle(i).using_handle, TRUE);
		}
	}
}

void DrawManager::Enemy_Draw(const Enemy_Managiment& enemy, const BackScreen& stage)
{
	//LoadGraphScreen(stage.m_stageMap[enemy.Enemy_PointX], stage.m_stageMap[enemy.PlayerMovePointY], enemy.Player_Handle, TRUE);
	DrawGraph(enemy.Get_enemyPoint().enemy_X, enemy.Get_enemyPoint().enemy_Y, enemy.Get_enemyPoint().Enemy_Eye_handlbe[Enemy_Left], enemy.Get_enemyPoint().isActive);
}

