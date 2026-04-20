#pragma once
#include "DxLib.h"
//アイテムの管理する列挙体
enum Item_number
{
	TOMATO,
	BASIL,
	CHEESE,
	GORGONZOLA,
	PIZZADOUGH,
	ITEM_MAX
};
struct ItemData
{
	int x, y;//ステージ上のマス目座標
	Item_number type;
	int aliveTimer;//消滅タイマー
	bool isActive;
};
class BackGrandManagiment;
class PlayerManagiment;

class Item_Managiment
{

private:
	//アイテムを収納するItem_Handle
	int Item_Handle[ITEM_MAX];
	//同時画面に出せるアイテムの最大数
	static const int MAX_SPAWN = 5;
	//画面上に存在するアイテムの実体
	ItemData m_items[MAX_SPAWN];
public:
	//初期化
	void ItemManagiment();
	//画像の読み込み
	void Load();
	//アイテムをランダムな位置に出現させる
	void Spawn(const BackGrandManagiment& stage, Item_number type);
	//タイマーの更新処理
	void Updata();
	//プレイヤーが拾ったかの判定（拾ったらItem_numberを返す)
	Item_number CheckPickUp(PlayerManagiment& player);
};

