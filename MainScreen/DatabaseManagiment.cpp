#include "DatabaseManagiment.h"
#include <cstring>
#include <Windows.h>

namespace
{
	int GetItemCount(const Item_count& items, int itemID)
	{
		switch (static_cast<Item_number>(itemID))
		{
		case TOMATO:      return items.Tmato_Counter;
		case BASIL:       return items.Basil_Counter;
		case CHEESE:      return items.Cheese_Counter;
		case GORGONZOLA:  return items.Gorgonzola_Counter;
		case PIZZADOUGH:  return items.Pizzadough_Counter;
		default:          return 0;
		}
	}

	void AddItemCount(Item_count& items, int itemID, int delta)
	{
		switch (static_cast<Item_number>(itemID))
		{
		case TOMATO:      items.Tmato_Counter += delta; break;
		case BASIL:       items.Basil_Counter += delta; break;
		case CHEESE:      items.Cheese_Counter += delta; break;
		case GORGONZOLA:  items.Gorgonzola_Counter += delta; break;
		case PIZZADOUGH:  items.Pizzadough_Counter += delta; break;
		default:          break;
		}
	}

	void SetPizzaTimer(PizzaTimer& timers, PizzaType type, float frames)
	{
		switch (type)
		{
		case PizzaType::Margherita:      timers.Marigherita = frames; break;
		case PizzaType::QuattroFormaggi: timers.QuattroFormaggi = frames; break;
		case PizzaType::Marinara:        timers.Marinara = frames; break;
		case PizzaType::Genovese:        timers.Genovese = frames; break;
		default: break;
		}
	}

	bool CanMakeRecipe(const Item_count& items, const RecipeRow* rows, int rowCount)
	{
		for (int i = 0; i < rowCount; ++i)
		{
			if (GetItemCount(items, rows[i].item_ID) < rows[i].item_count)
				return false;
		}
		return true;
	}

	void ConsumeRecipe(Item_count& items, const RecipeRow* rows, int rowCount)
	{
		for (int i = 0; i < rowCount; ++i)
			AddItemCount(items, rows[i].item_ID, -rows[i].item_count);
	}
}

// ============================================================
//  ??? / ??f
// ============================================================

