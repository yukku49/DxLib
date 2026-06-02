#pragma once
#include<string>

//セレクト項目
const char* string_select[];


class StartScreen
{
	bool isStartScreenActiveflag = true;//start Scrren active flag(JP:スタート画面を表示するのフラグ）
	//カーソル位置
	int cursorX = 0;
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

	//文字配列の宣言
	

	//Getter

	//フラグを返す
	bool Get_Start_Flog()const { return isStartScreenActiveflag; };

	
	

};

