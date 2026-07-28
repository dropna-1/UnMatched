#include "Game/Characters/Character.hpp"
using namespace std ;

Character::Character(const string& name , int maxHP , int movement , AttackType attackType) : 
    name(name) ,
    maxHp(maxHP) ,
    hp(maxHP)  ,
    movement(movement) ,
    position(-1) 
{
}

void Character::takeDamage(int damage) 
{
    hp -= damage;
    if(hp < 0)
    {
        hp = 0 ;
    }
}

void Character::heal(int amount)
{
    hp += amount ;
    if(hp > maxHp)
    {
        hp = maxHp ;
    }
}

bool Character::isAlive() const
{
    return hp>0 ;
}

int Character::getPosition() const
{
    return position ;
}

void Character::setPosition(int newPosition)
{
    position = newPosition ;
}

string Character::getname() const
{
    return name ; 
}

int Character::getHp() const
{
    return hp;
}

int Character::getMaxhp() const
{
    return maxHp;
}

int Character::getMovement() const
{
    return movement;
}

AttackType Character::getAttackType() const
{
    return attackType ;
}