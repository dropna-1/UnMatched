#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include "Game/Common/SaveData.hpp"
using namespace std;

class Hero;
class Character;

class Player {
    string name;
    int age;
    shared_ptr<Hero> hero;
public:
    Player() = default;
    void setHero(shared_ptr<Hero> hero);
    void setName(const string& n);
    void setAge(const int& a);
    shared_ptr<Hero> getHero();
    int getAge();
    string getName();
    vector<Character*> getAllCharacters();
    friend bool operator==(const Player& p1, const Player& p2);
    PlayerSave createSaveData() const;
};