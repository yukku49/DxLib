#include "StartScreen.h"
#include"DxLib.h"

const char* string_select[] =
{ "play the game",
	"option",
	"exit",
	NULL
};

void StartScreen::SelectGames()
{
	while (string_select[selectNumber] != nullptr)
	{
		++selectNumber;
	}
	

	//モードに応じた処理をする
    // メンバ関数ポインタの配列に修正
    void (StartScreen::*P_func[])() =
    {
        &StartScreen::Play_the_game,
        &StartScreen::Option,
		& StartScreen::Exit
    };
}

void StartScreen::MoveCursor()
{
	//キーを取得
	int cursorUp = CheckHitKey(KEY_INPUT_UP);
	int cursorDown = CheckHitKey(KEY_INPUT_DOWN);
	int cursorEnter = CheckHitKey(KEY_INPUT_RETURN);
	//カーソルを動かす
	if(cursorUp==1&&m_oldUp==0)
	{
		cursorY -= 1;

	}
	if (cursorDown == 1&&m_oldDown==0)
	{
		cursorY += 1;
	}
	//EnterKeyが押されたらモードに応じた処理
	if (cursorEnter == 1)
	{
		if (cursorY == 0)Play_the_game();
		else if (cursorY == 1)Option();
		else if (cursorY == 2)Exit();
	}
	//前フレームの状態を保存
	m_oldUp = cursorUp;
	m_oldDown = cursorDown;

	//カーソルの位置を制限する
	if (cursorY < 0)
	{
		cursorY = 0;
	}
	if (cursorY > selectNumber)
	{
		cursorY = selectNumber;
	}
	//描画（32px間隔）
	int startX = 100;
	int startY = 200;
	for (int i = 0; i < selectNumber; i++)
	{
		int drawY = startY + i * 32;
		if (i == cursorY)
		{
			DrawBox(startX, drawY, startX + 32, drawY + 32, GetColor(255, 255, 255), true);
		}
		DrawString(startX + 40, drawY + 8, string_select[i], GetColor(255, 255, 255));
	}
	
}

void StartScreen::Play_the_game()
{
	
	isStartScreenActiveflag = false;
}

void StartScreen::Option()
{
	
}

void StartScreen::Exit()
{
	modeNumber = 3;
	PostQuitMessage(0);
	DxLib_End();
}