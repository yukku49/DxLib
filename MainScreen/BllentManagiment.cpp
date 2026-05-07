#include "BllentManagiment.h"
#include"PlayerManagiment.h"
#include"BackScreenManagiment.h"
#include<iostream>
#include"DxLib.h"

void Bllent_Managiment::Load()
{
	bllet_Handle[GENOVESE] = LoadGraph("Pizza_Image/Genovese.png");
	bllet_Handle[MARGHERITA] = LoadGraph("Pizza_Image/margherita.png");
	bllet_Handle[MARINARA] = LoadGraph("Pizza_Image/marinara.png");
	bllet_Handle[QUATTROFORMAGGI] = LoadGraph("Pizza_Image/quattro_formaggi.png");
	bllet_Handle[SEAGE] = LoadGraph("Pizza_Image/sage.png");
	
	
}
void Bllent_Managiment::Update(BackScreen& stage, Player_Managiment& player)
{
	for (int i = 0; i < Max_Bullets; i++)
	{
		if (!m_bullets[i].isActive)
			continue;

		// Apply velocity (JP: sokudo han'ei)
		m_bullets[i].x += m_bullets[i].vx;
		m_bullets[i].y += m_bullets[i].vy;

		// Remove if out of screen (JP: gamengai de mukou)
		if (m_bullets[i].x < -32 || m_bullets[i].x > 1312 ||
			m_bullets[i].y < -32 || m_bullets[i].y > 768)
		{
			m_bullets[i].isActive = false;
			continue;
		}

		// Map size note X:40 Y:23 (JP: map saizu memo)
		// Check collision with map walls (JP: kabe collision)
		if (stage.CheckCollision(m_bullets[i].x, m_bullets[i].y) || stage.CheckCollision(m_bullets->x + 16, m_bullets[i].y + 16))
		{
			m_bullets[i].isActive = false;
		}
		// TODO: Add enemy hit detection (JP: teki hit hantei tsuika)
	}

}

// Select bullet type from held ingredients, fallback to sage (JP: sozai de dan shubetsu sentaku)
void Bllent_Managiment::Shot(float x, float y, Player_Managiment& player)
{

	auto item = player.Get_Player_Itembring();
	int tomato = item.Tmato_Counter;
	int Cheese = item.Cheese_Counter;
	int basil = item.Basil_Counter;
	int dough = item.Pizzadough_Counter;

	if (tomato > 0 && Cheese > 0 && basil > 0 && dough > 0) {
		now_bllet_Handle = bllet_Handle[MARGHERITA];
	}
	// Quattro formaggi (JP: kuattoro forumaggi)
	else if ( tomato> 0 && Cheese> 0 && dough > 0)
	{
		now_bllet_Handle = bllet_Handle[QUATTROFORMAGGI];
	}
	// Genovese (JP: jenobeze)
	else if (basil > 0 && dough > 0 && Cheese > 0 && tomato > 0)
	{
		now_bllet_Handle = bllet_Handle[GENOVESE];
	}
	// Marinara (JP: marinara)
	else if (tomato > 0 && dough > 0 && basil > 0)
	{
		now_bllet_Handle = bllet_Handle[MARINARA];
	}
	// Default to sage shot (JP: default sage)
	else
	{
		now_bllet_Handle = bllet_Handle[SEAGE];
	}
	// Find inactive bullet slot (JP: mishi yo slot)
	for (int i=0;i<Max_Bullets; i++)
	{
		// Use first available slot (JP: saisho no aki slot)
		if (!m_bullets[i].isActive)
		{
			m_bullets[i].isActive = true;
			m_bullets[i].x = player.GetX()*32;
			m_bullets[i].y = player.GetY()*32;
			m_bullets[i].using_handle = now_bllet_Handle;
			// Set velocity by player direction (JP: muki de sokudo set)
			m_bullets[i].vx = 0;
			m_bullets[i].vy = 0;
			// Up (JP: ue)
			if (player.GetDir() == PlayerEye_Up)
			{
				m_bullets[i].vy = -5.0f;
				m_bullets[i].vx = 0;
				break;
			}
			// Down (JP: shita)
			else if (player.GetDir() ==PlayerEye_Down)
			{
				m_bullets[i].vx = 0;
				m_bullets[i].vy = 5.0f;
				break;
			}
			// Left (JP: hidari)
			else if (player.GetDir() == PlayerEye_Left)
			{
				m_bullets[i].vx = -5.0f;
				m_bullets[i].vy = 0;
				break;
			}
			// Right (JP: migi)
			else if (player.GetDir() == PlayerEye_Right)
			{
				m_bullets[i].vx = 5.0f;
				m_bullets[i].vy = 0;
				break;
			}
		}
		

	}
}

