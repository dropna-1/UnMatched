#include "Game/Common/SaveManager.hpp"
#include <fstream>
#include <filesystem>

bool SaveManager::isValidSlot(int slot){
    return slot >= 1 && slot <= 5;
}

std::string SaveManager::getPath(int slot){
    return "saves/save" + std::to_string(slot) + ".json";
}

bool SaveManager::slotExists(int slot){
    if(!isValidSlot(slot))
        return false;
    return std::filesystem::exists(getPath(slot));
}

bool SaveManager::deleteSlot(int slot)
{
    if(!isValidSlot(slot))
        return false;
    try{
        if(!std::filesystem::exists(getPath(slot)))
            return true;
        return std::filesystem::remove(getPath(slot));
    }
    catch(...){
        return false;
    }
}

bool SaveManager::saveGame(const GameSave& save, int slot)
{
    if(!isValidSlot(slot))
        return false;
    try{
        const std::string path = getPath(slot);
        const std::string tempPath = path + ".tmp";
        json j = save;
        {
            std::ofstream file(tempPath);
            if(!file.is_open())
                return false;

            file << j.dump(4);
            if(!file.is_open()){
                file.close();
                std::filesystem::remove(tempPath);
                return false;
            }
        }
        std::filesystem::remove(path);
        std::filesystem::rename(tempPath, path);
        return true;
    }
    catch(...){
        return false;
    }
}

bool SaveManager::loadGame(GameSave& save, int slot)
{
    if(!isValidSlot(slot))
        return false;

    try{
        std::ifstream file(getPath(slot));
        if(!file.is_open())
            return false;

        json j;
        file >> j;
        save = j.get<GameSave>();
    }
    catch(const std::exception&){
        return false;
    }
    return true;
}