#pragma once

#include <iostream>
#include "Game/Cards/Cards.hpp"
#include "Game/Characters/Character.hpp"

class Game;
class GameContext;
class Card;
class Hero;

class IAbility
{
    public:
        virtual ~IAbility() = default;
        virtual bool allowCancel(Card* card , GameContext& context) 
        {
            return true ;
        }

        virtual bool HasAbilityOnStart() = 0;
        virtual void SendRequest(Game*) 
        {
        }
        virtual void execute(Character* , Hero*) 
        {
        }
        virtual std::string getName() const = 0;
        virtual std::string getDescription() const = 0;
        virtual int getDefenseBonus(Character* defender, GameContext& context)
        {
            return 0;
        }
};