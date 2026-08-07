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

struct SaveData {
    std::string player1Name;
    int player1Age;
    int player1Health; 
    
    std::string player2Name;
    int player2Age;
    int player2Health; 

    int currentPlayerIndex;
    int remainingAction;
};