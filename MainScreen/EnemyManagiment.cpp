#include "DxLib.h"
#include "EnemyManagiment.h"
#include "BackScreenManagiment.h"
#include <cmath>
#include <queue>
#include <array>
#include <algorithm>

static const int SCREEN_W = 1280;
static const int SCREEN_H = 736;
static const float ENEMY_SPEED = 1.5f;

// Hit-box size for collision checks; smaller than m_displaySize so the enemy
// can pass through gaps that look visually clear
// (JP: 衝突チェック用ヒットボックスサイズ。見た目より小さくして隙間を通れるようにする)
static const int HIT_SIZE = 24;

// Sets starting position, zero velocity, and loads all four directional sprites
// startX/startY must already include the 32px HUD offset so coordinates are
// consistent with how CheckCollision() works (it subtracts 32 internally)
// (JP: 開始位置・速度0を設定し、4方向スプライトを読み込む)
// (JP: startX/startYにはCheckCollision()と合わせるため32pxのHUDオフセットを含める)
void Enemy_Managiment::Enemy_Initialisation(float startX, float startY)
{
    m_lastTime = GetNowCount();
    // startX/startY はHUDオフセット込みのピクセル座標
    a.enemy_X = startX;
    a.enemy_Y = startY;
    a.vx = 0.0f;
    a.vy = 0.0f;
    a.isActive = true;

    a.Enemy_Eye_handlbe[Enemy_Up] = LoadGraph("Pizza_Image/Enemy_Up.png");
    a.Enemy_Eye_handlbe[Enemy_Left] = LoadGraph("Pizza_Image/Enemy_Left.png");
    a.Enemy_Eye_handlbe[Enemy_Right] = LoadGraph("Pizza_Image/Enemy_Right.png");
    a.Enemy_Eye_handlbe[Enemy_Down] = LoadGraph("Pizza_Image/Enemy_Down.png");
}

// Selects the sprite handle that matches the enemy's current movement direction
// Compares absolute values of vx and vy: the dominant axis decides horizontal vs vertical
// Within horizontal: negative vx = left, positive = right
// Within vertical:   negative vy = up,   positive = down
// Returns -1 if the enemy is not active (DrawManager skips drawing in that case)
// (JP: 現在の移動方向に合ったスプライトハンドルを選ぶ)
// (JP: |vx|と|vy|を比べて水平か垂直かを決定し、符号で左右・上下を決める)
// (JP: 非アクティブ時は-1を返す。DrawManagerはこれを見て描画をスキップする)
int Enemy_Managiment::Get_EnemyHandle() const
{
    if (!a.isActive) return -1;

    // vx/vy の大きい方の軸で向きを判定
    float absVx = std::fabs(a.vx);
    float absVy = std::fabs(a.vy);

    if (absVx >= absVy)
        return (a.vx < 0.0f) ? a.Enemy_Eye_handlbe[Enemy_Left] : a.Enemy_Eye_handlbe[Enemy_Right];
    else
        return (a.vy < 0.0f) ? a.Enemy_Eye_handlbe[Enemy_Up] : a.Enemy_Eye_handlbe[Enemy_Down];
}
void Enemy_Managiment::OnHit()
{
    if (m_invincibleTimer > 0.0f)return;//無敵中は無視
    m_invincibleTimer = INVINCIBLE_DURATION;
}
// 同じ行または列で間に壁がないか確認する
// (JP: 同じタイル行または列で、間のタイルに壁がなければtrueを返す)
static bool HasLineOfSight(const BackScreen& stage,
    int eTx, int eTy, int pTx, int pTy)
{
    if (eTy == pTy) // 同じ行（横方向）
    {
        int minX = (eTx < pTx) ? eTx : pTx;
        int maxX = (eTx > pTx) ? eTx : pTx;
        for (int x = minX + 1; x < maxX; x++)
        {
            if (stage.GetMapvalue(x, eTy) == 0) return false;
        }
        return true;
    }
    if (eTx == pTx) // 同じ列（縦方向）
    {
        int minY = (eTy < pTy) ? eTy : pTy;
        int maxY = (eTy > pTy) ? eTy : pTy;
        for (int y = minY + 1; y < maxY; y++)
        {
            if (stage.GetMapvalue(eTx, y) == 0) return false;
        }
        return true;
    }
    return false; // 直線上にいない
}

 bool Enemy_Managiment::TryShoot(const BackScreen& stage, float playerX, float playerY, float dt, float& outVx, float& outVy)
 {
     outVx = outVy = 0.0f;
     if (!a.isActive) return false;

     // タイマーカウントダウン（JP: 発射間隔のカウントダウン）
     m_shootTimer -= dt;
     if (m_shootTimer > 0.0f) return false;

     // タイル座標に変換（JP: ピクセル→タイル変換）
     int eTx = static_cast<int>(a.enemy_X) / 32;
     int eTy = static_cast<int>(a.enemy_Y - 32) / 32;
     int pTx = static_cast<int>(playerX) / 32;
     int pTy = static_cast<int>(playerY - 32) / 32;

     // 視線チェック（JP: 直線上かつ壁なしか確認）
     if (!HasLineOfSight(stage, eTx, eTy, pTx, pTy)) return false;

     // 方向決定（JP: プレイヤーへの方向を決める）
     if (eTy == pTy)
         outVx = (pTx > eTx) ? 300.0f : -300.0f; // 横方向
     else
         outVy = (pTy > eTy) ? 300.0f : -300.0f; // 縦方向

     m_shootTimer = SHOOT_INTERVAL; // タイマーリセット
     return true;
 }

