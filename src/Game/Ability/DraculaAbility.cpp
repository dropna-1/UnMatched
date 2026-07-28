#include "Game/Ability/DraculaAbility.hpp"
#include "Game/Cards/Deck.hpp"
#include "Game/Pending/Pending.hpp"
#include "Game/Game.hpp"
// inam esm o tozihatesho badan misazam
std::string DraculaAbility::getName() const
{
    return "Dracula Ability" ;
}

std::string DraculaAbility::getDescription() const
{
    return "At the start of your turn, you may deal 1 damage to a fighter adjacent to Dracula.If you do draw a card.";
}

void DraculaAbility::execute(Character* target , Hero* hero)
{
    target->takeDamage(1) ;
    hero->getDeck()->drawCard() ;
}

void DraculaAbility::SendRequest(Game* game)
{
    game->requestAction(make_unique<DraculaAction>()) ;
}

bool DraculaAbility::HasAbilityOnStart() 
{
    return true ;
}