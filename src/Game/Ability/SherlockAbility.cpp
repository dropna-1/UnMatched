#include "Game/Ability/SherlockAbility.hpp"
using namespace std ;

string SherlockAbility::getName() const
{
    return "Sherlock ability";
}

string SherlockAbility::getDescription() const
{
    return "Effects on HOLMES and DR.WATSON cards cannot be canceled by an opponet.(Effects on ANY cards can be canceled)";
}

bool SherlockAbility::allowCancel(Card* card , GameContext& context) 
{
    if(card->getFighter() == FighterType::Any)
    {
        return true ;
    }
    return false ;
}

bool SherlockAbility::HasAbilityOnStart() 
{
    return false ;
}