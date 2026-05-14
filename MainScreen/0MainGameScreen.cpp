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
	// Enable windowed mode (JP: )
	ChangeWindowMode(true);
	// Set window size (JP: ???????????)
	SetGraphMode(1280, 736, 32);
	// Initialize DxLib (JP: DxLib????)
	if (DxLib_Init() < 0)
	{
		return -1;
	}
	// Create manager instances (JP: マネジャーインスタンスの作成)
	Player_Managiment player;
	Item_Managiment item;
	Enemy_Managiment enemy;
	Bllent_Managiment bllet;
	DrawManager draw;
	BackScreen stage;

	// Initialize game objects (JP: ゲームオブジェクトの初期化)
	player.Initialisation();
	item.ItemManagiment();
	enemy.Enemy_Initialisation(4,2);
	bllet.Load  ();
	stage.Initialize();
	
	// Start main loop (JP: メインループ)
	// Replace with proper loop condition as needed (JP: )
	while(1)
	{
		if (ProcessMessage() != 0)break;
		ClearDrawScreen();
		//更新
		player.Update(stage, bllet);
		bllet.Update(stage, player);
		enemy.Enemy_Update();
		// Display the map (JP: マップを表示)
		draw.Map_Draw(stage);
		draw.Player_Draw(stage, player);
		draw.Enemy_Draw(enemy,stage);
		
		//player movement（プレイヤーを動く）
		player.Update(stage, bllet);
		

		//bullet draw（弾を表示）
		draw.Bullets_Draw(bllet);
		//screen flip
		ScreenFlip();
	}
	// Finalize DxLib (JP: )
	DxLib_End();

	// End application (JP: ??????????)
	return 0;
}
