#pragma once
#include "DxLib.h"
#include "BackScreenManagiment.h"
#include "PlayerManagiment.h"
#include "bllentManagiment.h"
#include"EnemyManagiment.h"
class DrawManager
{
	//ƒvƒŒƒCƒ„[•`‰æ‚ğˆ—‚·‚é
	void Player_Draw(const BackScreenManagiment::StageManager& stage, const PlayerManagiment& player);

	//áŠQ•¨•`‰æ‚ğˆ—‚·‚é
	void Object_Draw(const BackScreenManagiment::StageManager& object);

	//’e‚ğ•`‰æ‚ğˆ—‚·‚é
	void Bullets_Draw(const BllentManagiment::Bllent_Managiment& bullets);

	//“G‚ğ•`‰æ‚ğˆ—‚·‚é
	void Enemy_Draw(const EnemyManagiment::Enemy_Managiment& enemy);

};