#pragma once

// ‘O•ûéŒ¾‚ÅzŠÂ‚ğ”ğ‚¯‚é
class BackScreen;
class Player_Managiment;
class Bllent_Managiment;
class Enemy_Managiment;

class DrawManager
{
public:
    void Player_Draw(const BackScreen& stage, const Player_Managiment& player) const;
    void Map_Draw(const BackScreen& object) const;
    void Bullets_Draw(const Bllent_Managiment& bullets) const;
    void Enemy_Draw(const Enemy_Managiment& enemy, const BackScreen& stage) const;
};