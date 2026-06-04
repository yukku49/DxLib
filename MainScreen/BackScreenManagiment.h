#pragma once
    // Stage type definitions (JP: ステージ種別の定義)
    enum class StageType {
        stage1 = 0,
        stage2,
        MAX
    };
    enum class ObjectType {
        Object_1 = 0,
        Object_2,
        MAX
    };
    class BackScreen
    {
    private:
        static const int MAP_WIDTH = 40;  // 1280 / 32 tile count X (JP: X方向タイル数)
        static const int MAP_HEIGHT = 23; // 720 / 32 tile count Y (JP: Y方向タイル数)
        int const displaySize = 32;
        int m_stageMap[MAP_HEIGHT][MAP_WIDTH]; // Stage map array (JP: ステージマップ配列)
        int m_handles[(int)StageType::MAX];    // Background handles (JP: 背景グラフィックハンドル)
        int o_object[(int)ObjectType::MAX];
        StageType m_currentType;
        void CopyPart(int startX, int startY, int partType);

    public:
        void Initialize(); // Load images and initialize map (JP: 画像読み込みとマップ初期化)
        // Return tile value 0/1 (JP: マスの値0/1を返す)
        int GetMapvalue(int x, int y) const;
        // Check collision by world position (JP: ワールド座標で衝突判定)
        bool CheckCollision(double worldX, double worldY) const;

        
        // Return map size getters (JP: マップサイズ取得)
        int MAP_Get_SizeX()const { return MAP_WIDTH; };
        int MAP_Get_SizeY()const { return MAP_HEIGHT; };
        // Expose obstacle handle (JP: 障害物ハンドルを公開)
        int Get_ObjectHanadle()const { return o_object[0]; };
        // Expose map background handle (JP: マップ背景ハンドルを公開)
        int Get_Maphandle()const { return m_handles[(int)StageType::stage1]; };

        
    };


