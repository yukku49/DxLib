#include <iostream>
#include"DxLib.h"
#include"BackScreenManagiment.h"
#include"BllentManagiment.h"
#include"DrawManagiment.h"
#include"PlayerManagiment.h"
#include"EnemyManagiment.h"
#include"ItemManagiment.h"
#include"StartScreen.h"
#include"DatabaseManagiment.h"

// Entry point for Windows GUI applications; DxLib requires WinMain instead of main()
// (JP: WindowsGUIアプリのエントリポイント。DxLibはmain()ではなくWinMain()を必要とする)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow) 
{
    // Run in a window instead of exclusive full-screen
// (JP: 排他フルスクリーンではなくウィンドウモードで実行する)
    ChangeWindowMode(true);

    // Set the back-buffer resolution; 32 is the color bit-depth
      // (JP: バックバッファの解像度を設定する。32はカラービット深度)
    SetGraphMode(1280, 768, 32);

    // Initialize DxLib; a negative return value means initialization failed
   // (JP: DxLibを初期化する。負の戻り値は初期化失敗を意味する)

    if (DxLib_Init() < 0)
    {
        return -1;
    }
    
    // One tile is 32x32 pixels; used when converting between tile and pixel coordinates
     // (JP: 1タイルは32x32ピクセル。タイル座標とピクセル座標の変換に使う)
    static constexpr int TILE_SIZE = 32;

    // -----------------------------------------------------------------------
// Create all manager objects
// Each manager owns a specific domain: player, items, enemy, bullets, etc.
// (JP: 各マネージャオブジェクトを生成する)
// (JP: 各マネージャはプレイヤー・アイテム・敵・弾など特定のドメインを担当する)
// -----------------------------------------------------------------------
    Player_Managiment player;
    Item_Managiment item;
    Enemy_Managiment enemy;
    Bllent_Managiment bllet;
    DrawManager draw;
    BackScreen stage;
    StartScreen start;
    PizzaDatabase pizzaDb;

    // -----------------------------------------------------------------------
    // Initialize the pizza recipe database from an SQLite file
    // DebugValidateRecipes() prints a warning for any recipe with missing data
    // (JP: SQLiteファイルからピザレシピDBを初期化する)
    // (JP: DebugValidateRecipes()はデータが欠けているレシピがあれば警告を出力する)
    // -----------------------------------------------------------------------
    if (!pizzaDb.Initialize("pizza_data.db"))
    {
        OutputDebugStringA("PizzaDatabase initialization failed\n");
    }
    else
    {
        pizzaDb.DebugValidateRecipes();
    }

    // Give both player and bullet manager a pointer to the shared database
// so they can look up recipes and damage values at runtime
// (JP: プレイヤーと弾マネージャに共有DBへのポインタを渡す)
// (JP: これにより実行時にレシピやダメージ値を参照できるようになる)
    player.SetDatabase(&pizzaDb);
    bllet.SetDatabase(&pizzaDb);

    // -----------------------------------------------------------------------
    // Initialize each manager in dependency order
    // (JP: 依存関係の順に各マネージャを初期化する)
    // -----------------------------------------------------------------------
    player.Initialisation();
    item.ItemManagiment();

    // Enemy start position: tile (38, 21) converted to pixels, Y shifted down by one tile
// to place the enemy below the HUD bar (same convention as player)
// (JP: 敵の開始位置。タイル(38,21)をピクセルに変換し、HUD下に置くためYを1タイル分下げる)
    enemy.Enemy_Initialisation(38 * TILE_SIZE, 21 * TILE_SIZE+TILE_SIZE);
    bllet.Load();
    stage.Initialize();
    start.startInitialize();
    item.Load();

    // Debug variable: tracks what the player is carrying (used with debug keys 1-4)
// (JP: デバッグ変数。プレイヤーの所持アイテムを追跡する。デバッグキー1〜4で使用)
    Item_count playerItems;

    // Pre-spawn one of each item type so the map is not empty at game start
    // (JP: ゲーム開始時にマップが空にならないよう、各アイテム種別を1つずつ事前スポーンする)
    for (int i = 0; i < (int)ITEM_MAX; i++)
    {
        item.Spawn(stage, static_cast<Item_number>(i));
    }

    // -----------------------------------------------------------------------
   // Main game loop: runs until the OS asks us to quit (ProcessMessage != 0)
   // (JP: メインゲームループ。OSが終了を要求するまで(ProcessMessage != 0)実行し続ける)
   // -----------------------------------------------------------------------
    while (1)
    {

        // Handle Windows messages (close button, alt-F4, etc.)
        // Returns non-zero when the application should exit
        // (JP: Windowsメッセージを処理する。アプリ終了時は非ゼロを返す)
        if (ProcessMessage() != 0) break;

        // Clear the back buffer before drawing the new frame
        // (JP: 新しいフレームを描画する前にバックバッファをクリアする)
        ClearDrawScreen();
        
        // -----------------------------------------------------------------------
        // START SCREEN: while the start flag is true, only draw the menu
        // and skip all gameplay logic below
        // (JP: スタート画面。スタートフラグがtrueの間はメニューのみ描画し、
        //      以降のゲームプレイロジックをすべてスキップする)
        // -----------------------------------------------------------------------
        if (start.Get_Start_Flog() == true)
        {

            // Move selection cursor with input (JP: 入力でカーソルを動かす)
            start.MoveCursor();

            // Confirm selection (JP: 選択を確定する)
            start.SelectGames();

            // Handle option screens (JP: オプション画面を処理する)
            start.OptionIn();

            draw.Start_Draw(start); // ★ これが実行されているか確認
            ScreenFlip();

            // Skip gameplay update this frame (JP: このフレームのゲームプレイ更新をスキップ)
              continue;

        }
        
        // ここで他の描画（Map_Draw, Player_Draw など）を呼んでいれば、
        // Start_Draw が上書きされます。必要なら順番を変える。
        
        

        // 更新
        player.Update(stage, bllet);
        bllet.Update(stage, player, enemy);
        // 当たり判定を行う更新を呼ぶ（BackScreen を渡す）
        enemy.Enemy_Update(stage,player.GetXf(),player.GetYf());

        item.Updata();
        Item_number picked = item.CheckPickUp(player);
        if (picked != ITEM_MAX)
        {
            player.Player_BringItem(picked);
        }
        static int spawnTimer = 0;
        spawnTimer++;
        if (spawnTimer >= 360)
        {
            spawnTimer = 0;
            //現在のアクティブ数を数える
            int activeCount = 0;
            for (int i = 0; i < item.Get_Item_number(); i++)
            {
                if (item.Get_Items(i)->isActive)
                    activeCount++;
            }
            int spawncount = 5 - activeCount;
            for (int s = 0; s < spawncount; s++)
            {
                item.Spawn(stage, static_cast<Item_number>(GetRand(ITEM_MAX - 1)));
            }
        }
        // 描画
        draw.Map_Draw(stage);
        draw.Player_Draw(stage, player);
        draw.Enemy_Draw(enemy, stage);
        draw.Bullets_Draw(bllet);
        draw.Item_Draw(item, stage);
    
        // BFSが通っているタイルを緑で表示
        for (int i = 0; i < (int)enemy.GetPathSize(); i++)
        {
            auto [tx, ty] = enemy.GetPath(i);
            DrawBox(tx * 32, ty * 32 + 32,
                tx * 32 + 32, ty * 32 + 64,
                GetColor(0, 255, 0), false);
        }
        

        //pizzaデバック用
        
        if (CheckHitKey(KEY_INPUT_1))
        {
            player.Debug_SetItems(1, 2, 3, 0,3);
            
        }
        else if(CheckHitKey(KEY_INPUT_2))
        {
            player.Debug_SetItems(1, 0, 3, 3, 0);
            
		}
        else if (CheckHitKey(KEY_INPUT_3))
        {
            player.Debug_SetItems(1, 2, 3, 0, 2);
        }
        else if(CheckHitKey(KEY_INPUT_4))
        {
            player.Debug_SetItems(1, 3, 0, 0, 0);
		}
        
       
        // screen flip
        ScreenFlip();
    }

    pizzaDb.Close();

    DxLib_End();

    // End application (JP: ??????????)
    return 0;
}
