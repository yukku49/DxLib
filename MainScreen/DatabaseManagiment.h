#pragma once
#include "sqlite3.h"
#include "ItemTypes.h"      // Item_number 列挙
#include "PlayerManagiment.h" // Item_count, PizzaType, PizzaTimer

// -------------------------------------------------------
// ピザ1種のマスタデータ（Bllet_tableから読み込む）
// -------------------------------------------------------
struct PizzaStatus
{
    int    bllet_ID;       // PK (PizzaType と同値で使う)
    char   pizza_name[32]; // ピザ名
    int    pizza_damage;   // ダメージ量
    float  active_time;    // 有効秒数 (秒→フレームはゲーム側で変換)
    bool   active_flag;    // 現在有効か
};

// -------------------------------------------------------
// レシピ1行分のデータ（Recipe_tableから読み込む）
//   1レシピ = 1ピザ × 複数素材 のため複数行になる
// -------------------------------------------------------
struct RecipeRow
{
    int recipe_ID;   // PK
    int bllet_ID;    // FK → Bllet_table
    int item_ID;     // FK → Item_table  (Item_number の値)
    int item_count;  // 必要数
};

// -------------------------------------------------------
// fat_gauge テーブルの1行分
// -------------------------------------------------------
struct FatRecord
{
    int   fat_ID;     // PK
    int   recipe_ID;  // FK → Recipe_table
    float fat_meter;  // 満腹ゲージの増加量
};

// -------------------------------------------------------
// ピザ関連DBを一括管理するクラス
// -------------------------------------------------------
class PizzaDatabase
{
public:
    PizzaDatabase() : m_db(nullptr) {}
    ~PizzaDatabase() { Close(); }

    // ---- 接続 / 切断 ----

    // DBファイルを開き、なければテーブルを作成して初期データを挿入する
    bool Initialize(const char* filePath = "pizza_data.db");
    void Close();

    // ---- Bllet_table (ピザステータス) ----

    // 全ピザのステータスをDBから読み込み out[] に格納する（戻り値: 読み込んだ件数）
    int  LoadAllPizzaStatus(PizzaStatus out[], int maxCount) const;

    // 1件だけ更新（active_flag を書き換えるときなど）
    bool UpdatePizzaActiveFlag(int bllet_ID, bool active);

    // ---- Item_table (素材マスタ) ----

    // 素材名をDBから取得（デバッグ表示・UI用）
    bool GetItemName(Item_number id, char* outName, int bufLen) const;

    // ---- Recipe_table ----

    // 指定ピザのレシピ行を全部取得する（戻り値: 取得件数）
    int  LoadRecipeForPizza(int bllet_ID, RecipeRow out[], int maxRows) const;

    // ---- fat_gauge ----

    // 現在の満腹メーターをDBに保存する（セーブ時に呼ぶ）
    bool SaveFatMeter(int recipe_ID, float fatMeter);

    // 最新の満腹メーターをDBから読み込む（ロード時に呼ぶ）
    bool LoadFatMeter(int recipe_ID, float& outFat) const;

    // ---- ゲームとの連携ヘルパー ----

    // TryMakePizza() の成功後に呼ぶ
    //   → 使ったレシピに対応する fat_gauge を fat_meter 分加算して保存する
    //   → 戻り値: 加算後の合計満腹値（Player 側で m_fullness に代入する）
    float OnPizzaMade(PizzaType type, float currentFat);

    // 起動時に一度だけ呼ぶ：DBのレシピ定義を TryMakePizza() と一致しているか確認
    // （デバッグ用。不一致なら OutputDebugStringA で警告を出す）
    void  DebugValidateRecipes() const;

private:
    sqlite3* m_db;

    // テーブルが存在しなければ CREATE TABLE する
    bool CreateTablesIfNeeded();

    // 初期マスタデータを INSERT する（既に存在する行はスキップ）
    bool InsertMasterData();

    // PizzaType → bllet_ID の変換（enum の int 値をそのまま使う）
    static int PizzaTypeToID(PizzaType t) { return static_cast<int>(t); }
};