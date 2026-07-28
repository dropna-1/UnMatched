#pragma once

#include "IAbility.hpp"

class SherlockAbility : public IAbility
{
    public:
        std::string getName() const override;
        std::string getDescription() const override;

        bool allowCancel(Card* card , GameContext& context) override; 
        bool HasAbilityOnStart() override ;       
};
