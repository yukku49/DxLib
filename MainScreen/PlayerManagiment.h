#pragma once
// Avoid including DrawManagiment.h here (JP: koko de include shinai)
#include <array>

enum Player_EyeContact
{
	PlayerEye_Up = 0,
	PlayerEye_Down,
	PlayerEye_Left,
	PlayerEye_Right,
	PlayerEye_Max
};

struct Item_count
{
	int Tmato_Counter;
	int Basil_Counter;
	int Cheese_Counter;
	int Gorgonzola_Counter;
	int Pizzadough_Counter;
};

class Item_Managiment;
class Bllent_Managiment;
class BackScreen; // Forward declaration (JP: zenpo sengen)

class Player_Managiment
{
private:
	// Player initial position constants (JP: shoki ichi teisu)
	const float Player_StanderdpointX = 1.0f;
	const float Player_StanderdpointY = 1.0f;

	// Player movement position values (JP: ido ichi hensu)
	float Player_MovePointX = 0;
	float Player_MovePointY = 0;

	// Player image handles by direction (JP: houkou betsu handle)

	int PlayerImage_Handle[Player_EyeContact::PlayerEye_Max];

	// Current player draw handle (JP: genzai hyoji handle)
	int Player_Handle;

	const int m_displaySize = 28;// Draw size (JP: hyouji saizu)

	// Previous key state for single-step input (JP: tanoshi nyuryoku mae frame)
	int m_oldUp, m_oldDown, m_oldLeft, m_oldRight;

	// Current facing direction (JP: muki joho)
	int m_dir;

	// Structure for held ingredients (JP: shoji sozai kozo)
	Item_count Player_Itembring;
public:
	void Initialisation();
	// Update with stage collision and bullet actions (JP: shoutotsu to dan update)
	void Update(const BackScreen& stage, Bllent_Managiment& bllent);

	// Read-only position getters (JP: yomitori senyo getter)
	int GetX()const { return (int)Player_MovePointX; }
	int GetY()const { return (int)Player_MovePointY; }

	// Return current player sprite handle (JP: genzai sprite handle)
	int Get_PlayerHanadle()const { return Player_Handle; }

	// Return held ingredient data (JP: shoji data kaesu)
	Item_count Get_Player_Itembring()const { return Player_Itembring; };

	// Increase held item counters (JP: count zoka)
	void Player_BringItem(Item_Managiment& item);

	// Getter for current facing direction (JP: muki getter)
	int  GetDir()const { return m_dir; };


	// Player draw function is declared in Draw header (JP: draw sengen wa hoka)
	//void Draw_Managiment::DrawManager();
};
