#include "DrawManagiment.h"
#include"BackScreenManagiment.h"

//ƒvƒŒƒCƒ„[‰æ‘œ‚ğ•`‰æ
void DrawManager::Player_Draw(const BackScreenManagiment::StageManager& stage, const PlayerManagiment& player)
{
	LoadGraphScreen(stage.m_stageMap[player.PlayerMovePointX], stage.m_stageMap[player.PlayerMovePointY], player.Player_Handle, TRUE);
}

//áŠQ•¨‚ğ•`‰æ
void DrawManager::Object_Draw(const BackScreenManagiment::StageManager& object)
{
	for (int y = 0; y < object.MAP_HEIGHT; y++)
	{
		for (int x = 0; x < object.MAP_WIDTH; x++)
		{
			if (object.m_stageMap[y][x] == 0)
			{
				LoadGraphScreen(y, x, object.m_handles[1], TRUE);
			}
		}
	}
}

void DrawManager::Bullets_Draw(const BllentManagiment::Bllent_Managiment& bullets)
{
	for (int i = 0; i < bullets.Max_Bullets; i++)
	{
		if (bullets.m_bullets[i].isActive)
		{
			DrawGraph(bullets.m_bullets[i].x, bullets.m_bullets[i].y, bullets.m_bullets[i].using_hnadle, TRUE);
		}
	}
}

void DrawManager::Enemy_Draw(const EnemyManagiment::EnemyManagiment& enemy, const BackGrandManagiment::StageManeger& stage)
{
	LoadGraphScreen(stage.m_stageMap[enemy.Enemy_PointX], stage.m_stageMap[enemy.PlayerMovePointY], enemy.Player_Handle, TRUE);
}

