#include <iostream>
#include"DxLib.h"
#include"BackScreenManagiment.h"
#include"BllentManagiment.h"
#include"DrawManagiment.h"
#include"PlayerManagiment.h"
#include"EnemyManagiment.h"
#include"ItemManagiment.h"
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow) {
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
	Player_Managiment player;
	Item_Managiment item;
	Enemy_Managiment enemy;
	Bllent_Managiment bllent;
	DrawManager draw;
	BackScreen stage;

	//ゲーム初期化
	player.Initialisation();
	item.ItemManagiment();
	enemy.Enemy_Initialisation(4,2);
	bllent.Load();
	stage.Initialize();
	
	//ループ開始
	//while (ProcessMessage() == 0 && ClearDrawScreen() == 0)
	while(1)
	{
		draw.Map_Draw(stage);
		draw.Player_Draw(stage, player);
		
		player.Update(stage, bllent);

		

		ScreenFlip();
	}
	// DXライブラリ使用の終了処理
	DxLib_End();

	// ソフトの終了
	return 0;
}