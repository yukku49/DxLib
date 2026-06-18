#include "ItemManagiment.h"
#include "BackScreenManagiment.h"
#include "PlayerManagiment.h"

// Sets every item slot to inactive so the pool is clean before the first Spawn() call
// (JP: 最初のSpawn()呼び出し前にプールがクリーンになるよう全スロットを非アクティブにする)
void Item_Managiment::ItemManagiment()
{
    for (int i = 0; i < MAX_SPAWN; i++)
    {
        m_items[i].isActive = false;
    }
}

// Loads one image per item type; the returned int handle is stored for use in Item_Draw
// (JP: アイテム種別ごとに1枚画像を読み込む。返されるintハンドルはItem_Draw用に保存する)
void Item_Managiment::Load()
{
    Item_Handle[TOMATO] = LoadGraph("../Pizza_Image/TOMATO.png");
    Item_Handle[BASIL] = LoadGraph("../Pizza_Image/Basil.png");
    Item_Handle[CHEESE] = LoadGraph("../Pizza_Image/cheese.png");
    Item_Handle[GORGONZOLA] = LoadGraph("../Pizza_Image/gorgonzola.png");
    Item_Handle[PIZZADOUGH] = LoadGraph("../Pizza_Image/pizza_Dough.png");
}

// Places one item of the given type at a random passable tile
// Algorithm:
//   1. Find the first inactive slot in the pool
//   2. Randomly pick a tile up to MAX_ATTEMPTS times until one passes all three checks:
//        a) tile value == 1 (passable in the map array)
//        b) CheckCollision returns false (pixel-space double-check)
//        c) no other active item already occupies the same tile
//   3. Write the chosen position into the slot and activate it with a 300-frame lifetime
// If MAX_ATTEMPTS is exhausted without finding a valid tile, the spawn is silently skipped
// (JP: 指定種別のアイテムをランダムな通行可能タイルに1つ配置する)
// (JP: アルゴリズム: 1.プール内の非アクティブスロットを探す 2.3つのチェックを全て通過するタイルを最大MAX_ATTEMPTS回ランダムに選ぶ)
// (JP: a)タイル値==1(通行可) b)CheckCollisionがfalse(ピクセル空間二重チェック) c)同タイルに他のアクティブアイテムなし)
// (JP: 3.選んだ座標をスロットに書き込み、寿命300フレームでアクティブにする)
// (JP: MAX_ATTEMPTSを使い果たしても有効なタイルが見つからない場合はスポーンを静かにスキップする)
void Item_Managiment::Spawn(const BackScreen& stage, Item_number type)
{
    for (int i = 0; i < MAX_SPAWN; i++)
    {
        if (!m_items[i].isActive)
        {
            int gx = 0, gy = 0;
            bool foundLocation = false;

            const int MAX_ATTEMPTS = 1000;
            int attempts = 0;
            const int TILE_SIZE = 32;

            while (!foundLocation && attempts < MAX_ATTEMPTS)
            {
                attempts++;

                // Pick a random tile within the map bounds
                // (JP: マップ範囲内でランダムなタイルを選ぶ)
                gx = GetRand(stage.MAP_Get_SizeX() - 1);
                gy = GetRand(stage.MAP_Get_SizeY() - 1);

                // Check a: tile must be marked passable in the map array
                // (JP: チェックa: タイルがマップ配列で通行可とマークされていること)
                if (stage.GetMapvalue(gx, gy) != 1) continue;

                // Check b: pixel-space collision check (catches edge cases near walls)
                // (JP: チェックb: ピクセル空間衝突チェック(壁近くのエッジケースを捕捉する))
                double worldX = gx * TILE_SIZE + TILE_SIZE / 2.0;
                double worldY = gy * TILE_SIZE + TILE_SIZE / 2.0;
                if (stage.CheckCollision(worldX, worldY)) continue;

                // Check c: no other active item on the same tile
                // (JP: チェックc: 同じタイルに他のアクティブアイテムがないこと)
                bool overlapWithItem = false;
                for (int j = 0; j < MAX_SPAWN; j++)
                {
                    if (m_items[j].isActive && m_items[j].x == gx && m_items[j].y == gy)
                    {
                        overlapWithItem = true;
                        break;
                    }
                }
                if (overlapWithItem) continue;

                foundLocation = true;
            }

            // Give up quietly if no valid tile was found within the attempt limit
            // (JP: 試行上限内に有効なタイルが見つからなかった場合は静かに諦める)
            if (!foundLocation) return;

            m_items[i].x = gx;
            m_items[i].y = gy;
            m_items[i].type = type;
            m_items[i].isActive = true;
            m_items[i].aliveTimer = 1000.0f;// frames until despawn (JP: デスポーンまでのフレーム数)
            break;
        }
    }
}

// Counts down each active item's lifetime timer and deactivates it when it reaches 0
// Called once per frame from the main loop
// (JP: アクティブな各アイテムの寿命タイマーをカウントダウンし、0になったら非アクティブにする)
// (JP: メインループから毎フレーム1回呼ばれる)
void Item_Managiment::Updata()
{
    for (int i = 0; i < MAX_SPAWN; i++)
    {
        if (m_items[i].isActive)
        {
            m_items[i].aliveTimer--;
            if (m_items[i].aliveTimer <= 0)
            {
                m_items[i].isActive = false;
            }
        }
    }
}

// Checks whether the player is within one tile of any active item
// If a match is found the item is deactivated and its type is returned
// The caller (main loop) then passes the type to player.Player_BringItem()
// Returns ITEM_MAX when no item is within pickup range
// (JP: プレイヤーがアクティブなアイテムの1タイル以内にいるか確認する)
// (JP: 一致するものが見つかればアイテムを非アクティブにし、その種別を返す)
// (JP: 呼び出し元(メインループ)が種別をplayer.Player_BringItem()に渡す)
// (JP: 拾える範囲にアイテムがない場合はITEM_MAXを返す)
Item_number Item_Managiment::CheckPickUp(Player_Managiment& player)
{
    for (int i = 0; i < MAX_SPAWN; i++)
    {
        if (!m_items[i].isActive) continue;

        // abs() distance <= 1 tile on both axes defines the pickup range
        // (JP: 両軸でabs()距離<=1タイルがピックアップ範囲を定義する)
        if (abs(m_items[i].x - player.GetX()) <= 1 &&
            abs(m_items[i].y - player.GetY()) <= 1)
        {
            // Save the type before deactivating to avoid reading a cleared slot
            // (JP: スロットをクリアする前に種別を保存する)
            Item_number pickedType = m_items[i].type;
            m_items[i].isActive = false;
            return pickedType;
        }
    }

    return ITEM_MAX;// No item in range (JP: 範囲内にアイテムなし)
}