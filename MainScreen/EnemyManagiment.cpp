#include"DxLib.h"
#include "EnemyManagiment.h"

void Enemy_Managiment::Enemy_Initialisation()
{
	this->Enemy.enemy_X = 22;
	this->Enemy.enemy_Y = 22;
	this->Enemy.vx = -2.0f;
	this->Enemy.vy = -2.0f;
	this->Enemy.isActive = true;
	// Load enemy direction sprites (JP: 敵の向き別スプライトを読み込む)

	this->Enemy.Enemy_Eye_handlbe[0] = LoadGraph("../Pizza_Image/Enemy_Up.png");
	this->Enemy.Enemy_Eye_handlbe[1] = LoadGraph("../Pizza_Image/Enemy_Left.png");
	this->Enemy.Enemy_Eye_handlbe[2] = LoadGraph("../Pizza_Image/Enemy_Right.png");
	this->Enemy.Enemy_Eye_handlbe[3] = LoadGraph("../Pizza_Image/Enemy_Down.png");

	//Load Intilisation enemy direction(初期の敵の向きを入れる)
	this->Enemy_Handle = enemy_Eye::Enemy_Left;
};



void Enemy_Managiment::Enemy_Update()
{

	if (!this->Enemy.isActive)
		return;
	// Move enemy (JP: 敵を移動)
	this->Enemy.enemy_X += this->Enemy.vx;
	this->Enemy.enemy_Y += this->Enemy.vy;
	// Bounce back at screen edges (JP: 画面端で跳ね返る)
	if (this->Enemy.enemy_X < 0 || this->Enemy.enemy_X>1280 - 32)
	{
		this->Enemy.vx *= -1;
	}
};


