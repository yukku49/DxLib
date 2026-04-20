#pragma once

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
	//
	float x, y;//プレイヤー座標
	float vx, vy;//速度
	int using_handle;//使う画像を収納
	int timer;//生存時間
	bool isActive;//生存フラグ
};
class PlayerManagiment;
class BackScreenManagiment;
class Bllent_Managiment
{
	//画面に出せる最大弾数
	static const int Max_Bullets = 25;
	//武器の画像を保持する配列
	int bllet_Handle[bllet_number::BLLET_MAX];
	//現在の打つものを入れるハンドル
	int now_bllet_Handle;
	//発射できる弾の数を入れるBlletData型の配列
	BulletData m_bullets[Max_Bullets];
public:
	//弾の画像を読み込む
	void Load();
	//弾の軌道
	void Update(BackScreenManagiment::StageManager& stage, PlayerManagiment::PlayerManagiment& player);
	//弾を収納
	void Shot(float x, float y, PlayerManagiment& player);
};