bool PizzaDatabase::Initialize(const char* filePath)
{
    if (sqlite3_open(filePath, &m_db) != SQLITE_OK)
    {
        OutputDebugStringA("PizzaDatabase: DB?I?[?v?????s\n");
        m_db = nullptr;
        return false;
    }

    // ?O???L?[?????L????
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
//  ?e?[?u????
// ============================================================

bool PizzaDatabase::CreateTablesIfNeeded()
{
    const char* sql =

        // --- ?f??}?X?^ ---
        "CREATE TABLE IF NOT EXISTS Item_table ("
        "  Item_ID   INTEGER PRIMARY KEY,"
        "  Item_name TEXT    NOT NULL"
        ");"

        // --- ?s?U?i?e?j?}?X?^ ---
        "CREATE TABLE IF NOT EXISTS Bllet_table ("
        "  Bllet_ID     INTEGER PRIMARY KEY,"
        "  pizza_name   TEXT    NOT NULL,"
        "  pizza_damage INTEGER NOT NULL DEFAULT 1,"
        "  active_time  REAL    NOT NULL DEFAULT 0.0,"
        "  active_flag  INTEGER NOT NULL DEFAULT 0"  // 0=false 1=true
        ");"

        // --- ???V?s?i?s?U?~?f??~?K?v???j---
        "CREATE TABLE IF NOT EXISTS Recipe_table ("
        "  recipe_ID  INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  Bllet_ID   INTEGER NOT NULL REFERENCES Bllet_table(Bllet_ID),"
        "  Item_ID    INTEGER NOT NULL REFERENCES Item_table(Item_ID),"
        "  item_count INTEGER NOT NULL DEFAULT 1"
        ");"

        // --- ?????Q?[?W ---
        "CREATE TABLE IF NOT EXISTS fat_gauge ("
        "  fat_ID    INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  recipe_ID INTEGER NOT NULL UNIQUE REFERENCES Recipe_table(recipe_ID),"
        "  fat_meter REAL    NOT NULL DEFAULT 0.0"
        ");";

    char* errMsg = nullptr;
    if (sqlite3_exec(m_db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK)
    {
        OutputDebugStringA("PizzaDatabase: ?e?[?u???????s: ");
        OutputDebugStringA(errMsg);
        OutputDebugStringA("\n");
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

// ============================================================
//  ?????}?X?^?f?[?^?}??
//  INSERT OR IGNORE ???d?}????h??
// ============================================================

bool PizzaDatabase::InsertMasterData()
{
    const char* sql =

        // ---- Item_table ----
        // ItemTypes.h ?? Item_number ???????? ID
        "INSERT OR IGNORE INTO Item_table(Item_ID, Item_name) VALUES"
        "  (0, '?g?}?g'),"
        "  (1, '?o?W??'),"
        "  (2, '?`?[?Y'),"
        "  (3, '?S???S???]?[??'),"
        "  (4, '?s?U???n');"

        // ---- Bllet_table ----
        // PizzaType ???l??????? ID
        // active_time ?? PlayerManagiment.cpp ?? TryMakePizza() ???????i?b?P??j
        "INSERT OR IGNORE INTO Bllet_table"
        "  (Bllet_ID, pizza_name,       pizza_damage, active_time, active_flag) VALUES"
        "  (1, '?}???Q???[?^',          3,            30.0,        0),"
        "  (2, '?N?A?g???t?H???}?b?W',  5,            40.0,        0),"
        "  (3, '?}???i?[??',            2,            10.0,        0),"
        "  (4, '?W?F?m?x?[?[',          3,            30.0,        0),"
        "  (5, '?Z?[?W',                1,             0.0,        0);"

        // ---- Recipe_table ----
        // ?}???Q???[?^: ?g?}?g?~2, ?`?[?Y?~3, ?o?W???~3, ???n?~1
        "INSERT OR IGNORE INTO Recipe_table(recipe_ID, Bllet_ID, Item_ID, item_count) VALUES"
        "  (1,  1, 0, 2),"   // ?}???Q???[?^ - ?g?}?g 2
        "  (2,  1, 2, 3),"   // ?}???Q???[?^ - ?`?[?Y 3
        "  (3,  1, 1, 3),"   // ?}???Q???[?^ - ?o?W?? 3
        "  (4,  1, 4, 1),"   // ?}???Q???[?^ - ???n   1

        // ?N?A?g???t?H???}?b?W: ?`?[?Y?~3, ?S???S???~3, ???n?~1
        "  (5,  2, 2, 3),"
        "  (6,  2, 3, 3),"
        "  (7,  2, 4, 1),"

        // ?}???i?[??: ?g?}?g?~3, ???n?~1
        "  (8,  3, 0, 3),"
        "  (9,  3, 4, 1),"

        // ?W?F?m?x?[?[: ?g?}?g?~2, ?o?W???~2, ?`?[?Y?~2, ???n?~1
        "  (10, 4, 0, 2),"
        "  (11, 4, 1, 2),"
        "  (12, 4, 2, 2),"
        "  (13, 4, 4, 1);"

        // fat_gauge ??????f?[?^??u?v???C???[???????s?U???????????v??}????????
        // ??????? INSERT ?????

        ;

    char* errMsg = nullptr;
    if (sqlite3_exec(m_db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK)
    {
        OutputDebugStringA("PizzaDatabase: ?}?X?^?}?????s: ");
        OutputDebugStringA(errMsg);
        OutputDebugStringA("\n");
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

// ============================================================
//  Bllet_table ????
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
        // pizza_name ?????S??R?s?[
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
//  Item_table ????
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
//  Recipe_table ????
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
//  fat_gauge ????
// ============================================================

bool PizzaDatabase::SaveFatMeter(int recipe_ID, float fatMeter)
{
    if (!m_db) return false;

    // ???? recipe_ID ?????????? UPDATE?A?????? INSERT
    const char* sql =
        "INSERT INTO fat_gauge(recipe_ID, fat_meter) VALUES(?, ?)"
        "ON CONFLICT(recipe_ID) DO UPDATE SET fat_meter = excluded.fat_meter;";

    // fat_gauge ?? PK ?? AUTOINCREMENT ???? recipe_ID ?? UNIQUE ????K?v
    // ?? ?????? ALTER ?????iCreateTablesIfNeeded ?????????s?v?j
    // ??????? UPSERT ????? INSERT OR REPLACE ???g??????
    const char* sqlUpsert =
        "INSERT INTO fat_gauge(recipe_ID, fat_meter) VALUES(?, ?) "
        "ON CONFLICT(recipe_ID) DO UPDATE SET fat_meter = excluded.fat_meter;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(m_db, sqlUpsert, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_int(stmt, 1, recipe_ID);
    sqlite3_bind_double(stmt, 2, static_cast<double>(fatMeter));

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
//  ?Q?[?????A?g?w???p?[
// ============================================================

PizzaType PizzaDatabase::BlletIDToPizzaType(int bllet_ID)
{
    switch (bllet_ID)
    {
    case 1: return PizzaType::Margherita;
    case 2: return PizzaType::QuattroFormaggi;
    case 3: return PizzaType::Marinara;
    case 4: return PizzaType::Genovese;
    case 5: return PizzaType::Sage;
    default: return PizzaType::None;
    }
}

PizzaType PizzaDatabase::TryMakePizza(Item_count& items, PizzaTimer& timers)
{
    if (!m_db) return PizzaType::None;

    static const int kPriority[] = { 1, 2, 3, 4 };

    for (int blletID : kPriority)
    {
        RecipeRow rows[8];
        const int rowCount = LoadRecipeForPizza(blletID, rows, 8);
        if (rowCount <= 0 || !CanMakeRecipe(items, rows, rowCount))
            continue;

        ConsumeRecipe(items, rows, rowCount);

        const PizzaType type = BlletIDToPizzaType(blletID);
        const float activeSeconds = GetPizzaActiveTime(type);
        SetPizzaTimer(timers, type, activeSeconds * 60.0f);
        return type;
    }

    return PizzaType::None;
}

int PizzaDatabase::GetPizzaDamage(PizzaType type) const
{
    if (!m_db || type == PizzaType::None || type == PizzaType::Sage)
        return 1;

    PizzaStatus status[8];
    const int count = LoadAllPizzaStatus(status, 8);
    const int id = PizzaTypeToID(type);
    for (int i = 0; i < count; ++i)
    {
        if (status[i].bllet_ID == id)
            return status[i].pizza_damage;
    }
    return 1;
}

float PizzaDatabase::GetPizzaActiveTime(PizzaType type) const
{
    if (!m_db || type == PizzaType::None)
        return 0.0f;

    PizzaStatus status[8];
    const int count = LoadAllPizzaStatus(status, 8);
    const int id = PizzaTypeToID(type);
    for (int i = 0; i < count; ++i)
    {
        if (status[i].bllet_ID == id)
            return status[i].active_time;
    }
    return 0.0f;
}

float PizzaDatabase::OnPizzaMade(PizzaType type, float currentFat)
{
    if (!m_db) return currentFat;

    int blletID = PizzaTypeToID(type);

    // ????s?U????V?s???s?? recipe_ID ??????i??\?l?????g???j
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

    // active_time ?????????????g???i?Q?[???o?????X???????\?j
    PizzaStatus status[8];
    float gain = 0.0f;
    int n = LoadAllPizzaStatus(status, 8);
    for (int i = 0; i < n; i++)
    {
        if (status[i].bllet_ID == blletID)
        {
            gain = status[i].active_time * 0.1f; // active_time ?? 10% ????l??
            break;
        }
    }

    float newFat = currentFat + gain;
    if (newFat > 100.0f) newFat = 100.0f; // MAX_FULLNESS ???????

    // ???
    SaveFatMeter(recipeID, newFat);

    return newFat;
}

// ============================================================
//  ?f?o?b?O?F???V?s???????`?F?b?N
// ============================================================

void PizzaDatabase::DebugValidateRecipes() const
{
    if (!m_db) return;

    // DB???S???V?s????????? OutputDebugStringA ??o???????
    const char* sql =
        "SELECT r.recipe_ID, b.pizza_name, i.Item_name, r.item_count "
        "FROM Recipe_table r "
        "JOIN Bllet_table b ON r.Bllet_ID = b.Bllet_ID "
        "JOIN Item_table  i ON r.Item_ID  = i.Item_ID "
        "ORDER BY r.Bllet_ID, r.recipe_ID;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr) != SQLITE_OK) return;

    OutputDebugStringA("=== PizzaDatabase ???V?s?? ===\n");
    char buf[256];
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int   rid = sqlite3_column_int(stmt, 0);
        const char* pizza = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        const char* item = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        int   cnt = sqlite3_column_int(stmt, 3);
        printf_s(buf, "  [recipe_%02d] %s <- %s x%d\n",
            rid,
            pizza ? pizza : "?",
            item ? item : "?",
            cnt);
        OutputDebugStringA(buf);
    }
    OutputDebugStringA("================================\n");
    sqlite3_finalize(stmt);
}