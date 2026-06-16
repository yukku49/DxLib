#pragma once
// Stage type used to select which background image to draw
// (JP: 背景画像の選択に使うステージ種別)
    enum class StageType {
        stage1 = 0,
        stage2,
        MAX
    };
    // Object type used to select which obstacle image to draw
// (JP: 障害物画像の選択に使うオブジェクト種別)
    enum class ObjectType {
        Object_1 = 0,
        Object_2,
        MAX
    };
    // Manages the stage background and tile-based collision map
// (JP: ステージ背景とタイルベースの衝突マップを管理するクラス)
    class BackScreen
    {
    private:

        // Number of tiles in X direction (1280px / 32px per tile)
        // (JP: X方向のタイル数。1280 / 32)
        static const int MAP_WIDTH = 40; 

        // Number of tiles in Y direction (736px / 32px per tile)
        // (JP: Y方向のタイル数。736 / 32)
        static const int MAP_HEIGHT = 23;

        // Size of one tile in pixels
        // (JP: 1タイルのピクセルサイズ)
        int const displaySize = 32;

        // 2D array storing tile values: 1 = passable, 0 = obstacle
        // (JP: タイル値を格納する2次元配列。1=通行可、0=障害物)
        int m_stageMap[MAP_HEIGHT][MAP_WIDTH]; 

        // Handles for background images, indexed by StageType
         // (JP: StageType でインデックスされた背景画像ハンドル)
        int m_handles[(int)StageType::MAX];

        // Handles for obstacle images, indexed by ObjectType
        // (JP: ObjectType でインデックスされた障害物画像ハンドル)
        int o_object[(int)ObjectType::MAX];

        // Currently active stage type
        // (JP: 現在アクティブなステージ種別)
        StageType m_currentType;

        // Copies a tile pattern into m_stageMap starting at (startX, startY)
        // partType selects which pattern to use (currently only 1 is defined)
        // (JP: 指定パターンをm_stageMapの(startX, startY)からコピーする)
        void CopyPart(int startX, int startY, int partType);

    public:
        // Loads background/obstacle images and initializes the tile map
        // Must be called once before any other method
        // (JP: 背景・障害物画像を読み込み、タイルマップを初期化する。最初に1回呼ぶ)
        void Initialize(); 

        // Returns the tile value at tile coordinates (x, y)
        // Returns 0 (obstacle) if out of bounds
        // (JP: タイル座標(x,y)のタイル値を返す。範囲外は0=障害物)
        int GetMapvalue(int x, int y) const;

        // Check collision by world position (JP: ワールド座標で衝突判定)
        bool CheckCollision(double worldX, double worldY) const;

        // Returns true if the world-pixel coordinate (worldX, worldY) is inside an obstacle tile
        // Used for collision detection by player, enemy, and bullets
        // Note: worldY has a 32px HUD offset subtracted internally
        // (JP: ワールドピクセル座標が障害物タイル内なら true を返す。HUDオフセット32pxを内部で減算)
        int MAP_Get_SizeX()const { return MAP_WIDTH; };

        // Returns the tile map height in tiles
        // (JP: タイルマップの縦幅をタイル単位で返す)
        int MAP_Get_SizeY()const { return MAP_HEIGHT; };

        // Returns the image handle for the first obstacle type (pizza box)
        // (JP: 最初の障害物画像ハンドル（ピザボックス）を返す)
        int Get_ObjectHanadle()const { return o_object[0]; };

        // Returns the image handle for the stage1 background
        // (JP: ステージ1背景の画像ハンドルを返す)
        int Get_Maphandle()const { return m_handles[(int)StageType::stage1]; };

        
    };


