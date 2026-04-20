#include"DxLib.h"
#include "EnemyManagiment.h"

void Enemy_Managiment::Enemy_Initialisation(float startX, float startY)
{
	this->a.enemy_X = startX;
	this->a.enemy_Y = startY;
	this->a.vx = -2.0f;
	this->a.isActive = true;
	//キャラクター画像を読み込む

	this->a.Enemy_Eye_handlbe[0] = LoadGraph("Image/player_up.png");
	/*
	PlayerImage_Handle[0] = LoadGraph("Image/player_up.png");
PlayerImage_Handle[1] = LoadGraph("Image/player_down.png");
PlayerImage_Handle[2] = LoadGraph("Image/player_left.png");
PlayerImage_Handle[3] = LoadGraph("Image/player_right.png");
*/
};



void Enemy_Update(Enemy_Managiment::Enemy_Update()
{

	if (!this->a.isActive)
		return;
	//移動
	this->a.enemy_X += this->a.vx;
	this->a.enemy_Y += this->a.vy;
	//画面端で跳ね返る
	if (this->a.enemy_X < 0 || this->a.enemy_X>1280 - 32)
	{
		this->a.vx *= -1;
	}
};


