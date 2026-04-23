#include "BllentManagiment.h"
#include"PlayerManagiment.h"
#include"BackScreenManagiment.h"

#include"DxLib.h"

void Bllent_Managiment::Load()
{
	bllet_Handle[GENOVESE] = LoadGraph("Image/Genovese.png");
	bllet_Handle[MARGHERITA] = LoadGraph("Image/margherita.png");
	bllet_Handle[MARINARA] = LoadGraph("Image/marinara.png");
	bllet_Handle[QUATTROFORMAGGI] = LoadGraph("Image/quattro_formaggi.png");
	bllet_Handle[SEAGE] = LoadGraph("Image/sage.png");

}
void Bllent_Managiment::Update(BackScreen& stage, Player_Managiment& player)
{
	for (int i = 0; i < Max_Bullets; i++)
	{
		if (!m_bullets[i].isActive)
			continue;

		//ベクトル加算
		m_bullets[i].x += m_bullets[i].vx;
		m_bullets[i].y += m_bullets[i].vy;

		//画面外消滅
		if (m_bullets[i].x < -32 || m_bullets[i].x > 1312 ||
			m_bullets[i].y < -32 || m_bullets[i].y > 768)
		{
			m_bullets[i].isActive = false;
			continue;
		}

		//マップ x座標　MAP_WIDTH:40
		//マップ y座標　MAP_HEIGHT:23
		//壁判定
		if (stage.CheckCollision(m_bullets[i].x, m_bullets[i].y) || stage.CheckCllision(m_bullets->x + 16, m_bullets[i].y + 16))
		{
			m_bullets[i].isActive = false;
		}
		//敵にあった時に弾を消滅させる判定
	}

}
//素材を取得した時に現在取得している材料を組み合わせて作れるピザを見つける、ない場合はセージを入れる
void Bllent_Managiment::Shot(float x, float y, Player_Managiment& player)
{
	//マルゲリータ
	if (player.Itembring.Tomato_Counter > 0 && player.Player_Itembring.Cheese_Counter > 0 && player.Itembring.Basil_Counter > 0 && player.Itembring.Pizzadough_Counter > 0)
	{
		now_bllet_Handle = bllet_Handle[MARGHERITA];
	}
	//クワトロフォルマッジ
	else if (player.Itembring.Cheese_Counter > 0 && player.Itembring.Gorgonzola_Counter > 0 && player.Itembring.Pizzadough_Counter > 0)
	{
		now_bllet_Handle = bllet_Handle[QUATTROFORMAGGI];
	}
	//ジェノベーゼ
	else if (player.Itembring.Basil.Counter > 0 && player.Itembring.Pizzadough_Counter > 0 && player.Itembring.Cheese_Counter > 0 && player.Itembring.Tomato_Counter > 0)
	{
		now_bllet_Handle = bllet_Handle[GENOVESE];
	}
	//マリナーラ
	else if (player.Itembring.Tomato_Counter > 0 && player.Itembring.Pizzadough_Counter > 0 && player.Itembring.Basil_Counter > 0)
	{
		now_bllet_Handle = bllet_Handle[MARINARA];
	}
	//どの条件に合わないならセージ
	else
	{
		now_bllet_Handle = bllet_Handle[SEAGE];
	}
	//弾をセットする
	for (int i = 0; i < Max_Bullets; i++)
	{
		//開いて枠を検索
		if (!m_bullets[i].isActive)
		{
			m_bullets[i].isActive = true;
			m_bullets[i].x = player.Player_PositionX;
			m_bullets[i].y = player.Player_PositionY;
			m_bullets[i].using_handle = now_bllet_Handle;
			//向きに合わして速度をセット
			m_bullets[i].vx = 0;
			m_bullets[i].vy = 0;
			//上
			if (player.oldUp == 1)
			{
				m_bullets[i].vy = -5.0f;
				m_bullets[i].vx = 0;
				break;
			}
			//下
			else if (player.oldDown == 1)
			{
				m_bullets[i].vx = 0;
				m_bullets[i].vy = 5.0f;
				break;
			}
			//左
			else if (player.oldLeft == 1)
			{
				m_bullets[i].vx = -5.0f;
				m_bullets[i].vy = 0;
				break;
			}
			//右
			else if (player.oldRight == 1)
			{
				m_bullets[i].vx = 5.0f;
				m_bullets[i].vy = 0;
				break;
			}
		}
		break;

	}
}