// Legacy update: moves by vx/vy with simple screen-boundary bouncing and no tile collision
// Kept so older call sites still compile; prefer Enemy_Update(stage, px, py) instead
// (JP: 旧更新。vx/vyで移動し画面端で跳ね返るだけでタイル衝突なし)
// (JP: 旧呼び出し元がコンパイルできるよう残している。新規コードはEnemy_Update(stage,px,py)を使う)
void Enemy_Managiment::Enemy_Update()
{
    
    if (!a.isActive) return;
    a.enemy_X += a.vx;
    a.enemy_Y += a.vy;
    if (a.enemy_X < 0.0f || a.enemy_X > SCREEN_W - 32.0f) a.vx *= -1.0f;
    if (a.enemy_Y < 0.0f || a.enemy_Y > SCREEN_H - 32.0f) a.vy *= -1.0f;
}

// Full BFS-based update called once per frame from the main loop
// Steps:
//   1. Calculate delta time (dt) for frame-rate-independent speed
//   2. Convert pixel positions to tile coordinates for BFS
//   3. Recalculate the BFS path every PATH_INTERVAL frames
//   4. Move toward the next tile in the path at 'speed' px/s
//   5. Check tile collision separately on X and Y axes so the enemy
//      can slide along walls instead of stopping completely
//   6. Force path recalculation when both axes are blocked
// (JP: メインループから毎フレーム呼ばれるBFS経路追従付き完全更新)
void Enemy_Managiment::Enemy_Update(const BackScreen& stage, float playerX, float playerY)
{
    

    if (!a.isActive) return;

    // --- 1. Delta time ---
    // Clamp to 100ms to prevent large position jumps after frame hitches
    // (JP: フレーム落ち後の大きな位置ジャンプを防ぐため100msにクランプ)
    unsigned int now = GetNowCount();
    float dt = (now - m_lastTime) / 1000.0f;
    if (dt > 0.1f) dt = 0.1f;
    m_lastTime = now;

    if (m_invincibleTimer > 0.0f)
    {
        m_invincibleTimer -= dt;
        m_blinkTimer++;
    }
    // --- 2. Convert pixel positions to tile coordinates ---
    // Subtract 32 from Y before dividing because the HUD bar occupies the top 32px;
    // CheckCollision does the same subtraction, so tile indices stay consistent
    // (JP: Yから32を引いてから割る。HUDバーが上部32pxを占めるため。CheckCollisionも同じ処理をする)
    int eTx = static_cast<int>(a.enemy_X) / 32;
    int eTy = static_cast<int>(a.enemy_Y - 32) / 32; // タイル座標変換（HUD分を引く）
    int pTx = static_cast<int>(playerX) / 32;
    int pTy = static_cast<int>(playerY - 32) / 32;

    // --- 3. Recalculate BFS path periodically ---
   // PATH_INTERVAL frames between recalculations to limit CPU usage
   // Also recalculate immediately when m_path is empty (first frame or after a blocked path)
   // (JP: CPU負荷を抑えるためPATH_INTERVALフレームごとに再計算する)
   // (JP: m_pathが空の時(初回またはパスがブロックされた後)も即座に再計算する)
    m_pathTimer++;
    if (m_pathTimer >= PATH_INTERVAL || m_path.empty())
    {
        m_pathTimer = 0;
        CalcPath(stage, eTx, eTy, pTx, pTy);

        m_pathIndex = 0; // Always reset index when the path changes (JP: パス変更時は必ずインデックスをリセット)


    }

    if (m_path.empty() || m_pathIndex >= (int)m_path.size()) return;

    // --- 4. Move toward next tile in path ---
    auto [nextTx, nextTy] = m_path[m_pathIndex];

    // Convert next tile back to pixel space (add 32 to Y to restore HUD offset)
   // (JP: 次のタイルをピクセル空間に戻す。HUDオフセットを戻すためYに32を足す)
    float nextPx = nextTx * 32.0f;
    float nextPy = nextTy * 32.0f + 32.0f;

    float dx = nextPx - a.enemy_X;
    float dy = nextPy - a.enemy_Y;
    float dist = std::sqrt(dx * dx + dy * dy);

    // Snap to the tile center when close enough to avoid jitter
    // (JP: 十分近づいたらタイル中心にスナップしてジッターを防ぐ)
    if (dist < 2.0f)
    {
        a.enemy_X = nextPx;
        a.enemy_Y = nextPy;
        m_pathIndex++;
        return;
    }

    const float speed = 90.0f;// px/s
    float moveX = (dx / dist) * speed*dt;
    float moveY = (dy / dist) * speed*dt;

    // --- 5. Separate-axis collision so enemy slides along walls ---
    // Check X movement first, then Y movement independently.
    // If only X is blocked the enemy can still move in Y (slides along the wall).
    // (JP: 軸分離衝突チェック。壁にぶつかっても反対軸は移動できるようにする（壁スライド）)
    const int w = m_displaySize;


    // ★ X軸衝突判定
    float newX = a.enemy_X + moveX;
    float hitBaseY = a.enemy_Y +moveY;
    bool hitX =
        stage.CheckCollision(newX, hitBaseY) ||
        stage.CheckCollision(newX + w - 1, hitBaseY) ||
        stage.CheckCollision(newX, hitBaseY + w - 1) ||
        stage.CheckCollision(newX + w - 1, hitBaseY + w - 1);

    float newY = a.enemy_Y + moveY;
    bool hitY =
        stage.CheckCollision(a.enemy_X, newY ) ||
        stage.CheckCollision(a.enemy_X + w - 1, newY ) ||
        stage.CheckCollision(a.enemy_X, newY  + w - 1) ||
        stage.CheckCollision(a.enemy_X + w - 1, newY  + w - 1);


    if (!hitX) a.enemy_X = newX;
    if (!hitY) a.enemy_Y = newY;

    //向き判定用の実際に移動方向を記録する
    a.vx = hitX ? 0.0f : moveX;
    a.vy = hitY ? 0.0f : moveY;

    // --- 6. Force path recalculation when fully stuck ---
    // If both axes are blocked the current path is no longer valid;
    // setting m_pathTimer to PATH_INTERVAL triggers an immediate recalc next frame
    // (JP: 両軸ともブロックされた場合は現在のパスが無効なので次フレームで即再計算する)
    if (hitX && hitY)
    {
        m_pathTimer = PATH_INTERVAL;
    }

}

