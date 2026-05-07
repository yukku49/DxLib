#include"DxLib.h"
#include "EnemyManagiment.h"

void Enemy_Managiment::Enemy_Initialisation(float startX, float startY)
{
	this->a.enemy_X = startX;
	this->a.enemy_Y = startY;
	this->a.vx = -2.0f;
	this->a.isActive = true;
	// Load enemy direction sprites (JP: teki houkou sprite load)

	this->a.Enemy_Eye_handlbe[0] = LoadGraph("Pizza_Image/Enemy_Up.png");
	this->a.Enemy_Eye_handlbe[1] = LoadGraph("Pizza_Image/Enemy_Left.png");
	this->a.Enemy_Eye_handlbe[2] = LoadGraph("Pizza_Image/Enemy_Right.png");
	this->a.Enemy_Eye_handlbe[3] = LoadGraph("Pizza_Image/Enemy_Down.png");

};



void Enemy_Managiment::Enemy_Update()
{

	if (!this->a.isActive)
		return;
	// Move enemy (JP: teki ido)
	this->a.enemy_X += this->a.vx;
	this->a.enemy_Y += this->a.vy;
	// Bounce back at screen edges (JP: haji de hanekaeri)
	if (this->a.enemy_X < 0 || this->a.enemy_X>1280 - 32)
	{
		this->a.vx *= -1;
	}
};


