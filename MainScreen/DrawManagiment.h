#pragma once

// Forward declarations to avoid pulling in heavy headers here;
// the full definitions are only needed in DrawManagiment.cpp
// (JP: ここで重いヘッダをインクルードしないよう前方宣言する。完全な定義はcppファイルだけに必要)class BackScreen;
class Player_Managiment;
class Bllent_Managiment;
class Enemy_Managiment;
class Item_Managiment;
class StartScreen;

// Forward declarations to avoid pulling in heavy headers here;
// the full definitions are only needed in DrawManagiment.cpp
// (JP: ここで重いヘッダをインクルードしないよう前方宣言する。完全な定義はcppファイルだけに必要)
class DrawManager
{
public:
    // Forward declarations to avoid pulling in heavy headers here;
// the full definitions are only needed in DrawManagiment.cpp
// (JP: ここで重いヘッダをインクルードしないよう前方宣言する。完全な定義はcppファイルだけに必要)
    void Player_Draw(const BackScreen& stage, const Player_Managiment& player) const;

    // Draws the stage background image then overlays obstacle sprites on every tile where value == 0
    // (JP: ステージ背景画像を描画し、値が0のタイル全てに障害物スプライトを重ねて描く)
    void Map_Draw(const BackScreen& object) const;

    // Iterates the bullet pool and draws each active bullet at its current pixel position
// (JP: 弾プールを走査し、アクティブな弾を現在のピクセル座標に描画する)
    void Bullets_Draw(const Bllent_Managiment& bullets) const;

    // Draws the enemy sprite scaled to fit, centered on its tile; skips drawing if not active
// (JP: 敵スプライトを拡縮してタイル上に中央揃えで描画する。非アクティブ時はスキップ)
    void Enemy_Draw(const Enemy_Managiment& enemy, const BackScreen& stage) const;

    // Draws all active item sprites at their tile grid positions
// (JP: アクティブなアイテムスプライトをタイルグリッド座標に描画する)
    void Item_Draw(const Item_Managiment& item, const BackScreen& stage)const;

    // Draws the start/title screen (currently a placeholder white rectangle)
// (JP: スタート/タイトル画面を描画する。現在は仮の白い矩形)
    void Start_Draw(const StartScreen& start)const;

    // Draws the HUD bar at the top of the screen:
    //   - Player fullness gauge (left, orange)
    //   - Enemy fullness gauge  (right, red)   <- currently hardcoded to 30%
    //   - Item inventory counts (center)
    // (JP: 画面上部のHUDバーを描画する)
    // (JP: プレイヤー満腹ゲージ(左・オレンジ)、敵満腹ゲージ(右・赤)、アイテム所持数(中央))
    void HUD_Draw(const Player_Managiment& player, const Enemy_Managiment& enemy)const;

    // Draws a semi-transparent overlay with WIN or LOSE message and final score
// Called from the main loop once a win condition is detected
// (JP: 半透明オーバーレイにWIN/LOSEメッセージと最終スコアを描画する)
// (JP: 勝利条件が検出されたらメインループから呼ぶ)
    void Result_Draw(bool playerWin) const;
};