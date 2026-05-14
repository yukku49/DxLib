#include "PlayerManagiment.h"
#include "ItemManagiment.h"
#include"BackScreenManagiment.h"
#include"BllentManagiment.h"

#include "DxLib.h"

void Player_Managiment::Player_BringItem(Item_Managiment& item)
{
	switch (item.Get_Item_number())
	{
	case (TOMATO):
	{
		Player_Itembring.Tmato_Counter++;
		break;
	}

	case (BASIL):
	{
		Player_Itembring.Basil_Counter++;
		break;
	}
	case (CHEESE):
	{
		Player_Itembring.Cheese_Counter++;
		break;
	}
	case (GORGONZOLA):
	{
		Player_Itembring.Gorgonzola_Counter++;
		break;
	}
	case (PIZZADOUGH):
	{
		Player_Itembring.Pizzadough_Counter++;
		break;
	}

	}
}

void Player_Managiment::Initialisation()
{
	// Set initial move position (JP: 初期移動位置を設定)
	Player_MovePointX = Player_StanderdpointX;
	Player_MovePointY = Player_StanderdpointY;

	// Load player images into handles (JP: プレイヤー画像をハンドルに読み込む)
	PlayerImage_Handle[0] = LoadGraph("../Pizza_Image/player_up.png");
	PlayerImage_Handle[1] = LoadGraph("../Pizza_Image/player_down.png");
	PlayerImage_Handle[2] = LoadGraph("../Pizza_Image/player_left.png");
	PlayerImage_Handle[3] = LoadGraph("../Pizza_Image/player_right.png");

	//initlaize player direction(プレイヤー向きを右向きで初期化)
	Player_Handle = PlayerImage_Handle[3];
	//initlaize player m_dir(プレイヤーの向きを外部に送るgetterのm_dirも右向きで初期化)
	m_dir = Player_EyeContact::PlayerEye_Right;
	// initlaize space bar state(スペースの前フレームの状態を初期化)
	m_oldSpace = 0;
}


void Player_Managiment::Update(const BackScreen& stage, Bllent_Managiment& bllent)
{
	// Read current key input states (JP: 現在のキー入力状態を取得)
	int nowUp = CheckHitKey(KEY_INPUT_W);
	int nowDown = CheckHitKey(KEY_INPUT_S);
	int nowLeft = CheckHitKey(KEY_INPUT_A);
	int nowRight = CheckHitKey(KEY_INPUT_D);
	int nowSpace = CheckHitKey(KEY_INPUT_SPACE);


	// Move only on new key press (one-tile step) (JP: 新規押下時のみ1マス移動)
	if ((nowUp == 1 && m_oldUp == 0))
	{
		// Move up (JP: 上へ移動)
		if (stage.GetMapvalue(Player_MovePointX, Player_MovePointY - 1) == 1)
		{
			Player_MovePointY--;
			this->Player_Handle = this->PlayerImage_Handle[PlayerEye_Up];
			this->m_dir =PlayerEye_Up;
		}
	}
		// Move down (JP: 下へ移動)
		if (nowDown == 1 && m_oldDown == 0)
		{
			if (stage.GetMapvalue(Player_MovePointX, Player_MovePointY + 1) == 1)
			{
				Player_MovePointY++;
				this->Player_Handle = this->PlayerImage_Handle[PlayerEye_Down];
				this->m_dir = PlayerEye_Down;
			}


		}
		// Move left (JP: 左へ移動)
		if (nowLeft == 1 && m_oldLeft == 0)
		{
			if (stage.GetMapvalue(Player_MovePointX - 1, Player_MovePointY) == 1)
			{
				Player_MovePointX--;
				this->Player_Handle = this->PlayerImage_Handle[PlayerEye_Left];
				this->m_dir = PlayerEye_Left;
			}

		}
		// Move right (JP: 右へ移動)
		if (nowRight == 1 && m_oldRight == 0)
		{
			if (stage.GetMapvalue(Player_MovePointX + 1, Player_MovePointY) == 1)
			{
				Player_MovePointX++;
				this->Player_Handle = this->PlayerImage_Handle[PlayerEye_Right];
				this->m_dir = PlayerEye_Right;

			}
		}
		if (nowSpace==1&&m_oldSpace==0)
		{
			bllent.Shot(Player_MovePointX, Player_MovePointY, *this);
		}
		// Save current state for next frame (JP: 次フレーム用に状態を保存)
		m_oldUp = nowUp;
		m_oldDown = nowDown;
		m_oldLeft = nowLeft;
		m_oldRight = nowRight;
		m_oldSpace = nowSpace;
	

}
