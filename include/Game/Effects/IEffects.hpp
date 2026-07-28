#ifndef IEFFECTS_HPP
#define IEFFECTS_HPP

#include <vector>
#include "Game/Characters/Character.hpp"

class GameContext ; 
class Card ;

class IEffect
{
    public :
        virtual void execute(GameContext& context , const std::vector<Character*>& targets) = 0 ;
        virtual ~IEffect() = default ;
};

#endif