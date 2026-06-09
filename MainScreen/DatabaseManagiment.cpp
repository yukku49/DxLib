#include "DatabaseManagiment.h"
#include "sqlite3.h"
#include <cstring>
#include <Windows.h>   // OutputDebugStringA

// ============================================================
//  接続 / 切断
// ============================================================

bool PizzaDatabase::Initialize(const char* filePath)
{
    if (sqlite3_open(filePath, &m_db) != SQLITE_OK)
    {
        OutputDebugStringA("PizzaDatabase: DBオープン失敗\n");
        m_db = nullptr;
        return false;
    }

    // 外部キー制約を有効化
    sqlite3_exec(m_db, "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr);

    if (!CreateTablesIfNeeded()) return false;
    if (!InsertMasterData())     return false;

    return true;
}

void PizzaDatabase::Close()
{
    if (m_db)
    {
        sqlite3_close(m_db);
        m_db = nullptr;
    }
}

// ============================================================
//  テーブル作成
// ============================================================

bool PizzaDatabase::CreateTablesIfNeeded()
{
    const char* sql =

        // --- 素材マスタ ---
        "CREATE TABLE IF NOT EXISTS Item_table ("
        "  Item_ID   INTEGER PRIMARY KEY,"
        "  Item_name TEXT    NOT NULL"
        ");"

        // --- ピザ（弾）マスタ ---
        "CREATE TABLE IF NOT EXISTS Bllet_table ("
        "  Bllet_ID     INTEGER PRIMARY KEY,"
        "  pizza_name   TEXT    NOT NULL,"
        "  pizza_damage INTEGER NOT NULL DEFAULT 1,"
        "  active_time  REAL    NOT NULL DEFAULT 0.0,"
        "  active_flag  INTEGER NOT NULL DEFAULT 0"  // 0=false 1=true
        ");"

        // --- レシピ（ピザ×素材×必要数）---
        "CREATE TABLE IF NOT EXISTS Recipe_table ("
        "  recipe_ID  INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  Bllet_ID   INTEGER NOT NULL REFERENCES Bllet_table(Bllet_ID),"
        "  Item_ID    INTEGER NOT NULL REFERENCES Item_table(Item_ID),"
        "  item_count INTEGER NOT NULL DEFAULT 1"
        ");"

        // --- 満腹ゲージ ---
        "CREATE TABLE IF NOT EXISTS fat_gauge ("
        "  fat_ID    INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  recipe_ID INTEGER NOT NULL REFERENCES Recipe_table(recipe_ID),"
        "  fat_meter REAL    NOT NULL DEFAULT 0.0"
        ");";

    char* errMsg = nullptr;
    if (sqlite3_exec(m_db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK)
    {
        OutputDebugStringA("PizzaDatabase: テーブル作成失敗: ");
        OutputDebugStringA(errMsg);
        OutputDebugStringA("\n");
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

// ============================================================
//  初期マスタデータ挿入
//  INSERT OR IGNORE で二重挿入を防ぐ
// ============================================================

bool PizzaDatabase::InsertMasterData()
{
    const char* sql =

        // ---- Item_table ----
        // ItemTypes.h の Item_number 列挙に合わせた ID
        "INSERT OR IGNORE INTO Item_table(Item_ID, Item_name) VALUES"
        "  (0, 'トマト'),"
        "  (1, 'バジル'),"
        "  (2, 'チーズ'),"
        "  (3, 'ゴルゴンゾーラ'),"
        "  (4, 'ピザ生地');"

        // ---- Bllet_table ----
        // PizzaType 列挙の値に合わせた ID
        // active_time は PlayerManagiment.cpp の TryMakePizza() と揃える（秒単位）
        "INSERT OR IGNORE INTO Bllet_table"
        "  (Bllet_ID, pizza_name,       pizza_damage, active_time, active_flag) VALUES"
        "  (1, 'マルゲリータ',          3,            30.0,        0),"
        "  (2, 'クアトロフォルマッジ',  5,            40.0,        0),"
        "  (3, 'マリナーラ',            2,            10.0,        0),"
        "  (4, 'ジェノベーゼ',          3,            30.0,        0),"
        "  (5, 'セージ',                1,             0.0,        0);"

        // ---- Recipe_table ----
        // マルゲリータ: トマト×2, チーズ×3, バジル×3, 生地×1
        "INSERT OR IGNORE INTO Recipe_table(recipe_ID, Bllet_ID, Item_ID, item_count) VALUES"
        "  (1,  1, 0, 2),"   // マルゲリータ - トマト 2
        "  (2,  1, 2, 3),"   // マルゲリータ - チーズ 3
        "  (3,  1, 1, 3),"   // マルゲリータ - バジル 3
        "  (4,  1, 4, 1),"   // マルゲリータ - 生地   1

        // クアトロフォルマッジ: チーズ×3, ゴルゴン×3, 生地×1
        "  (5,  2, 2, 3),"
        "  (6,  2, 3, 3),"
        "  (7,  2, 4, 1),"

        // マリナーラ: トマト×3, 生地×1
        "  (8,  3, 0, 3),"
        "  (9,  3, 4, 1),"

        // ジェノベーゼ: トマト×2, バジル×2, チーズ×2, 生地×1
        "  (10, 4, 0, 2),"
        "  (11, 4, 1, 2),"
        "  (12, 4, 2, 2),"
        "  (13, 4, 4, 1);"

        // fat_gauge の初期データは「プレイヤーが初めてピザを作ったとき」に挿入するため
        // ここでは INSERT しない

        ;

    char* errMsg = nullptr;
    if (sqlite3_exec(m_db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK)
    {
        OutputDebugStringA("PizzaDatabase: マスタ挿入失敗: ");
        OutputDebugStringA(errMsg);
        OutputDebugStringA("\n");
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

// ============================================================
//  Bllet_table 操作
// ============================================================

int PizzaDatabase::LoadAllPizzaStatus(PizzaStatus out[], int maxCount) const
{
    if (!m_db || !out || maxCount <= 0) return 0;

    const char* sql =
        "SELECT Bllet_ID, pizza_name, pizza_damage, active_time, active_flag "
        "FROM Bllet_table ORDER BY Bllet_ID;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return 0;

    int count = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW && count < maxCount)
    {
        out[count].bllet_ID = sqlite3_column_int(stmt, 0);
        // pizza_name を安全にコピー
        const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        strncpy_s(out[count].pizza_name, sizeof(out[count].pizza_name),
            name ? name : "", _TRUNCATE);
        out[count].pizza_damage = sqlite3_column_int(stmt, 2);
        out[count].active_time = static_cast<float>(sqlite3_column_double(stmt, 3));
        out[count].active_flag = sqlite3_column_int(stmt, 4) != 0;
        ++count;
    }
    sqlite3_finalize(stmt);
    return count;
}

bool PizzaDatabase::UpdatePizzaActiveFlag(int bllet_ID, bool active)
{
    if (!m_db) return false;

    const char* sql =
        "UPDATE Bllet_table SET active_flag = ? WHERE Bllet_ID = ?;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_int(stmt, 1, active ? 1 : 0);
    sqlite3_bind_int(stmt, 2, bllet_ID);

    bool ok = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return ok;
}

// ============================================================
//  Item_table 操作
// ============================================================

bool PizzaDatabase::GetItemName(Item_number id, char* outName, int bufLen) const
{
    if (!m_db || !outName || bufLen <= 0) return false;

    const char* sql =
        "SELECT Item_name FROM Item_table WHERE Item_ID = ?;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_int(stmt, 1, static_cast<int>(id));

    bool ok = false;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        strncpy_s(outName, bufLen, name ? name : "", _TRUNCATE);
        ok = true;
    }
    sqlite3_finalize(stmt);
    return ok;
}

// ============================================================
//  Recipe_table 操作
// ============================================================

int PizzaDatabase::LoadRecipeForPizza(int bllet_ID, RecipeRow out[], int maxRows) const
{
    if (!m_db || !out || maxRows <= 0) return 0;

    const char* sql =
        "SELECT recipe_ID, Bllet_ID, Item_ID, item_count "
        "FROM Recipe_table WHERE Bllet_ID = ?;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return 0;

    sqlite3_bind_int(stmt, 1, bllet_ID);

    int count = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW && count < maxRows)
    {
        out[count].recipe_ID = sqlite3_column_int(stmt, 0);
        out[count].bllet_ID = sqlite3_column_int(stmt, 1);
        out[count].item_ID = sqlite3_column_int(stmt, 2);
        out[count].item_count = sqlite3_column_int(stmt, 3);
        ++count;
    }
    sqlite3_finalize(stmt);
    return count;
}

// ============================================================
//  fat_gauge 操作
// ============================================================

bool PizzaDatabase::SaveFatMeter(int recipe_ID, float fatMeter)
{
    if (!m_db) return false;

    // 同じ recipe_ID が既にあれば UPDATE、なければ INSERT
    const char* sql =
        "INSERT INTO fat_gauge(recipe_ID, fat_meter) VALUES(?, ?)"
        "ON CONFLICT(recipe_ID) DO UPDATE SET fat_meter = excluded.fat_meter;";

    // fat_gauge の PK は AUTOINCREMENT なので recipe_ID に UNIQUE 制約が必要
    // → 初回のみ ALTER で追加（CreateTablesIfNeeded で対応済みなら不要）
    // ここでは UPSERT として INSERT OR REPLACE を使う簡易版
    const char* sqlUpsert =
        "INSERT OR REPLACE INTO fat_gauge(fat_ID, recipe_ID, fat_meter) "
        "VALUES("
        "  (SELECT fat_ID FROM fat_gauge WHERE recipe_ID = ?),"  // 既存ID or NULL
        "  ?, ?"
        ");";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(m_db, sqlUpsert, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_int(stmt, 1, recipe_ID);
    sqlite3_bind_int(stmt, 2, recipe_ID);
    sqlite3_bind_double(stmt, 3, static_cast<double>(fatMeter));

    bool ok = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return ok;
}

bool PizzaDatabase::LoadFatMeter(int recipe_ID, float& outFat) const
{
    if (!m_db) return false;

    const char* sql =
        "SELECT fat_meter FROM fat_gauge WHERE recipe_ID = ? "
        "ORDER BY fat_ID DESC LIMIT 1;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_int(stmt, 1, recipe_ID);

    bool ok = false;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        outFat = static_cast<float>(sqlite3_column_double(stmt, 0));
        ok = true;
    }
    sqlite3_finalize(stmt);
    return ok;
}

// ============================================================
//  ゲームとの連携ヘルパー
// ============================================================

float PizzaDatabase::OnPizzaMade(PizzaType type, float currentFat)
{
    if (!m_db) return currentFat;

    int blletID = PizzaTypeToID(type);

    // そのピザのレシピ先頭行の recipe_ID を取得（代表値として使う）
    const char* sql =
        "SELECT recipe_ID FROM Recipe_table WHERE Bllet_ID = ? LIMIT 1;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return currentFat;

    sqlite3_bind_int(stmt, 1, blletID);

    int recipeID = -1;
    if (sqlite3_step(stmt) == SQLITE_ROW)
        recipeID = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);

    if (recipeID < 0) return currentFat;

    // active_time を満腹増加量として使う（ゲームバランスは調整可能）
    PizzaStatus status[8];
    float gain = 0.0f;
    int n = LoadAllPizzaStatus(status, 8);
    for (int i = 0; i < n; i++)
    {
        if (status[i].bllet_ID == blletID)
        {
            gain = status[i].active_time * 0.1f; // active_time の 10% を満腹値に
            break;
        }
    }

    float newFat = currentFat + gain;
    if (newFat > 100.0f) newFat = 100.0f; // MAX_FULLNESS と合わせる

    // 保存
    SaveFatMeter(recipeID, newFat);

    return newFat;
}

// ============================================================
//  デバッグ：レシピ整合性チェック
// ============================================================

void PizzaDatabase::DebugValidateRecipes() const
{
    if (!m_db) return;

    // DB上の全レシピを取得して OutputDebugStringA に出力するだけ
    const char* sql =
        "SELECT r.recipe_ID, b.pizza_name, i.Item_name, r.item_count "
        "FROM Recipe_table r "
        "JOIN Bllet_table b ON r.Bllet_ID = b.Bllet_ID "
        "JOIN Item_table  i ON r.Item_ID  = i.Item_ID "
        "ORDER BY r.Bllet_ID, r.recipe_ID;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr) != SQLITE_OK) return;

    OutputDebugStringA("=== PizzaDatabase レシピ一覧 ===\n");
    char buf[256];
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int   rid = sqlite3_column_int(stmt, 0);
        const char* pizza = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        const char* item = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        int   cnt = sqlite3_column_int(stmt, 3);
        sprintf_s(buf, "  [recipe_%02d] %s <- %s x%d\n",
            rid,
            pizza ? pizza : "?",
            item ? item : "?",
            cnt);
        OutputDebugStringA(buf);
    }
    OutputDebugStringA("================================\n");
    sqlite3_finalize(stmt);
}