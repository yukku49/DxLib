#pragma once
// Enum for enemy facing sprite indices (JP: teki muki index)
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
	int enemy_X = 0;// Enemy X position (JP: teki X zahyo)
	int enemy_Y = 0;// Enemy Y position (JP: teki Y zahyo)
	float vx = 0;// X velocity (JP: X sokudo)
	float vy = 0;// Y velocity (JP: Y sokudo)
	int fullness_gauge = 0;// Fullness gauge (JP: manpuku gauge)
	bool isActive = false;// Active state flag (JP: yuko hantei)
	// Enemy direction sprite handles (JP: muki image handle)
	int Enemy_Eye_handlbe[enemy_Eye::Enemy_Max];
};
class Enemy_Managiment
{

	Enemy_date a;
public:
	// Spawn and initialize enemy (JP: teki shokika)
	void Enemy_Initialisation(float startX, float startY);
	// Getter for enemy state (JP: teki joho getter)
	const Enemy_date& Get_enemyPoint()const { return a; };

	// Update enemy movement (JP: teki ido update)
	void Enemy_Update();



};

