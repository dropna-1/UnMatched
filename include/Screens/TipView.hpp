#pragma once
#include "raylib.h"
#include "Game/Enums/TypeEnums.hpp"
#include <map>
#include <string>

class Player;
class Game;

class TipView{
private:
    std::map<Stage, std::string> message;
public:
    TipView() = default;
    ~TipView() = default;
    void FillMessage(Player& pc, Player& po);
    void Draw(Rectangle t, Stage* stage, Font& font);
};