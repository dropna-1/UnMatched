#include "Game/Characters/Hero.hpp"
#include "Game/Ability/IAbility.hpp"
#include "Game/Cards/Deck.hpp"
#include "Game/Characters/SideKick.hpp"
using namespace std; 

#include <iostream> 
using namespace std ;

Hero::Hero(const string& name , int hp,
            int movement,
            AttackType attackType,
            HeroType type) :
            Character(name , hp , movement ,attackType) ,herotype(type)  
{
}

HeroType Hero::getHeroType() const
{
    return herotype;
}

shared_ptr<Deck> Hero::getDeck()
{
    return deck;
}

shared_ptr<IAbility> Hero::getAbility()
{
    return ability;
}

vector<shared_ptr<SideKick>>& Hero::getSidekicks()
{
    return sidekicks;
}

void Hero::setDeck(shared_ptr<Deck> deck)
{
    this->deck = deck;
}

void Hero::setAbility(shared_ptr<IAbility> ability)
{
    this->ability = ability;
}

void Hero::addSidekick(shared_ptr<SideKick> sidekick)
{
    sidekicks.push_back(sidekick);
}

bool Hero::hasAliveSidekicks() const
{
    for (const auto& sidekick : sidekicks)
    {
        if (sidekick->isAlive())
        {
            return true;
        }
    }
    return false;
}

void Hero::printInfo() const
{
    cout << "=========================\n";
    cout << "Hero : " << name << '\n';
    cout << "HP : " << hp << "/" << maxHp << '\n';
    cout << "Movement : " << movement << '\n';

    cout << "Attack Type : ";

    if (attackType == AttackType::Melee)
        cout << "Melee\n";
    else
        cout << "Ranged\n";

    cout << "Sidekicks : " << sidekicks.size() << '\n';

    if (ability)
        cout << "Ability Loaded\n";
    else
        cout << "No Ability\n";

    if (deck)
        cout << "Deck Loaded\n";
    else
        cout << "No Deck\n";

    cout << "=========================\n";
}
            
bool Hero::isHero() const
{
    return true ;
}