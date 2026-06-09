#pragma once
#include "sqlite3.h"
#include "ItemTypes.h"

struct PizzaStatus
{
	int    bllet_ID;
	char   pizza_name[32];
	int    pizza_damage;
	float  active_time;
	bool   active_flag;
};

struct RecipeRow
{
	int recipe_ID;
	int bllet_ID;
	int item_ID;
	int item_count;
};

struct FatRecord
{
	int   fat_ID;
	int   recipe_ID;
	float fat_meter;
};

class PizzaDatabase
{
public:
	PizzaDatabase() : m_db(nullptr) {}
	~PizzaDatabase() { Close(); }

	bool Initialize(const char* filePath = "pizza_data.db");
	void Close();
	bool IsInitialized() const { return m_db != nullptr; }

	int  LoadAllPizzaStatus(PizzaStatus out[], int maxCount) const;
	bool UpdatePizzaActiveFlag(int bllet_ID, bool active);
	bool GetItemName(Item_number id, char* outName, int bufLen) const;
	int  LoadRecipeForPizza(int bllet_ID, RecipeRow out[], int maxRows) const;
	bool SaveFatMeter(int recipe_ID, float fatMeter);
	bool LoadFatMeter(int recipe_ID, float& outFat) const;

	// DBレシピでピザ作成（成功時は材料消費＋タイマー設定）
	PizzaType TryMakePizza(Item_count& items, PizzaTimer& timers);

	// ピザ種別のダメージ・有効時間（秒）を取得
	int   GetPizzaDamage(PizzaType type) const;
	float GetPizzaActiveTime(PizzaType type) const;

	float OnPizzaMade(PizzaType type, float currentFat);
	void  DebugValidateRecipes() const;

private:
	sqlite3* m_db;

	bool CreateTablesIfNeeded();
	bool InsertMasterData();

	static int PizzaTypeToID(PizzaType t) { return static_cast<int>(t); }
	static PizzaType BlletIDToPizzaType(int bllet_ID);
};
