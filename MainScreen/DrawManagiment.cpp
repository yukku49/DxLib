#include "DrawManagiment.h"
#include "BackScreenManagiment.h"
#include "EnemyManagiment.h"
#include "BllentManagiment.h"
#include "PlayerManagiment.h"
#include "ItemManagiment.h"
#include "StartScreen.h"
#include "DxLib.h"

static constexpr int TILE_SIZE = 32;

void DrawManager::Player_Draw(const BackScreen& stage, const Player_Managiment& player) const
{
    int w = 1, h = 1;
    GetGraphSize(player.Get_PlayerHandle(), &w, &h);

    const int CHARA_WIDTH = player.Get_PlayerDisplaySize();
    int drawH = static_cast<int>(CHARA_WIDTH * (static_cast<float>(h) / w));

<<<<<<< HEAD
    int x1 = static_cast<int>(player.GetXf()) + (TILE_SIZE - CHARA_WIDTH) / 2;
    int y1 = static_cast<int>(player.GetYf()) + (TILE_SIZE - drawH);
    int x2 = x1 + CHARA_WIDTH;
    int y2 = y1 + drawH;
=======
	// 描画座標：ピクセル座標を直接使用
	int x1 = static_cast<int>(player.GetXf()) + (TILE_SIZE - CHARA_WIDTH) / 2;
	int y1 = static_cast<int>(player.GetYf()) + (TILE_SIZE - drawH);
>>>>>>> ffbacaaea3bb321e0fa3d1a232042b05f6921aae

    DrawExtendGraph(x1, y1, x2, y2, player.Get_PlayerHandle(), TRUE);
}

