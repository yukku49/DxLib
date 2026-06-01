#pragma once
#include<string>
//セレクト項目
const char* string_select[] =
{ "play the game",
	"option",
	"exit",
	NULL
};

class StartScreen
{
	bool isStartScreenActiveflag = true;//start Scrren active flag(JP:スタート画面を表示するのフラグ）
	//カーソル位置
	
	int cursorY = 0;
	//セレクト項目数
	int selectNumber = 0;
	
	//項目に応じた変数を受け取る
	int modeNumber= 0;

public:
	//Setter
	
	//セレクト
	void SelectGames();

	//十字キーでカーソルを動かす
	void MoveCursor();

	void Play_the_game();
	void Option();
	void Exit();

	//Getter

	//フラグを返す
	bool Get_Start_Flog()const { return isStartScreenActiveflag; };

	
	

};

