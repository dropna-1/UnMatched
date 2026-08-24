#ifndef CARD_HPP
#define CARD_HPP

#include <iostream>
#include <vector> 
#include <memory>
#include "Game/Enums/TypeEnums.hpp"
#include "Game/Effects/IConditions.hpp"
#include "Game/Effects/IEffects.hpp"

class IConditions;

struct EffectEntry
{
    TriggerType trigger ;
    EffectTarget target ;
    std::shared_ptr<IConditions> condition;
    std::shared_ptr<IEffect> effect ;
};

class Card 
{
    private:
        std::string name ; 
        CardType type ; 
        TriggerType trigger ;
        FighterType fighter ;
        int value ;
        int boost ;
        std::string description ;
        std::vector<EffectEntry> effects ;
        std::string pathID;
        bool valueLocked = false;
    public :
        Card(const std::string& name,
            CardType type,
            FighterType fighter,
            TriggerType trigger , 
            int value,
            int boost ,
            const std::string& description, 
            const std::string& pathID);
        void addEffect(TriggerType , EffectTarget , std::shared_ptr<IConditions> , std::shared_ptr<IEffect>) ;
        const std::string& getName() const;
        int getValue() const;
        int getBoost() const;
        CardType getType() const;
        FighterType getFighter() const;
        TriggerType getTrigger() const;
        const std::string& getDescription() const ;
        std::vector<EffectEntry>& getEffects() ;
        const std::vector<EffectEntry>& getEffects() const ;
        void execute(TriggerType , GameContext&) ;
        void setBoost(int) ;
        void setValue(int) ;
        std::string getId() const ;
        void lockValue();
        bool isValueLocked() const;
};


#endif