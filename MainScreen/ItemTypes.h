#pragma once
// Identifies each type of ingredient item that can be picked up on the map
// Used as array indices for Item_Handle[] and item spawning
// (JP: マップ上で拾える食材アイテムの種別。Item_Handle[]のインデックスにもなる)
enum Item_number
{
	TOMATO,
	BASIL,
	CHEESE,
	GORGONZOLA,
	PIZZADOUGH,
	ITEM_MAX// Sentinel value for array sizing and loop bounds (JP: 配列サイズ・ループ終端用)
};

// Identifies which type of pizza was crafted
// Used to look up damage and fullness values in PizzaDatabase
// (JP: 作られたピザの種別。PizzaDatabase でダメージ・満腹値を引くために使う)
enum class PizzaType
{
	None,// No pizza was made (recipe requirements not met) (JP: ピザ未完成)
	Margherita,
	QuattroFormaggi,
	Marinara,
	Genovese,
	Sage
};

// Holds the quantity of each ingredient the player (or enemy) currently carries
// (JP: プレイヤー（または敵）が現在所持している各食材の数量)
struct Item_count
{
	int Pizzadough_Counter = 0;
	int Tmato_Counter = 0;
	int Cheese_Counter = 0;
	int Gorgonzola_Counter = 0;
	int Basil_Counter = 0;
};

// Tracks how many seconds remain for each pizza's effect after being crafted
// While a timer is > 0 the next shot fired will use that pizza type
// (JP: 各ピザの効果が残っている秒数を追跡する。タイマーが0より大きい間は次の弾がそのピザ種別になる)
struct PizzaTimer
{
	float Marigherita = 0.0f;
	float QuattroFormaggi = 0.0f;
	float Genovese = 0.0f;
	float Marinara = 0.0f;
};
