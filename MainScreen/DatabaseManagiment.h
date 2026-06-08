#pragma once
#include"BllentManagiment.h"
#include"PlayerManagiment.h"
#include"sqlite3.h"

class DatabaseManagiment
{
private:
	sqlite3* db;
	void* m_db = nullptr;//仮
public:
	bool Initialize(const char* filse);//DB開く・テーブル作成
	void Close();
	//弾データ保存/読み込み
	bool SaveBulletData(const BulletData& bullets, int count);
	bool LoadBulletData(BulletData* bullets, int count);

	//素材カウント保存/読込
	bool SaveItemCount(const Item_count& items);
	bool LoadItemCount(Item_count& items);

	//ピザステータス保存/読込
	bool SavePizzaStatus(int pizzaType, int score);
	bool LoadPizzaStatus(int pizzaType, int& score);
};

