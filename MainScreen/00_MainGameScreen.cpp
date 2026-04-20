#include <iostream>
#include"DxLib.h"
#include"BackScreenManagiment.h"
#include"BllentManagiment.h"
#include"DrawManagiment.h"
#include"PlayerManagiment.h"
#include"EnemyManagiment.h"
#include"ItemManagiment.h"
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// ウインドウモードで起動
	ChangeWindowMode(true);
	//ウインドウサイズを固定
	SetGraphMode(1280, 736, 32);
	// DXライブラリの初期化
	if (DxLib_Init() < 0)
	{
		return -1;
	}
	//インスタンスの作成
	PlayerManegiment player;
	ItemManagiment item;
	Enemy_Managiment enemy;
	//ループ開始
	while (ProcessMessage() == 0 && ClearDrawScreen() == 0)
	{

	}
	// DXライブラリ使用の終了処理
	DxLib_End();

	// ソフトの終了
	return 0;
}