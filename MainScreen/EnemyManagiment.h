#pragma once
//Enemyの視点を収納する配列の要素数を決める列挙体
enum enemy_Eye
{
	Enemy_Up,
	Enemy_Left,
	Enemy_Right,
	Enemy_Down,
	Enemy_Max
};

struct Enemy_date
{
	int enemy_X = 0;//敵のX座標
	int enemy_Y = 0;//敵のY座標
	float vx = 0;//移動用X座標
	float vy = 0;//移動用Y座標
	int fullness_gauge = 0;//満腹ゲージ
	bool isActive = false;//被弾しているかの判定
	//敵の向き画像を収納するハンドル
	int Enemy_Eye_handlbe[enemy_Eye::Enemy_Max];
};
class Enemy_Managiment
{

	Enemy_date a;
public:
	//enemyをスポーンさせる
	void Enemy_Initialisation(float startX, float startY);

	//nemyを動かす
	void Enemy_Update();



};

