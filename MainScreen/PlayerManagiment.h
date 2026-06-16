#pragma once
#include"ItemTypes.h"
// Avoid including DrawManagiment.h here (JP: ここではDrawManagiment.hをインクルードしない)
#include <array>

// Four directions the player sprite can face
// Used as indices into PlayerImage_Handle[]
// (JP: プレイヤースプライトが向ける4方向。PlayerImage_Handle[]のインデックスに使う)
enum Player_EyeContact
{
	PlayerEye_Up = 0,
	PlayerEye_Down,
	PlayerEye_Left,
	PlayerEye_Right,
	PlayerEye_Max
};

// Forward declarations to avoid circular includes
// (JP: 循環インクルードを避けるための前方宣言)
class Item_Managiment;
class Bllent_Managiment;
class BackScreen; // Forward declaration (JP: 前方宣言)
class PizzaDatabase;


// Manages everything related to the player:
// movement, collision, item carrying, pizza crafting, and shooting
// (JP: プレイヤーの移動・衝突・アイテム所持・ピザ製作・射撃をすべて管理するクラス)
class Player_Managiment
{
private:
	// Initial tile position (converted to pixels in Initialisation)
	// (JP: 初期タイル座標。Initialisation()でピクセルに変換される)
	const float Player_StanderdpointX = 1.0f;
	const float Player_StanderdpointY = 1.0f;

	// Current position in pixel space (float for smooth sub-pixel movement)
	// (JP: 現在のピクセル座標。滑らかなサブピクセル移動のためfloat)
	float Player_MovePointX = 0.0f; // px
	float Player_MovePointY = 0.0f; // px

	// One image handle per facing direction
	// (JP: 向きごとに1つの画像ハンドル)
	int PlayerImage_Handle[Player_EyeContact::PlayerEye_Max];

	// The handle currently selected for drawing (changes with direction)
	// (JP: 現在描画に使うハンドル。向き変更で切り替わる)
	int Player_Handle = 0;

	// Width used for both drawing and hit-box calculations (px)
	// (JP: 描画とヒットボックス計算に使う幅(px))
	const int m_displaySize = 28; // 描画サイズ(px/概ね)

	// Previous-frame key states, used to detect key-press edges (press, not hold)
	// (JP: 前フレームのキー状態。押した瞬間(エッジ)を検出するために使う)
	int m_oldUp = 0, m_oldDown = 0, m_oldLeft = 0, m_oldRight = 0;
	int m_oldSpace = 0;

	// Current facing direction (one of Player_EyeContact values)
	// (JP: 現在の向き。Player_EyeContact の値のいずれか)
	int m_dir;

	// Ingredients the player is currently carrying
	// (JP: プレイヤーが現在所持している食材)
	Item_count Player_Itembring;

	// Movement speed in pixels per second
	// (JP: 移動速度。ピクセル毎秒)
	float m_moveSpeed = 128.0f; // px/s（調整可）

	// Timestamp of the last Update() call, used for delta-time calculation
	// (JP: 最後のUpdate()呼び出し時刻。デルタタイム計算に使う)
	unsigned int m_lastTime = 0;

	// Fullness gauge value (0 ~ MAX_FULLNES)
	// Increases when a pizza is successfully crafted
	// (JP: 満腹ゲージ値(0〜MAX_FULLNES)。ピザ完成時に増加する)
	float m_fullness = 0.0f;
	
	// Remaining effect duration for each pizza type (seconds)
	// While > 0, the next bullet fired will use that pizza's image and damage
	// (JP: 各ピザ効果の残り秒数。0より大きい間は次の弾がそのピザ種別になる)
	PizzaTimer m_pizzaTimers;

	// Pointer to the shared pizza recipe/damage database (not owned)
    // (JP: 共有ピザレシピ・ダメージDBへのポインタ（所有しない）)
	PizzaDatabase* m_db = nullptr;

public:
	// Maximum value of the fullness gauge
	// (JP: 満腹ゲージの最大値)
	static constexpr float MAX_FULLNES = 100.0f;

