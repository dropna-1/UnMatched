#ifndef IEFFECTS_HPP
#define IEFFECTS_HPP

#include <vector>
#include <memory>

#include "Game/Characters/Character.hpp"

class GameContext;
class Card;

class IEffect
{
public:
    virtual void execute( GameContext& context, const std::vector<Character*>& targets ) = 0;
    virtual std::shared_ptr<IEffect> clone() const = 0;
    virtual ~IEffect() = default;
};

#endif