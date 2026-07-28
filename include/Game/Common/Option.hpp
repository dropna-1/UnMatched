#pragma once
#include <string>

class Character;

struct Option {
    std::string text;
    int id;

    bool operator<(const Option& other){
        return id < other.id; 
    }
};

struct AttackOption {
    Character* attacker;
    Character* target;
};