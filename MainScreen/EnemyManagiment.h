#pragma once
#include<vector>
#include<utility>
// Sprite index for each facing direction of the enemy
// (JP: 敵の各向きに対応するスプライトインデックス)
enum enemy_Eye
{
	Enemy_Up,
	Enemy_Left,
	Enemy_Right,
	Enemy_Down,
	Enemy_Max
};

// Sprite index for each facing direction of the enemy
// (JP: 敵の各向きに対応するスプライトインデックス)
struct Enemy_date
{
	float enemy_X = 0.0f; // Current pixel X position (JP: 現在のピクセルX座標)
	float enemy_Y = 0.0f; // Current pixel Y position (JP: 現在のピクセルY座標)）
	float vx = 0.0f;      // X velocity (px per second) (JP: X方向速度(px/秒))
	float vy = 0.0f;      // Y velocity (px per second) (JP: Y方向速度(px/秒))
	int fullness_gauge = 0;// Fullness level 0~100; reaches 100 = enemy wins (JP: 満腹ゲージ0〜100。100で敵の勝利)
	bool isActive = false;// False when defeated (JP: 倒された場合はfalse)
	int Enemy_Eye_handlbe[enemy_Eye::Enemy_Max];// One handle per direction (JP: 向きごとの画像ハンドル)
};

class BackScreen; // Forward declaration for collision check
class PlayerManagiment;

// Manages enemy movement(BFS pathfinding), collision, and state
// (JP: 敵の移動(BFS経路探索)・衝突・状態を管理するクラス)
class Enemy_Managiment
{
private:
	// The single enemy's data (JP: 敵1体分のデータ)
	Enemy_date a;

	// Draw width in pixels, also used for hit-box (JP: 描画幅(px)。ヒットボックスにも使う)
	const int m_displaySize = 28;

	// BFS pathfinding state
	// (JP: BFS経路探索の状態)
	std::vector<std::pair<int, int>> m_path;// Ordered list of tile coordinates to walk (JP: 歩くタイル座標の順序付きリスト)
	
	// Index of the next tile to move toward (JP: 次に向かうタイルのインデックス)
	int m_pathIndex = 0;

	// Counts frames since the last path recalculation (JP: 最後の経路再計算からのフレーム数)
	int m_pathTimer = 0;

	// Recalculate path every 60 frames (~1 second at 60fps) (JP: 60フレームごとに経路を再計算)
	static const int PATH_INTERVAL = 60;

	// Calculates the shortest BFS path from startT* to goalT* through passable tiles
	// Stores the result in m_path (excludes the start tile, includes the goal tile)
	// (JP: 通行可能タイルを通るBFS最短経路をstartT*からgoalT*まで計算する)
	// (JP: 結果をm_pathに格納する。開始タイルは含まず、ゴールタイルは含む)
	void CalcPath(const BackScreen& stage,
		int startTx, int startTy,
		int goalTx, int goalTy);

	// Delta-time timestamp
	// (JP: デルタタイム用タイムスタンプ)
	unsigned int m_lastTime = 0;

public:
	// Initializes enemy position, velocity, and loads directional sprites
   // startX/startY are pixel coordinates including the 32px HUD offset
   // (JP: 敵の位置・速度を初期化し、方向別スプライトを読み込む)
   // (JP: startX/startYは32pxのHUDオフセットを含むピクセル座標)
	void Enemy_Initialisation(float startX, float startY);

	// Returns read-only access to the enemy's raw data struct
	// (JP: 敵の生データ構造体への読み取り専用アクセスを返す)
	const Enemy_date& Get_enemyPoint() const { return a; }

	// Simple movement update without collision (kept for compatibility)
	// (JP: 衝突なしの単純移動更新。互換性のために残している)
	void Enemy_Update();

	// Full update: BFS movement toward the player with tile collision detection
	// playerX/playerY are pixel coordinates of the player (including HUD offset)
	// (JP: 完全更新。プレイヤーへのBFS移動＋タイル衝突判定)
	// (JP: playerX/playerYはHUDオフセット込みのプレイヤーのピクセル座標)
	void Enemy_Update(const BackScreen& stage,float playerX,float playerY);


	// Full update: BFS movement toward the player with tile collision detection
	// playerX/playerY are pixel coordinates of the player (including HUD offset)
	// (JP: 完全更新。プレイヤーへのBFS移動＋タイル衝突判定)
	// (JP: playerX/playerYはHUDオフセット込みのプレイヤーのピクセル座標)

	float Get_enemyX() const { return a.enemy_X; }

	float Get_enemyY() const { return a.enemy_Y; }

	// Returns the correct sprite handle based on current movement direction
	// Returns -1 if the enemy is not active
	// (JP: 現在の移動方向に基づいて正しいスプライトハンドルを返す。非アクティブ時は-1)
	int Get_EnemyHandle() const;

	int Get_EnemyDisplaySize() const { return m_displaySize; }

	// Returns the correct sprite handle based on current movement direction
	// Returns -1 if the enemy is not active
	// (JP: 現在の移動方向に基づいて正しいスプライトハンドルを返す。非アクティブ時は-1)
	void OnHit() { a.isActive = false; }

	// Returns true while the enemy is alive and active
	// (JP: 敵が生きてアクティブな間はtrueを返す)
	bool Get_EnemyActive() const { return a.isActive; }
	
	// Debug: returns the number of tiles in the current calculated path
	// (JP: デバッグ用。現在計算済みの経路のタイル数を返す)
	int GetPathSize() const { return (int)m_path.size(); }
	// Debug: returns the tile coordinate at index i in the current path
    // (JP: デバッグ用。現在の経路のインデックスiのタイル座標を返す)
	std::pair<int, int> GetPath(int i) const { return m_path[i]; }
};