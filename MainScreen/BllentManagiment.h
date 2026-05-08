#pragma once
#include"PlayerManagiment.h"
#include"BackScreenManagiment.h"
enum bllet_number
{
	GENOVESE,
	MARGHERITA,
	MARINARA,
	QUATTROFORMAGGI,
	SEAGE,
	BLLET_MAX
};
struct BulletData
{
	
	float x, y;// Position (JP: 位置)
	float vx, vy;// Velocity (JP: 速度)
	int using_handle;// Sprite handle in use (JP:外部ハンドル)
	int timer;// Lifetime timer (JP:生存時間)
	bool isActive;// Active flag (JP: 有効フラグ)
};

class Bllent_Managiment
{
	// Maximum bullets on screen (JP: 最大弾数)
	static const int Max_Bullets = 25;
	// Bullet sprite handle array (JP:弾画像ハンドル配列)
	int bllet_Handle[bllet_number::BLLET_MAX];
	// Currently selected bullet handle (JP: 選択中の弾ハンドル)
	int now_bllet_Handle;
	// Bullet data array (JP: 弾データ配列)
	BulletData m_bullets[Max_Bullets];
public:
	// Getter for max bullets (JP: 最大弾数ゲッター)
	int GetMaxBullets()const { return Max_Bullets; };
	// Getter for bullet data by index (JP: インデックスから弾データを取得)
	const BulletData& Get_Bullethandle(int index)const { return m_bullets[index]; };
	// Getter for current bullet image handle (JP: 現在の弾画像ハンドルを取得)
	const int Get_Bullet_Image()const { return now_bllet_Handle; };
	
	// Load bullet images (JP: 弾画像を読み込む)
	void Load();
	// Update bullet movement and collisions (JP: 移動と衝突を更新)
	void Update(BackScreen& stage, Player_Managiment& player);
	// Fire a bullet (JP: 弾を発射)
	void Shot(float x, float y, Player_Managiment& player);
};

