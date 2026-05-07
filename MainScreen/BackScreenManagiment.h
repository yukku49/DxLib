#pragma once
    // Stage type definitions (JP: stage shubetsu)
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
        static const int MAP_WIDTH = 40;  // 1280 / 32 tile count X (JP: X tile su)
        static const int MAP_HEIGHT = 23; // 720 / 32 tile count Y (JP: Y tile su)
        int const displaySize = 32;
        int m_stageMap[MAP_HEIGHT][MAP_WIDTH]; // Stage map array (JP: map hairetsu)
        int m_handles[(int)StageType::MAX];    // Background handles (JP: haikei handle)
        int o_object[(int)ObjectType::MAX];
        StageType m_currentType;
        void CopyPart(int startX, int startY, int partType);

    public:
        void Initialize(); // Load images and initialize map (JP: image load to map shokika)
        // Return tile value 0/1 (JP: masu atai kaesu)
        int GetMapvalue(int x, int y) const;
        // Check collision by world position (JP: world zahyo hantei)
        bool CheckCollision(double worldX, double worldY) const;

        
        // Return map size getters (JP: map saizu getter)
        int MAP_Get_SizeX()const { return MAP_WIDTH; };
        int MAP_Get_SizeY()const { return MAP_HEIGHT; };
        // Expose obstacle handle (JP: shogai handle koukai)
        int Get_ObjectHanadle()const { return o_object[0]; };
        // Expose map background handle (JP: map handle koukai)
        int Get_Maphandle()const { return m_handles[(int)StageType::stage1]; };

        // Check whether item can be placed (JP: item haichi kanou)
     //   bool IsWalkable(int x, int y);
    };


