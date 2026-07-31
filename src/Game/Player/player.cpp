#include "Game/Player/player.hpp"
#include "Game/Characters/SideKick.hpp"
#include "Game/Characters/Hero.hpp"


void Player::setHero(shared_ptr<Hero> hero){
    this->hero = hero;
}

void Player::setName(const string& n){
    this->name = n;
}

void Player::setAge(const int& a){
    this->age = a;
}

shared_ptr<Hero> Player::getHero(){
    return hero;
}

int Player::getAge(){
    return age;
}

string Player::getName(){
    return name;
}

vector<Character*> Player::getAllCharacters(){
    vector<Character*> characters;
    if(hero.get()->isAlive())
        characters.push_back(hero.get());

    for(auto sidekick : hero.get()->getSidekicks())
        if(sidekick.get()->isAlive())
            characters.push_back(sidekick.get());
            
    return characters;
}


bool operator==(const Player& p1, const Player& p2){
    return p1.name == p2.name && p1.hero->getname() == p2.hero->getname();
}