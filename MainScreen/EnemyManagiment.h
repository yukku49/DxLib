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
	float enemy_X = 0.0f; // 敵の X（ピクセル単位）
	float enemy_Y = 0.0f; // 敵の Y（ピクセル単位）
	float vx = 0.0f;      // X velocity (px/frame or px/sec depending on Update)
	float vy = 0.0f;      // Y velocity
	int fullness_gauge = 0;
	bool isActive = false;
	int Enemy_Eye_handlbe[enemy_Eye::Enemy_Max];
};

class Enemy_Managiment
{
private:
	Enemy_date a;
	// プレイヤーと同じ描画サイズ（px）
	const int m_displaySize = 28;

public:
	// Spawn and initialize enemy (startX/startY are pixel coordinates)
	void Enemy_Initialisation(float startX, float startY);

	// Getter for enemy state
	const Enemy_date& Get_enemyPoint() const { return a; }

	// Update enemy movement
	void Enemy_Update();

	// DrawManager 用 getter
	float Get_enemyX() const { return a.enemy_X; }
	float Get_enemyY() const { return a.enemy_Y; }
	int Get_EnemyHandle() const;
	int Get_EnemyDisplaySize() const { return m_displaySize; }
};