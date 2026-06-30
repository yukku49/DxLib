#include "DxLib.h"
#include "BackScreenManagiment.h"
// Tile layout for stage 1
// 1 = passable (corridor), 0 = obstacle (wall/box)
// Array is [row(Y)][col(X)], row 0 is the top of the map
// (JP: ステージ1のタイルレイアウト。1=通行可、0=障害物。[行(Y)][列(X)]で上が行0)
int statgeaPatern_1[23][40] =
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
  0,1,0,1,0,0,0,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,1,0,0,0,0,0,0,0,1,0,
  0,1,0,1,0,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,1,0,0,0,0,0,0,0,1,0,  
  0,1,0,1,0,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,1,0,0,0,0,0,0,0,1,0,
  0,1,0,1,0,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,1,1,1,1,1,1,1,1,1,0,
  0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,0,0,0,0,1,0,1,0,
  0,1,0,1,0,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,1,0,0,0,0,0,1,0,1,0,
  0,1,0,1,0,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,1,0,1,1,1,1,1,0,1,0,
  0,1,0,1,0,0,0,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,0,
  0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,1,0,0,0,0,0,0,0,1,0,
  0,1,0,1,0,0,0,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,1,0,0,0,0,0,0,0,1,0,
  0,1,0,1,0,0,0,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,1,0,1,1,1,1,1,0,1,0,
  0,1,0,1,0,0,0,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,1,0,1,1,1,1,1,0,1,0,
  0,1,0,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,1,0,1,1,1,1,1,0,1,0,
  0,1,0,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,1,1,1,1,1,1,1,1,1,0,
  0,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,1,0,1,1,1,1,1,0,1,0,
  0,1,0,1,0,0,0,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,1,0,1,1,1,1,1,0,1,0,
  0,1,0,1,0,0,0,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,1,0,1,1,1,1,1,0,1,0,
  0,1,0,1,0,0,0,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,
  0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
  0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};


    // Copies a predefined tile pattern into m_stageMap
    // startX/startY: top-left tile coordinate to start writing
    // partType: which pattern to use (only 1 is currently defined)
    // (JP: 定義済みタイルパターンをm_stageMapにコピーする)
    void BackScreen::CopyPart(int startX, int startY, int partType)
    {
        if (partType == 1) {
            for (int y = 0; y < 23; y++) {
                for (int x = 0; x < 40; x++) {
                    // Guard against writing outside the map array bounds
                    // (JP: マップ配列の範囲外への書き込みを防ぐ)
                    if (startY + y < MAP_HEIGHT && startX + x < MAP_WIDTH) {
                        m_stageMap[startY + y][startX + x] = statgeaPatern_1[y][x];
                    }
                }
            }
        }
    }

    // Loads all stage images and builds the collision tile map
    // (JP: ステージ画像を読み込み、衝突タイルマップを構築する)
    void BackScreen::Initialize()
    {
        // Load background images; the returned int is a handle used for drawing later
        // (JP: 背景画像を読み込む。戻り値のintは後で描画に使うハンドル)
        m_handles[0] = LoadGraph("Pizza_Image/stage1.png");

        // Load obstacle images (pizza box, brick wall)
        // (JP: 障害物画像を読み込む。ピザボックスとレンガ)
        o_object[0] = LoadGraph("Pizza_Image/pizza box.png");
        o_object[1] = LoadGraph("Pizza_Image/renga.png");

        // Fill the entire map with stage 1 pattern starting from tile (0,0)
        // (JP: タイル(0,0)からステージ1パターンでマップ全体を埋める)
        CopyPart(0, 0, 1);

    }
    

    



    // Returns the tile value at tile coordinate (x, y)
    // Out-of-bounds coordinates return 0 (treated as obstacle) for safety
    // (JP: タイル座標(x,y)の値を返す。範囲外は安全のため0=障害物を返す)
    int BackScreen::GetMapvalue(int x, int y)const
    {

        if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT)
            return 0;
        return m_stageMap[y][x];
    }

    // Returns true if the world-pixel position (worldX, worldY) falls inside an obstacle tile
    // worldY has 32px subtracted to account for the HUD bar at the top of the screen
    // Converts pixel position to tile index by dividing by 32 (tile size)
    // (JP: ワールドピクセル座標が障害物タイル内なら true を返す)
    // (JP: 画面上部32pxのHUDバー分をworldYから引いてからタイル変換する)
    bool BackScreen::CheckCollision(double worldX, double worldY) const
    {
        // Convert from pixel space to tile space
        // (JP: ピクセル座標からタイル座標に変換)
        int Worldx = static_cast<int>(worldX) / 32;
        int Worldy = static_cast<int>(worldY-32) / 32;// subtract HUD offset (JP: HUDオフセットを引く)
        int chipType = this->GetMapvalue(Worldx, Worldy);

        // 0 = obstacle (collision), 1 = passable (no collision)
        // (JP: 0=障害物(衝突あり)、1=通路(衝突なし))
        return (chipType == 0);
    }

   
   
