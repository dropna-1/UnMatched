#include "Game/Effects/Conditions.hpp"
#include <algorithm>
using namespace std; 

bool areAdjacent(const Board* board , const Character* first , const Character* Second) 
{
    const auto& neighbors = board->getSpace(first->getPosition()).neighbors ;
    return (find(neighbors.begin(), neighbors.end() , Second->getPosition()) != neighbors.end()) ;
}

bool areInSameZone(const Board* board , const Character* first , const Character* Second)
{
    const auto& firstZone = board->getSpace(first->getPosition()).zone;
    const auto& SecondZone = board->getSpace(Second->getPosition()).zone ; 
    for(int zone : firstZone)
    {
        if(find(SecondZone.begin() , SecondZone.end() , zone) != SecondZone.end())
        {
            return true ;
        }
    }
    return false ; 
}

AdjacentCondition::AdjacentCondition(ConditionTarget first , ConditionTarget Second) : first(first) , Second(Second) 
{
}

bool AdjacentCondition::check(GameContext& context) const
{
    auto FirstTarget = context.resolve(first) ;
    auto SecondTarget = context.resolve(Second) ;
    for(auto* c1 : FirstTarget)
    {
        for (auto* c2 : SecondTarget)
        {
            if(areAdjacent(context.getBoard() , c1 , c2)) 
            {
                return true ;
            }
        }
    }
    return false ;
}

WonBattleCondition::WonBattleCondition(ConditionTarget Winner) : Winner(Winner)
{
}

bool WonBattleCondition::check(GameContext& context) const
{
    auto IsWinner = context.resolve(Winner); 
    for(auto c : IsWinner)
    {
        if(c == context.getWinner())
        {
            return true ;
        }
    }
    return false ;
}

LossBattleCondition::LossBattleCondition(ConditionTarget Winner) : Winner(Winner)
{
}

bool LossBattleCondition::check(GameContext& context) const
{
    auto IsWinner = context.resolve(Winner); 
    for(auto c : IsWinner)
    {
        if(c != context.getWinner())
        {
            return true ;
        }
    }
    return false ;
}

AreDeadSisters::AreDeadSisters(ConditionTarget target)
{
    sisters = target ;
}
bool AreDeadSisters::check(GameContext& context) const
{ 
    for(auto sister : context.resolve(sisters))
    {
        if(!sister->isAlive())
        {
            return true ;
        }
    }
    return false ;
};
