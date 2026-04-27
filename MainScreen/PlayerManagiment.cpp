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
	//移動用変数に初期座標を代入
	Player_MovePointX = Player_StanderdpointX;
	Player_MovePointY = Player_StanderdpointY;

	//プレイヤー画像をハンドルに入れる
	PlayerImage_Handle[0] = LoadGraph("Pizza_Image/player_up.png");
	PlayerImage_Handle[1] = LoadGraph("Pizza_Image/player_down.png");
	PlayerImage_Handle[2] = LoadGraph("Pizza_Image/player_left.png");
	PlayerImage_Handle[3] = LoadGraph("Pizza_Image/player_right.png");

	DrawGraph(Player_StanderdpointX, Player_StanderdpointY, PlayerImage_Handle[PlayerEye_Left], TRUE);
}

void Player_Managiment::Update(const BackScreen& stage, Bllent_Managiment& bllent)
{
	//現在の前期―の状態を取得
	int nowUp = CheckHitKey(KEY_INPUT_W);
	int nowDown = CheckHitKey(KEY_INPUT_S);
	int nowLeft = CheckHitKey(KEY_INPUT_A);
	int nowRight = CheckHitKey(KEY_INPUT_D);


	//今押されていて、かつ前回は押されていな買った時だけ移動（一マス移動)
	if ((nowUp == 1 && m_oldUp == 0))
	{
		//上移動
		if (stage.GetMapvalue(Player_MovePointX, Player_MovePointY - 1) == 1)
		{
			Player_MovePointY--;
			this->Player_Handle = this->PlayerImage_Handle[PlayerEye_Up];
			this->m_dir =PlayerEye_Up;
		}
		//下移動
		if (nowDown == 1 && m_oldDown == 0)
		{
			if (stage.GetMapvalue(Player_MovePointX, Player_MovePointY + 1) == 1)
			{
				Player_MovePointY++;
				this->Player_Handle = this->PlayerImage_Handle[PlayerEye_Down];
				this->m_dir = PlayerEye_Down;
			}


		}
		if (nowLeft == 1 && m_oldLeft == 0)
		{
			if (stage.GetMapvalue(Player_MovePointX - 1, Player_MovePointY) == 1)
			{
				Player_MovePointX--;
				this->Player_Handle = this->PlayerImage_Handle[PlayerEye_Left];
				this->m_dir = PlayerEye_Left;
			}

		}
		if (nowRight == 1 && m_oldRight == 0)
		{
			if (stage.GetMapvalue(Player_MovePointX + 1, Player_MovePointY) == 1)
			{
				Player_MovePointX++;
				this->Player_Handle = this->PlayerImage_Handle[PlayerEye_Right];
				this->m_dir = PlayerEye_Right;

			}
		}
		if (CheckHitKey(KEY_INPUT_SPACE))
		{
			bllent.Shot(Player_MovePointX, Player_MovePointY, *this);
		}
		//次のフレームのために「今の状態」を「前回の状態」にコピー
		m_oldUp = nowUp;
		m_oldDown = nowDown;
		m_oldLeft = nowLeft;
		m_oldRight = nowRight;
	}

}