	// Loads images and sets the player's starting position
	// Must be called once before Update()
	// (JP: 画像を読み込み、初期位置を設定する。Update()より前に1回呼ぶ)
	void Initialisation();

	// Injects the shared pizza database (called before Initialisation)
	// (JP: 共有ピザDBを注入する。Initialisation()より前に呼ぶ)
	void SetDatabase(PizzaDatabase* db) { m_db = db; }

	// Reads input, moves the player, handles collision, and triggers shooting
	// dt is derived internally from GetNowCount()
	// (JP: 入力を読み、移動・衝突処理・射撃トリガーを行う。dtはGetNowCount()から内部計算)
	void Update(const BackScreen& stage, Bllent_Managiment& bllent);

	// Returns current tile X coordinate (pixel X / 32)
	// Used for item pickup proximity checks
	// (JP: 現在のタイルX座標(ピクセルX/32)。アイテム拾い近接チェックに使う)
	int GetX() const { return static_cast<int>(Player_MovePointX) / 32; }

	// Returns current tile Y coordinate (pixel Y / 32)
	// (JP: 現在のタイルY座標)
	int GetY() const { return static_cast<int>(Player_MovePointY) / 32; }

	// Returns current pixel X position (used for drawing and bullet spawn)
	// (JP: 現在のピクセルX座標。描画と弾の発射位置に使う)
	float GetXf() const { return Player_MovePointX; } // px

	// Returns current pixel Y position
    // (JP: 現在のピクセルY座標)
	float GetYf() const { return Player_MovePointY; } // px

	// Returns the sprite handle to draw this frame
	// (JP: このフレームで描画するスプライトハンドルを返す)）
	int Get_PlayerHanadle() const { return Player_Handle; }
	int Get_PlayerHandle() const { return Player_Handle; } // alias for typo compatibility

	// Returns a copy of the current ingredient inventory
   // (JP: 現在の食材所持数のコピーを返す)
	Item_count Get_Player_Itembring() const { return Player_Itembring; };

	// Increments the counter for the picked-up ingredient type
	// (JP: 拾ったアイテム種別のカウンタを1増やす)
	void Player_BringItem(Item_number picked);

	// Returns the direction the player is currently facing
	// (JP: プレイヤーが現在向いている方向を返す)
	int GetDir() const { return m_dir; };

	// Returns the sprite draw width in pixels (also used as hit-box reference)
	// (JP: スプライト描画幅(px)を返す。ヒットボックスの基準にも使う)
	int Get_PlayerDisplaySize() const { return m_displaySize; }

	// Tries to craft a pizza from the current inventory using the database
	// If successful, consumes ingredients, increases fullness, and returns the pizza type
	// Returns PizzaType::None if no recipe is satisfied
	// (JP: 現在の所持食材でピザを作れるかDBに問い合わせる。成功すれば食材消費・満腹増加・ピザ種別を返す)
	PizzaType TryMakePizza();

	// Tries to craft a pizza from the current inventory using the database
	// If successful, consumes ingredients, increases fullness, and returns the pizza type
    // Returns PizzaType::None if no recipe is satisfied
    // (JP: 現在の所持食材でピザを作れるかDBに問い合わせる。成功すれば食材消費・満腹増加・ピザ種別を返す)
    void Debug_SetItems(int dough, int tomato, int cheese, int gorgonzola, int basil);

	// Returns the current fullness gauge value
	// (JP: 現在の満腹ゲージ値を返す)
	float GetFullness()const { return m_fullness; };
	
	// Returns a const reference to the pizza effect timers
	// Read by BllentManagiment to decide which pizza image/damage to use for the next shot
	// (JP: ピザ効果タイマーへのconst参照を返す。BllentManagimentが次弾の種別決定に参照する)
	const PizzaTimer& GetPizzaTimer()const { return m_pizzaTimers; };

	// Debug helper: directly sets ingredient counters without picking up items
	// (JP: デバッグ用。アイテムを拾わずに食材カウンタを直接セットする)
	void Debug_SetItems(int dough, int tomato, int cheese, int gorgonzola, int basil);


};
