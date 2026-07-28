#include "Game/Cards/Deck.hpp"
#include "Game/Cards/Cards.hpp"

#include <random>
#include <algorithm>
using namespace std ;

void Deck::addcard(shared_ptr<Card> card)
{
    drawPile.push_back(card) ;
}

void Deck::shuffleDeck()
{
    random_device rd ;
    mt19937 generator(rd()) ;
    shuffle(drawPile.begin() , drawPile.end() , generator) ;
}

bool Deck::drawCard()
{
    if(drawPile.empty())
    {
        return false ;
    }
    hand.push_back(drawPile.back()) ;
    drawPile.pop_back() ;
    return true ;
}

void Deck::discardFromHand(int handIndex)
{
    if(handIndex < 0 || handIndex  >= hand.size())
    {
        return ;
    }
    discardPile.push_back(hand[handIndex]) ;
    hand.erase(hand.begin() + handIndex) ;
}

void Deck::discardCard(shared_ptr<Card> card)
{
    if(card== nullptr)
    {
        return; 
    }
    discardPile.push_back(card) ;
}

int Deck::getDrawPileSize() const
{
    return drawPile.size();
}

int Deck::getHandSize() const
{
    return hand.size();
}

int Deck::getDiscardPileSize() const
{
    return discardPile.size();
}

const vector<shared_ptr<Card>>& Deck::getHand() const
{
    return hand;
}

shared_ptr<Card> Deck::playCard(int handIndex)
{
    if(handIndex < 0 || handIndex>= hand.size() )
    {
        return nullptr ;
    }
    auto card = hand[handIndex] ;
    hand.erase(hand.begin() + handIndex);
    return card ;

}

Card* Deck::previewCard(int index) const 
{
    return hand[index].get() ;
}