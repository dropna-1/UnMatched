#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <iostream> 
#include "Game/Enums/TypeEnums.hpp"

class Character
{
    protected :
        std::string name ; 
        int maxHp ;
        int hp ; 
        int movement ; 
        int position ;
        AttackType attackType ;

    public :
        Character(const std::string& name , int maxHP , int movement  ,AttackType attackType) ;
        
        virtual ~Character() = default ;
        virtual bool isHero() const= 0 ; 

        void takeDamage(int) ; 
        void heal(int) ;
        bool isAlive() const ;
        int getPosition() const ;
        void setPosition(int) ;

        std::string getname() const ; 
        int getMaxhp() const ;
        int getHp() const ;
        int getMovement() const ;
        AttackType getAttackType() const;
        virtual void printInfo() const = 0 ;
};


#endif