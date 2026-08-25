#pragma once

#include "IAbility.hpp"

class InvisibleManAbility : public IAbility
{
    public:
        bool allowCancel(Card* card, GameContext& context) override;

        bool HasAbilityOnStart() override;

        int getDefenseBonus(
            Character* defender,
            GameContext& context
        ) override;

        std::string getName() const override;

        std::string getDescription() const override;
};