// Breadth-First Search to find the shortest path from start tile to goal tile
// Only tiles with value 1 (passable) are traversed
// The result is stored in m_path as an ordered list from start+1 to goal
// If goal is unreachable (blocked or out of bounds), m_path is left empty
// (JP: 開始タイルからゴールタイルへの最短BFS経路を求める)
// (JP: 値1(通行可)のタイルだけを通る。結果はstart+1〜goalの順でm_pathに格納)
// (JP: ゴールに到達不可能な場合(ブロックまたは範囲外)はm_pathを空のままにする)
void Enemy_Managiment::CalcPath(const BackScreen& stage,
    int startTx, int startTy, int goalTx, int goalTy)
{
    m_path.clear();
    m_pathIndex = 0;

    const int W = stage.MAP_Get_SizeX();
    const int H = stage.MAP_Get_SizeY();

    // Boundary and passability guards
   // (JP: 境界チェックと通行可能チェック)
    if (goalTx < 0 || goalTx >= W || goalTy < 0 || goalTy >= H) return;
    if (startTx < 0 || startTx >= W || startTy < 0 || startTy >= H) return;
    if (stage.GetMapvalue(goalTx, goalTy) == 0) return;// goal is a wall (JP: ゴールが壁)

    // parent[y][x] stores the tile that discovered (x, y) during BFS
   // Used to reconstruct the path by backtracking from goal to start
   // Initialized to {-1,-1} meaning "not yet visited"
   // (JP: parent[y][x]はBFS中に(x,y)を発見したタイルを格納する)
   // (JP: ゴールからスタートへのバックトラックでパスを再構築するために使う)
   // (JP: {-1,-1}は「未訪問」を意味する)
    std::vector<std::vector<std::pair<int, int>>> parent(
        H, std::vector<std::pair<int, int>>(W, { -1,-1 }));

    std::queue<std::pair<int, int>> q;
    q.push({ startTx, startTy });// mark start as visited (JP: スタートを訪問済みにする)
    parent[startTy][startTx] = { startTx, startTy };

  
    /// Four cardinal directions: up, down, left, right
    // (JP: 上下左右の4方向)
    const std::array<std::pair<int, int>, 4> dirs = { {{0,-1},{0,1},{-1,0},{1,0}} };

    bool found = false;
    while (!q.empty() && !found)
    {
        auto [cx, cy] = q.front(); q.pop();
        for (auto [ddx, ddy] : dirs)
        {
            int nx = cx + ddx, ny = cy + ddy;
            if (nx < 0 || nx >= W || ny < 0 || ny >= H) continue;// out of bounds
            if (stage.GetMapvalue(nx, ny) == 0)          continue;// wall tile
            if (parent[ny][nx].first != -1)              continue;// already visited

            parent[ny][nx] = { cx, cy };
            q.push({ nx, ny });// mark start as visited (JP: スタートを訪問済みにする)

            if (nx == goalTx && ny == goalTy) { found = true; break; }
        }
    }

    if (!found) return;// no path exists (JP: 経路なし)

    // Backtrack from goal to start using the parent map, then reverse
    // so m_path goes from first step after start → goal
    // (JP: parentマップを使ってゴールからスタートへバックトラックし、逆順にする)
    // (JP: m_pathはスタートの次のステップ→ゴールの順になる)
    std::pair<int, int> cur = { goalTx, goalTy };
    while (cur != std::make_pair(startTx, startTy))
    {
        m_path.push_back(cur);
        cur = parent[cur.second][cur.first];
    }
    std::reverse(m_path.begin(), m_path.end());
}