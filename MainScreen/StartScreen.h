#pragma once
class StartScreen
{
	bool isStartScreenActiveflag = true;//start Scrren active flag(JP:スタート画面を表示するのフラグ）
	//カーソル位置
	int cursorX = 0;
	int cursorY = 0;

public:
	//十字キーでカーソルを動かす
	void MoveCursor();

	void PlaytheGame();

};

