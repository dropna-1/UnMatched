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
    shared_ptr<Hero> getHero() const;
    int getAge() const;
    string getName() const;
    vector<Character*> getAllCharacters() const;
    friend bool operator==(const Player& p1, const Player& p2);
    PlayerSave createSaveData() const;
};