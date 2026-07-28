#pragma once 

#include <memory>
#include <vector>

class Card ;

class Deck 
{
    private :
        std::vector<std::shared_ptr<Card>> drawPile;
        std::vector<std::shared_ptr<Card>> hand;
        std::vector<std::shared_ptr<Card>> discardPile; 
    public :
        Deck() = default ;
        void shuffleDeck() ;
        void addcard(std::shared_ptr<Card> card) ;
        bool drawCard();
        void discardCard(std::shared_ptr<Card> card);
        void discardFromHand(int) ;
        int getDrawPileSize() const;
        int getHandSize() const;
        int getDiscardPileSize() const;        
        const std::vector<std::shared_ptr<Card>>& getHand() const;
        std::shared_ptr<Card> playCard(int handindex) ;
        Card* previewCard(int) const ;
    
};