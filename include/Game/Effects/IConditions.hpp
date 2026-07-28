#pragma once 
#include "Game/Effects/GameContext.hpp"

class GameContext ;

class IConditions 
{
    public :
        virtual bool check(GameContext& context) const = 0 ; 
        virtual ~IConditions() = default ;
};