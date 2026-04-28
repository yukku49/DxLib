#pragma once
// DrawManagiment.h をここで include しない（循環を避ける）
#include <array>

enum Player_EyeContact
{
	PlayerEye_Up = 0,
	PlayerEye_Down,
	PlayerEye_Left,
	PlayerEye_Right,
	PlayerEye_Max
};

struct Item_count
{
	int Tmato_Counter;
	int Basil_Counter;
	int Cheese_Counter;
	int Gorgonzola_Counter;
	int Pizzadough_Counter;
};

class Item_Managiment;
class Bllent_Managiment;
class BackScreen; // 前方宣言（BackScreenManagiment.h のクラス名に合わせる）

class Player_Managiment
{
private:
	//プレイヤーの初期位置変数
	const float Player_StanderdpointX = 1.0f;
	const float Player_StanderdpointY = 1.0f;

	//プレイヤーを動かす際に変化させる変数
	float Player_MovePointX = 0;
	float Player_MovePointY = 0;

	//プレイヤー画像を取得するハンドル

	int PlayerImage_Handle[Player_EyeContact::PlayerEye_Max];

	//プレイヤー画像を出力するための画像を受け取るハンドル
	int Player_Handle;

	const int m_displaySize = 28;//描画サイズ

	//押しっぱなし秒使用のプラグ
	int m_oldUp, m_oldDown, m_oldLeft, m_oldRight;

	//現在プレイヤーが向いている方向
	int m_dir;

	//プレイヤーがもつ材料の構造体
	Item_count Player_Itembring;
public:
	void Initialisation();
	//StageManagerを仮引数として受け取り、壁判定を行う
	void Update(const BackScreen& stage, Bllent_Managiment& bllent);

	//他クラスから座標を強いりたいとき陽のゲッター（読み取り専用）
	int GetX()const { return (int)Player_MovePointX; }
	int GetY()const { return (int)Player_MovePointY; }

	//プレイヤーの進行方向の画像を返す
	int Get_PlayerHanadle()const { return Player_Handle; }

	//持っている材料を外部から読み取れるようにする
	Item_count Get_Player_Itembring()const { return Player_Itembring; };

	//アイテムのカウンターを上げ下げする
	void Player_BringItem(Item_Managiment& item);

	//プレイヤーの現在の向きを外部からアクセスできるGetter
	int  GetDir()const { return m_dir; };


	//Drawヘッダーファイルのプレイヤーを描画する関数を持ってくる
	//void Draw_Managiment::DrawManager();
};
