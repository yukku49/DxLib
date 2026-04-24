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
	
	float x, y;//プレイヤー座標
	float vx, vy;//速度
	int using_handle;//使う画像を収納
	int timer;//生存時間
	bool isActive;//生存フラグ
};

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
	//画面に出せる球数を外部で使えるようにする
	int GetMaxBullets()const { return Max_Bullets; };
	//弾のハンドルを外部で使えようにする
	const BulletData& Get_Bullethandle(int index)const { return m_bullets[index] };
	//弾の画像を外部で使えるようにする
	const int Get_Bullet_Image()const { return now_bllet_Handle; };
	
	//弾の画像を読み込む
	void Load();
	//弾の軌道
	void Update(BackScreen& stage, Player_Managiment& player);
	//弾を収納
	void Shot(float x, float y, Player_Managiment& player);
};

