#pragma once
#include "DxLib.h"
#include "BackScreenManagiment.h"
#include "PlayerManagiment.h"
#include "BllentManagiment.h"
#include"EnemyManagiment.h"
class DrawManager
{
	//ƒvƒŒƒCƒ„[•`‰æ‚ğˆ—‚·‚é
	void Player_Draw(const BackScreen& stage, const Player_Managiment& player);

	//áŠQ•¨•`‰æ‚ğˆ—‚·‚é
	void Object_Draw(const BackScreen& object);

	//’e‚ğ•`‰æ‚ğˆ—‚·‚é
	void Bullets_Draw(const Bllent_Managiment& bullets);

	//“G‚ğ•`‰æ‚ğˆ—‚·‚é
	void Enemy_Draw(const Enemy_Managiment& enemy, const BackScreen& stage)

};