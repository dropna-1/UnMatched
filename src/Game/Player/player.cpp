#include "Game/Player/player.hpp"
#include "Game/Characters/SideKick.hpp"
#include "Game/Characters/Hero.hpp"
#include "Game/Cards/Deck.hpp"
#include "Game/Characters/InvisibleMan.hpp"


void Player::setHero(shared_ptr<Hero> hero){
    this->hero = hero;
}

void Player::setName(const string& n){
    this->name = n;
}

void Player::setAge(const int& a){
    this->age = a;
}

shared_ptr<Hero> Player::getHero() const{
    return hero;
}

int Player::getAge() const{
    return age;
}

string Player::getName() const{
    return name;
}

vector<Character*> Player::getAllCharacters() const{
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


PlayerSave Player::createSaveData() const
{
    PlayerSave save;

    save.name = name;
    save.age = age;

    save.hero.type = hero->getHeroType();
    save.hero.HP = hero->getHp();
    save.hero.position = hero->getPosition();

    int index = 0;

    if(hero->getHeroType() == HeroType::Invisibleman){
        InvisibleMan* h = dynamic_cast<InvisibleMan*>(hero.get());
        for(const auto& fog : h->getFogs()){
            FogSave fogSave;
            fogSave.position = fog.getPosition();
            save.hero.fogs.push_back(fogSave);
        }
    }
    else{
        for(const auto& sidekick : hero->getSidekicks())
        {
            SidekickSave sidekickSave;

            sidekickSave.name = sidekick->getname();
            sidekickSave.index = index++;
            sidekickSave.HP = sidekick->getHp();
            sidekickSave.position = sidekick->getPosition();

            save.hero.sidekicks.push_back(sidekickSave);
        }
    }

    auto deck = hero->getDeck();

    for(const auto& card : deck->getDrawPile())
        save.deck.drawPile.push_back(card->getId());

    for(const auto& card : deck->getHand())
        save.deck.hand.push_back(card->getId());

    for(const auto& card : deck->getDiscardPile())
        save.deck.discardPile.push_back(card->getId());

    return save;
}