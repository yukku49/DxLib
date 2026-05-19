#include "DxLib.h"
#include "EnemyManagiment.h"

// 画面サイズやスプライトサイズが必要なら共通化してください
static const int SCREEN_W = 1280;
static const int SCREEN_H = 736;
static const float DEFAULT_SPEED = -2.0f;

void Enemy_Managiment::Enemy_Initialisation(float startX, float startY)
{
	// startX/startY はピクセル単位で受け取る想定
	a.enemy_X = startX;
	a.enemy_Y = startY;
	a.vx = DEFAULT_SPEED; // px/frame あるいは px/更新単位
	a.vy = 0.0f;
	a.isActive = true;

	// 画像は初期化時一度だけロードする
	a.Enemy_Eye_handlbe[Enemy_Up] = LoadGraph("../Pizza_Image/Enemy_Up.png");
	a.Enemy_Eye_handlbe[Enemy_Left] = LoadGraph("../Pizza_Image/Enemy_Left.png");
	a.Enemy_Eye_handlbe[Enemy_Right] = LoadGraph("../Pizza_Image/Enemy_Right.png");
	a.Enemy_Eye_handlbe[Enemy_Down] = LoadGraph("../Pizza_Image/Enemy_Down.png");
}

int Enemy_Managiment::Get_EnemyHandle() const
{
	// 簡易：vx/vy によって向きを推定してハンドルを返す
	if (!a.isActive) return -1;
	if (a.vx < 0.0f) return a.Enemy_Eye_handlbe[Enemy_Left];
	if (a.vx > 0.0f) return a.Enemy_Eye_handlbe[Enemy_Right];
	if (a.vy < 0.0f) return a.Enemy_Eye_handlbe[Enemy_Up];
	return a.Enemy_Eye_handlbe[Enemy_Down];
}

void Enemy_Managiment::Enemy_Update()
{
	if (!a.isActive) return;

	// 単純な移動。vx/vy はピクセル単位で扱う（必要なら dt を掛ける）
	a.enemy_X += a.vx;
	a.enemy_Y += a.vy;

	// 端で反転（スクリーン端を想定）
	if (a.enemy_X < 0.0f || a.enemy_X > SCREEN_W - 32.0f)
	{
		a.vx *= -1.0f;
	}
	if (a.enemy_Y < 0.0f || a.enemy_Y > SCREEN_H - 32.0f)
	{
		a.vy *= -1.0f;
	}
}


