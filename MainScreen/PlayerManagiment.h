#pragma one
#include "DrawManagiment.h"
enum Player_EyeContact
{
	PlayerEye_Up,
	PlayerEye_Down,
	PlayerEye_Left,
	PlayerEye_Right
};
struct Item_count
{
	int Tmato_Counter;
	int Basil_Counter;
	int Cheese_Counter;
	int Gorgonzola_Counter;
	int Pizzadough_Counter;
};
class ItemManagiment;
class BllentManagiment;
class BackScreenManagiment;

class Player_Managiment
{
private:
	//プレイヤーの初期位置変数
	const float Player_StanderdpointX = 1;
	const float Player_StanderdpointY = 1;

	//プレイヤーを動かす際に変化させる変数
	float Player_MovePointX = 1.0f;
	float Player_MovePointY = 1.0f;

	//プレイヤー画像を取得するハンドル

	int PlayerImage_Handle[Player_EyeContact::PlayerEye_Right];

	//プレイヤー画像を出力するための画像を受け取るハンドル
	int Player_Handle;

	const int m_displaySize = 28;//描画サイズ

	//押しっぱなし秒使用のプラグ
	int m_oldUp, m_oldDown, m_oldLeft, m_oldRight;

	//プレイヤーがもつ材料の構造体
	Item_count Player_Itembring;
public:
	void Initialisation();
	//StageManagerを仮引数として受け取り、壁判定を行う
	void Update(const BackScreen& stage, Bllent_Managiment& bllent);
	//他クラスから座標を強いりたいとき陽のゲッター（読み取り専用）
	int GetX()const { return Player_MovePointX; }
	int GetY()const { return Player_MovePointY; }
	int Get_PlayerHanadle()const { return Player_Handle; }
	//アイテムのカウンターを上げ下げする
	Item_count Player_BringItem(Item_Managiment& item);
	//Drawヘッダーファイルのプレイヤーを描画する関数を持ってくる
	//void Draw_Managiment::DrawManager();
};
