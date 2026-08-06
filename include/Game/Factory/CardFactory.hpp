#pragma once 

#include <vector>
#include <memory>
#include "Game/Cards/Cards.hpp"
#include "Game/Cards/Deck.hpp"

class CardFactory
{
    private :
        static std::shared_ptr<Card> createCard(
        const std::string& name,
        CardType type,
        FighterType fighter,
        TriggerType trigger ,
        int value,
        int boost,
        const std::string& description , 
        const string& pathID
        );
        static void addCopies(
        std::shared_ptr<Deck> deck,
        int count,
        const std::shared_ptr<Card>& card
        );
    public:
        static std::shared_ptr<Deck> createSherlockDeck();
        static std::shared_ptr<Deck> createDraculaDeck();
};