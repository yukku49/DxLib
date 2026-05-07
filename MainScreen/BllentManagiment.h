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
	
	float x, y;// Position (JP: ichi)
	float vx, vy;// Velocity (JP: sokudo)
	int using_handle;// Sprite handle in use (JP: shiyo handle)
	int timer;// Lifetime timer (JP: seizon timer)
	bool isActive;// Active flag (JP: yuko flag)
};

class Bllent_Managiment
{
	// Maximum bullets on screen (JP: saidai dango su)
	static const int Max_Bullets = 25;
	// Bullet sprite handle array (JP: dan image handle hairetsu)
	int bllet_Handle[bllet_number::BLLET_MAX];
	// Currently selected bullet handle (JP: sentaku chu handle)
	int now_bllet_Handle;
	// Bullet data array (JP: dan data hairetsu)
	BulletData m_bullets[Max_Bullets];
public:
	// Getter for max bullets (JP: saidai su getter)
	int GetMaxBullets()const { return Max_Bullets; };
	// Getter for bullet data by index (JP: index shitei getter)
	const BulletData& Get_Bullethandle(int index)const { return m_bullets[index]; };
	// Getter for current bullet image handle (JP: genzai image getter)
	const int Get_Bullet_Image()const { return now_bllet_Handle; };
	
	// Load bullet images (JP: dan image load)
	void Load();
	// Update bullet movement and collisions (JP: ido to collision)
	void Update(BackScreen& stage, Player_Managiment& player);
	// Fire a bullet (JP: dan hasha)
	void Shot(float x, float y, Player_Managiment& player);
};