void DrawManager::Map_Draw(const BackScreen& object) const
{
    DrawExtendGraph(0, TILE_SIZE, 1280, TILE_SIZE + 736, object.Get_Maphandle(), false);

<<<<<<< HEAD
    for (int y = 0; y < object.MAP_Get_SizeY(); y++)
    {
        for (int x = 0; x < object.MAP_Get_SizeX(); x++)
        {
            if (object.GetMapvalue(x, y) == 0)
            {
                // 【修正】背景画像と同じ y オフセット(TILE_SIZE=32)を加算してHUDバー分ずらす
                DrawExtendGraph(
                    x * TILE_SIZE, y * TILE_SIZE + TILE_SIZE,
                    x * TILE_SIZE + TILE_SIZE, y * TILE_SIZE + TILE_SIZE * 2,
=======
	for (int y = 0; y < object.MAP_Get_SizeY(); y++)
	{
		for (int x = 0; x < object.MAP_Get_SizeX(); x++)
		{
			if (object.GetMapvalue(x, y) == 0)
			{
				DrawExtendGraph(x * TILE_SIZE, y * TILE_SIZE,
                    x * TILE_SIZE + TILE_SIZE, y * TILE_SIZE + TILE_SIZE,
>>>>>>> ffbacaaea3bb321e0fa3d1a232042b05f6921aae
                    object.Get_ObjectHanadle(), true);
            }
        }
    }
}

void DrawManager::Bullets_Draw(const Bllent_Managiment& bullets) const
{
    for (int i = 0; i < bullets.GetMaxBullets(); ++i)
    {
        const auto& b = bullets.Get_Bullethandle(i);
        if (b.isActive && b.using_handle >= 0)
        {
            const int drawX = static_cast<int>(b.x) - 8;
            const int drawY = static_cast<int>(b.y) + TILE_SIZE - 8;
            DrawExtendGraph(drawX, drawY, drawX + 32, drawY + 32, b.using_handle, true);
        }
    }
}

void DrawManager::Enemy_Draw(const Enemy_Managiment& enemy, const BackScreen& stage) const
{
    const auto& e = enemy.Get_enemyPoint();
    if (!e.isActive) return;

    int handle = enemy.Get_EnemyHandle();
    if (handle < 0) return;

    int w = 1, h = 1;
    GetGraphSize(handle, &w, &h);

    const int CHARA_WIDTH = enemy.Get_EnemyDisplaySize();
    int drawH = static_cast<int>(CHARA_WIDTH * (static_cast<float>(h) / w));

    int x1 = static_cast<int>(enemy.Get_enemyX()) + (TILE_SIZE - CHARA_WIDTH) / 2;
    int y1 = static_cast<int>(enemy.Get_enemyY()) + (TILE_SIZE - drawH);
    int x2 = x1 + CHARA_WIDTH;
    int y2 = y1 + drawH;

    DrawExtendGraph(x1, y1, x2, y2, handle, TRUE);
}

void DrawManager::Start_Draw(const StartScreen& start) const
{
    DrawBox(0, 0, 736 - 1, 1280 - 1, GetColor(255, 255, 255), false);
}

void DrawManager::HUD_Draw(const Player_Managiment& player, const Enemy_Managiment& enemy) const
{
    // UI バー背景
    DrawBox(0, 0, 1280, TILE_SIZE, GetColor(30, 30, 30), TRUE);
    DrawBox(0, TILE_SIZE - 1, 1280, TILE_SIZE, GetColor(80, 80, 80), TRUE);

    // プレイヤー満腹ゲージ（左）
    const int BAR_W = 200, BAR_H = 14;
    const int P_BAR_X = 10, BAR_Y = (TILE_SIZE - BAR_H) / 2;

    DrawBox(P_BAR_X, BAR_Y, P_BAR_X + BAR_W, BAR_Y + BAR_H, GetColor(80, 80, 80), TRUE);

    float ratio = player.GetFullness() / Player_Managiment::MAX_FULLNES;
    int fillW = static_cast<int>(BAR_W * ratio);
    DrawBox(P_BAR_X, BAR_Y, P_BAR_X + fillW, BAR_Y + BAR_H, GetColor(255, 180, 0), TRUE);

    DrawFormatString(P_BAR_X + BAR_W + 6, BAR_Y, GetColor(255, 255, 255), "P1");

    // 敵の満腹ゲージ（右）
    const int E_BAR_X = 1280 - BAR_W - 10;
    DrawBox(E_BAR_X, BAR_Y, E_BAR_X + BAR_W, BAR_Y + BAR_H, GetColor(80, 80, 80), TRUE);

    float enemyRatio = 0.3f; // TODO: 敵の実際の満腹値を取得
    int eFillW = static_cast<int>(BAR_W * enemyRatio);
    DrawBox(E_BAR_X, BAR_Y, E_BAR_X + eFillW, BAR_Y + BAR_H, GetColor(220, 60, 60), TRUE);

    DrawFormatString(E_BAR_X - 30, BAR_Y, GetColor(255, 255, 255), "CPU");

    // 【修正】所持アイテム数（中央）— バジルカウンタを追加
    auto items = player.Get_Player_Itembring();
    DrawFormatString(380, BAR_Y, GetColor(200, 200, 200),
        "生地:%d トマト:%d チーズ:%d ゴルゴン:%d バジル:%d",
        items.Pizzadough_Counter,
        items.Tmato_Counter,
        items.Cheese_Counter,
        items.Gorgonzola_Counter,
        items.Basil_Counter);
}

void DrawManager::Item_Draw(const Item_Managiment& item, const BackScreen& stage) const
{
    for (int i = 0; i < item.Get_Item_number(); i++)
    {
        const ItemData* it = item.Get_Items(i);
        if (!it->isActive) continue;

        int x1 = it->x * TILE_SIZE;
        int y1 = it->y * TILE_SIZE + TILE_SIZE;
        int x2 = x1 + TILE_SIZE;
        int y2 = y1 + TILE_SIZE;

        DrawExtendGraph(x1, y1, x2, y2, item.Get_ItemHandle(it->type), TRUE);
    }
}