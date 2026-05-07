#include "DxLib.h"
#include "BackScreenManagiment.h"

// Stage passable/obstacle layout (1: passable, 0: obstacle) (JP: tsuro to shogai)
int statgeaPatern_1[23][40] =
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
  0,1,0,1,0,0,0,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,1,0,0,0,0,0,0,0,1,0,
  0,1,0,1,0,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,1,0,0,0,0,0,0,0,1,0,
  0,1,0,1,0,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,1,0,0,0,0,0,0,0,1,0,
  0,1,0,1,0,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,1,1,1,1,1,1,1,1,1,0,
  0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,0,0,0,0,1,0,1,0,
  0,1,0,1,0,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,1,0,0,0,0,0,1,0,1,0,
  0,1,0,1,0,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,1,1,1,0,0,0,1,0,1,0,
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
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};



    void BackScreen::CopyPart(int startX, int startY, int partType)
    {
        if (partType == 1) {
            for (int y = 0; y < 23; y++) {
                for (int x = 0; x < 40; x++) {
                    // Bounds check for safety (JP: hani check)
                    if (startY + y < MAP_HEIGHT && startX + x < MAP_WIDTH) {
                        m_stageMap[startY + y][startX + x] = statgeaPatern_1[y][x];
                    }
                }
            }
        }
    }

    void BackScreen::Initialize()
    {
        // 1. Load images with LoadGraph (JP: LoadGraph de image load)
        // Store returned handles (integer IDs) (JP: handle hozon)
        m_handles[0] = LoadGraph("../Pizza_Image/stage1.png");
        m_handles[1] = LoadGraph("../Pizza_Image/stage2.png");
        // Load obstacle images (JP: shogai image load)
        o_object[0] = LoadGraph("../Pizza_Image/pizza box.png");
        o_object[1] = LoadGraph("../Pizza_Image/renga.png");

        // Optional load failure checks (JP: load shippai check)
        /*
        if (m_handles[0] == -1 || m_handles[1] == -1) {
            // Error handling (JP: error shori)
            exit(1);
        }
        else if (o_object[0] == -1 || o_object[1] == -1)
        {
            exit(1);
        }
        */
        CopyPart(0, 0, 1);



        // 3. Call map generation logic if needed (JP: hitsuyo nara map seisei)
       // GenerateRandomMap();
    }
    

    




    int BackScreen::GetMapvalue(int x, int y)const
    {

        if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT)
            return 0;
        return m_stageMap[y][x];
    }

    // Determine passable or blocked tile (JP: tsuko kanou hantei)
    bool BackScreen::CheckCollision(double worldX, double worldY) const
    {
        int Worldx = (int)worldX / 32;
        int Worldy = (int)worldY / 32;
        // Read tile type via GetMapvalue (JP: tile atai shutoku)
        int chipType = this->GetMapvalue(Worldx, Worldy);
        // Check if tile is blocked (JP: kabe hantei)
        if (chipType == 1)
        {
            return true;
        }
        return false;
    }

   
    /*
    bool StageManager::IsWalkable(int x, int y)
    {

        return false;
    }
    */

