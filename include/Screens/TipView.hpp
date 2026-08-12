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
    std::map<RequestType, std::string> pendingMessage;
public:
    TipView() = default;
    ~TipView() = default;
    void FillMessage(Player& pc, Player& po);
    void Draw(Rectangle t, Stage* stage, Game& game, Font& font);
};