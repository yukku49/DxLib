#pragma once
    // ステージの種類を定義
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
        static const int MAP_WIDTH = 40;  // 1280 / 32　x座標
        static const int MAP_HEIGHT = 23; // 720 / 32　y座標
        int const displaySize = 32;
        int m_stageMap[MAP_HEIGHT][MAP_WIDTH]; // マップ配列
        int m_handles[(int)StageType::MAX];    // 背景ハンドル
        int o_object[(int)ObjectType::MAX];
        StageType m_currentType;
        void CopyPart(int startX, int startY, int partType);

    public:
        void Initialize(); // LoadGraphと配列の初期化
        //マスが０か１かをかえす
        int GetMapvalue(int x, int y) const;
        //画面外か画面内かを判定する
        bool CheckCollision(double worldX, double worldY) const;

        
        //マップサイズを返すMAP_Get_SizeXとY
        int MAP_Get_SizeX()const { return MAP_WIDTH; };
        int MAP_Get_SizeY()const { return MAP_HEIGHT; };
        //外部で障害物を扱えるようにする
        int Get_ObjectHanadle()const { return o_object[0]; };
        //外部にマップのハンドルを渡す
        int Get_Maphandle()const { return m_handles[(int)StageType::stage1]; };

        // アイテムが配置可能（床）かどうかを判定する関数
     //   bool IsWalkable(int x, int y);
    };


