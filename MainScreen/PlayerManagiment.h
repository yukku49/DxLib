#pragma once
// Avoid including DrawManagiment.h here (JP: ここではDrawManagiment.hをインクルードしない)
#include <array>

enum Player_EyeContact
{
	PlayerEye_Up = 0,
	PlayerEye_Down,
	PlayerEye_Left,
	PlayerEye_Right,
	PlayerEye_Max
};

struct Item_count
{
	int Tmato_Counter;
	int Basil_Counter;
	int Cheese_Counter;
	int Gorgonzola_Counter;
	int Pizzadough_Counter;
};

class Item_Managiment;
class Bllent_Managiment;
class BackScreen; // Forward declaration (JP: 前方宣言)

class Player_Managiment
{
private:
	// Player initial position constants (JP: プレイヤー初期位置の定数)
	const float Player_StanderdpointX = 1.0f;
	const float Player_StanderdpointY = 1.0f;

	// Player movement position values (JP: 移動位置の値)
	float Player_MovePointX = 0;
	float Player_MovePointY = 0;

	// Player image handles by direction (JP: 向き別プレイヤー画像ハンドル)

	int PlayerImage_Handle[Player_EyeContact::PlayerEye_Max];

	// Current player draw handle (JP: 現在の描画用ハンドル)
	int Player_Handle=0;

	const int m_displaySize = 28;// Draw size (JP: 描画サイズ)

	// Previous key state for single-step input (JP: 1マスずつ入力用の前フレームのキー状態)
	int m_oldUp = 0, m_oldDown = 0, m_oldLeft = 0, m_oldRight = 0;

	// Current facing direction (JP: 現在の向き)
	int m_dir;

	// Structure for held ingredients (JP: 所持材料の構造体)
	Item_count Player_Itembring;
public:
	void Initialisation();
	// Update with stage collision and bullet actions (JP: ステージ衝突と弾の処理で更新)
	void Update(const BackScreen& stage, Bllent_Managiment& bllent);

	// Read-only position getters (JP: 読み取り専用の位置取得)
	int GetX()const { return (int)Player_MovePointX; }
	int GetY()const { return (int)Player_MovePointY; }

	// Return current player sprite handle (JP: 現在のスプライトハンドルを返す)
	int Get_PlayerHanadle()const { return Player_Handle; }

	// Return held ingredient data (JP: 所持材料データを返す)
	Item_count Get_Player_Itembring()const { return Player_Itembring; };

	// Increase held item counters (JP: 所持アイテムのカウンタを増やす)
	void Player_BringItem(Item_Managiment& item);

	// Getter for current facing direction (JP: 向きの取得)
	int  GetDir()const { return m_dir; };


	// Player draw function is declared in Draw header (JP: 描画関数はDraw系ヘッダで宣言)
	//void Draw_Managiment::DrawManager();
};
