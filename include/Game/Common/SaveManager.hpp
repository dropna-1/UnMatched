#pragma once
#include <string>
#include "Game/Common/SaveData.hpp"

class SaveManager
{
public:
    static bool saveGame(const GameSave& save, const std::string& path);
    static bool loadGame(GameSave& save, const std::string& path);
};