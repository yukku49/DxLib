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
	//前フレームの状態を保存
	int m_oldUp = 0;
	int m_oldDown = 0;
	//セレクト項目数
	int selectNumber = 0;
	int selectopitionNumber = 0;
	
	//項目に応じた変数を受け取る
	int modeNumber= 0;

	

public:
	//Setter
	//itilize the start screen(JP : スタート画面の初期化)

	void startInitialize();

	//セレクト

	void SelectGames();

	//十字キーでカーソルを動かす
	void MoveCursor();

	void Play_the_game();
	void Option();
	void Exit();

	//オプションの中身
	
	void Option1();
	void Option2();
	void Option3();
	int returnOption() {return modeNumber = 0; };
	
	//文字配列の宣言
	

	//Getter

	//フラグを返す
	bool Get_Start_Flog()const { return isStartScreenActiveflag; };
	int Get_ModeNumber()const { return modeNumber; };

	
	

};

