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
	// Enable windowed mode (JP: window mode on)
	ChangeWindowMode(true);
	// Set window size (JP: mado size settei)
	SetGraphMode(1280, 736, 32);
	// Initialize DxLib (JP: DxLib shokika)
	if (DxLib_Init() < 0)
	{
		return -1;
	}
	// Create manager instances (JP: manager instance sakusei)
	Player_Managiment player;
	Item_Managiment item;
	Enemy_Managiment enemy;
	Bllent_Managiment bllent;
	DrawManager draw;
	BackScreen stage;

	// Initialize game objects (JP: game object shokika)
	player.Initialisation();
	item.ItemManagiment();
	enemy.Enemy_Initialisation(4,2);
	bllent.Load();
	stage.Initialize();
	
	// Start main loop (JP: main loop kaishi)
	// Replace with proper loop condition as needed (JP: shuryo joken wa ato de)
	while(1)
	{
		// Display the map（マップを表示）
		draw.Map_Draw(stage);
		draw.Player_Draw(stage, player);
		
		//player movement
		player.Update(stage, bllent);

		//shot bllet
		if(CheckHitKey(KEY_INPUT_SPACE))
		{
			bllent.Shot(player.GetX(), player.GetY(), player);
			draw.Bullets_Draw(bllent);
		}
		//bullet update
		bllent.Update(stage, player);
		//bullet draw
		draw.Bullets_Draw(bllent);
		//screen flip
		ScreenFlip();
	}
	// Finalize DxLib (JP: DxLib shuryo)
	DxLib_End();

	// End application (JP: app shuryo)
	return 0;
}