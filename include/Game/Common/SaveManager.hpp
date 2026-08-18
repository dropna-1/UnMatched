#pragma once
#include <string>
#include "Game/Common/SaveData.hpp"

class SaveManager
{
private:

    static std::string getPath(int slot);
    static bool isValidSlot(int slot);

public:

    static bool saveGame(const GameSave& save, int slot);
    static bool loadGame(GameSave& save, int slot);
    static bool slotExists(int slot);
    static bool deleteSlot(int slot);

};