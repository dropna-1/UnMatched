#pragma once

#include "IAbility.hpp"

class DraculaAbility : public IAbility
{
    public:
        void execute(Character* , Hero*) override ;
        void SendRequest(Game*) override ;
        std::string getName() const override;
        std::string getDescription() const override;
        bool HasAbilityOnStart() override ;
};