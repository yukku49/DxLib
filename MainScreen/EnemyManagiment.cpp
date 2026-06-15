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

// 衝突判定用のヒットボックスサイズ（描画サイズより小さめ）
static const int HIT_SIZE = 24;

void Enemy_Managiment::Enemy_Initialisation(float startX, float startY)
{
   
    // startX/startY はHUDオフセット込みのピクセル座標
    a.enemy_X = startX;
    a.enemy_Y = startY;
    a.vx = 0.0f;
    a.vy = 0.0f;
    a.isActive = true;

    a.Enemy_Eye_handlbe[Enemy_Up] = LoadGraph("../Pizza_Image/Enemy_Up.png");
    a.Enemy_Eye_handlbe[Enemy_Left] = LoadGraph("../Pizza_Image/Enemy_Left.png");
    a.Enemy_Eye_handlbe[Enemy_Right] = LoadGraph("../Pizza_Image/Enemy_Right.png");
    a.Enemy_Eye_handlbe[Enemy_Down] = LoadGraph("../Pizza_Image/Enemy_Down.png");
}

int Enemy_Managiment::Get_EnemyHandle() const
{
    if (!a.isActive) return -1;

    // vx/vy の大きい方の軸で向きを判定
    float absVx = std::fabs(a.vx);
    float absVy = std::fabs(a.vy);

    if (absVx >= absVy)
        return (a.vx <= 0.0f) ? a.Enemy_Eye_handlbe[Enemy_Left] : a.Enemy_Eye_handlbe[Enemy_Right];
    else
        return (a.vy <= 0.0f) ? a.Enemy_Eye_handlbe[Enemy_Up] : a.Enemy_Eye_handlbe[Enemy_Down];
}

// 既存互換（当たり判定なし）
void Enemy_Managiment::Enemy_Update()
{
    if (!a.isActive) return;
    a.enemy_X += a.vx;
    a.enemy_Y += a.vy;
    if (a.enemy_X < 0.0f || a.enemy_X > SCREEN_W - 32.0f) a.vx *= -1.0f;
    if (a.enemy_Y < 0.0f || a.enemy_Y > SCREEN_H - 32.0f) a.vy *= -1.0f;
}

// BFS 経路追従＋当たり判定あり更新
void Enemy_Managiment::Enemy_Update(const BackScreen& stage, float playerX, float playerY)
{
    if (!a.isActive) return;
   
    // 内部座標（HUDオフセット込み）をそのままタイル変換
    // CheckCollision が内部で -HUD_OFFSET するので、ここでは生座標を渡す
    int eTx = static_cast<int>(a.enemy_X) / 32;
    int eTy = static_cast<int>(a.enemy_Y - 32) / 32; // タイル座標変換（HUD分を引く）
    int pTx = static_cast<int>(playerX) / 32;
    int pTy = static_cast<int>(playerY - 32) / 32;
    m_pathTimer++;
    if (m_pathTimer >= PATH_INTERVAL || m_path.empty())
    {
        m_pathTimer = 0;
        CalcPath(stage, eTx, eTy, pTx, pTy);

        m_pathIndex = 0; // ← インデックスリセット忘れずに


    }

    if (m_path.empty() || m_pathIndex >= (int)m_path.size()) return;


    auto [nextTx, nextTy] = m_path[m_pathIndex];

    // タイル座標 → HUDオフセット込みピクセル座標
    float nextPx = nextTx * 32.0f;
    float nextPy = nextTy * 32.0f + 32.0f;

    float dx = nextPx - a.enemy_X;
    float dy = nextPy - a.enemy_Y;
    float dist = std::sqrt(dx * dx + dy * dy);

    // 到達したら次のタイルへ
    if (dist < 2.0f)
    {
        a.enemy_X = nextPx;
        a.enemy_Y = nextPy;
        m_pathIndex++;
        return;
    }

    const float speed = 90.0f;
    float moveX = (dx / dist) * speed;
    float moveY = (dy / dist) * speed;

    const int w = m_displaySize;


    // ★ X軸衝突判定
    float newX = a.enemy_X + moveX;
    float hitBaseY = a.enemy_Y + 32.0f;
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


    // ★ 衝突が続くなら再計算を強制
    if (hitX && hitY)
    {
        m_pathTimer = PATH_INTERVAL;
    }

}

void Enemy_Managiment::CalcPath(const BackScreen& stage,
    int startTx, int startTy, int goalTx, int goalTy)
{
    m_path.clear();
    m_pathIndex = 0;

    const int W = stage.MAP_Get_SizeX();
    const int H = stage.MAP_Get_SizeY();

    if (goalTx < 0 || goalTx >= W || goalTy < 0 || goalTy >= H) return;
    if (startTx < 0 || startTx >= W || startTy < 0 || startTy >= H) return;
    if (stage.GetMapvalue(goalTx, goalTy) == 0) return;

    std::vector<std::vector<std::pair<int, int>>> parent(
        H, std::vector<std::pair<int, int>>(W, { -1,-1 }));

    std::queue<std::pair<int, int>> q;
    q.push({ startTx, startTy });
    parent[startTy][startTx] = { startTx, startTy };

    const std::array<std::pair<int, int>, 4> dirs = { {{0,-1},{0,1},{-1,0},{1,0}} };

    bool found = false;
    while (!q.empty() && !found)
    {
        auto [cx, cy] = q.front(); q.pop();
        for (auto [ddx, ddy] : dirs)
        {
            int nx = cx + ddx, ny = cy + ddy;
            if (nx < 0 || nx >= W || ny < 0 || ny >= H) continue;
            if (stage.GetMapvalue(nx, ny) == 0)          continue;
            if (parent[ny][nx].first != -1)              continue;

            parent[ny][nx] = { cx, cy };
            q.push({ nx, ny });

            if (nx == goalTx && ny == goalTy) { found = true; break; }
        }
    }

    if (!found) return;

    std::pair<int, int> cur = { goalTx, goalTy };
    while (cur != std::make_pair(startTx, startTy))
    {
        m_path.push_back(cur);
        cur = parent[cur.second][cur.first];
    }
    std::reverse(m_path.begin(), m_path.end());
}