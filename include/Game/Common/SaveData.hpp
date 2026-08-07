#include <string>
#include "Enums/TypeEnums.hpp"

struct HeroSave {
    HeroType type;
    int HP = -1;
    int position = -1;
};

struct PlayerSave {
    std::string name;
    int age = -1;
    int heroId = -1;
};