#pragma once
// Enum for enemy facing sprite indices (JP: 敵の向きスプライトのインデックス)
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
	int enemy_X = 0;// Enemy X position (JP: 敵のX座標)
	int enemy_Y = 0;// Enemy Y position (JP: 敵のY座標)
	float vx = 0;// X velocity (JP: X方向速度)
	float vy = 0;// Y velocity (JP: Y方向速度)
	int fullness_gauge = 0;// Fullness gauge (JP: 満腹ゲージ)
	bool isActive = false;// Active state flag (JP: 有効フラグ)
	// Enemy direction sprite handles (JP: 向き別スプライトハンドル)
	int Enemy_Eye_handlbe[enemy_Eye::Enemy_Max];
};
class Enemy_Managiment
{

	Enemy_date Enemy;
	int Enemy_DisplaySize = 28;// Enemy draw size (JP: 敵の描画サイズ)
	int Enemy_Handle = 0;// Current enemy draw handle (JP: 現在の敵描画ハンドル)
public:
	//Setter
	// Spawn and initialize enemy (JP: 敵の生成と初期化)
	void Enemy_Initialisation();
	// Update enemy movement (JP: 敵の移動更新)
	void Enemy_Update();

	//Getter

	// Getter for enemy state (JP: 敵状態の取得)
	const Enemy_date& Get_enemyPoint()const { return Enemy; };

	//Get enemy position(JP:敵の位置を取得)
	float Get_EX()const { return Enemy.enemy_X; }
	float Get_EY()const { return Enemy.enemy_Y; }

	//Get enemy imate handle by direction(JP:現在の向きハンドルを取得)
	int Get_Ehandle()const { return Enemy_Handle; };

	//Get enemy dorw size(JP:敵の描画サイズを取得)
	int Get_EDisplaySize()const { return Enemy_DisplaySize; }
};