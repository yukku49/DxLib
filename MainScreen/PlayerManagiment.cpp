#include "PlayerManagiment.h"
#include "ItemManagiment.h"
#include "DxLib.h"

Item_count PlayerManegiment::Player_BringItem(ItemManagiment& item)
{
	switch (item)
	{
	case (item.TOMATO):
	{
		Player_Itembring.Tmato_Counter++;
		break;
	}

	case (item.BASIL):
	{
		Player_Itembring.Basil_Counter++;
		break;
	}
	case (item.CHEESE):
	{
		Player_Itembring.Cheese_Counter++;
		break;
	}
	case (item.GORGONZOLA):
	{
		Player_Itembring.Gorgonzola_Counter++;
		break;
	}
	case (item.PIZZADOUGH):
	{
		Player_Itembring.Pizzadough_Counter++;
		break;
	}

	}
}

void PlayerManegiment::Initialisation()
{
	//移動用変数に初期座標を代入
	Player_MovePointX = Player_StanderdpointX;
	Player_MovePointY = Player_StanderdpointY;

	//プレイヤー画像をハンドルに入れる
	PlayerImage_Handle[0] = LoadGraph("Image/player_up.png");
	PlayerImage_Handle[1] = LoadGraph("Image/player_down.png");
	PlayerImage_Handle[2] = LoadGraph("Image/player_left.png");
	PlayerImage_Handle[3] = LoadGraph("Image/player_right.png");
}

void PlayerManegiment::Update(const BackGrand::StageManeger& stage, BllentManagiment& bllent)
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
		if (stage.GetMapValue(Player_MovePointX, Player_MovePointY - 1) == 1)
			Player_MovePointY--;

	}
	//下移動
	if (nowDown == 1 && m_oldDown == 0)
	{
		if (stage.GetMapValue(Player_MovePointX, Player_MovePointY + 1) == 1)
			Player_MovePointY++;


	}
	if (nowLeft == 1 && m_oldLeft == 0)
	{
		if (stage.GetMapvalue(Player_MovePointX - 1, Player_MovePointY) == 1)
			Player_MovePointX--;

	}
	if (nowRight == 1 && m_oldRight == 0)
	{
		if (stage.GetMapvalue(Player_MovePointX + 1, Player_MovePointY) == 1)
			Player_MovePointX++;

	}
	if (CheckHitKey(KEY_INPUT_SPACE))
	{
		bllent.shot(Player_MovePointX, Player_MovePointY);
	}
	//次のフレームのために「今の状態」を「前回の状態」にコピー
	m_oldUp = nowUp;
	m_oldDown = nowDown;
	m_oldLeft = nowLeft;
	m_oldRight = nowRight;

}
