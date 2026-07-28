#ifndef SIDEKICK_HPP
#define SIDEKICK_HPP

#include "Character.hpp"

class SideKick : public Character
{
    public :
        SideKick(const std::string& name, int maxHP, int movement , AttackType attacktype ) ;
        void printInfo() const ;
        bool isHero() const override ;
};

#endif 