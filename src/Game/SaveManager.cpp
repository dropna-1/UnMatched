#include "Game/Common/SaveManager.hpp"
#include <fstream>

bool SaveManager::saveGame(const GameSave& save, const std::string& path)
{
    std::ofstream file(path, std::ios::out);
    if(!file.is_open())
        return false;

    nlohmann::json j = save;
    file << j.dump(4);

    return true;
}

bool SaveManager::loadGame(GameSave& save, const std::string& path)
{
    std::ifstream file(path);
    if(!file.is_open())
        return false;

    try{
        nlohmann::json j;
        file >> j;
        save = j.get<GameSave>();
    }
    catch(const std::exception&){
        return false;
    }
    return true;
}