#include "Game/Characters/SideKick.hpp"

SideKick::SideKick(const std::string& name, int maxHp, int movement , AttackType attacktype ) : Character(
    name , 
    maxHp ,
    movement,
    attacktype) 
{
}

void SideKick::printInfo() const
{
    std::cout << "Sidekick : " << name << '\n';
    std::cout << "Health   : " << hp
              << "/" << maxHp << '\n';
}

bool SideKick::isHero() const 
{
    return false ;
}