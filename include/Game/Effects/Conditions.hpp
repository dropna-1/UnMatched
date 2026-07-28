#pragma once 

#include "Game/Board/board.hpp"
#include "Game/Characters/Character.hpp"
#include "IConditions.hpp"

bool areAdjacent(const Board* board , const Character* first , const Character* Second) ;
bool areInSameZone(const Board* board , const Character* first , const Character* Second) ;
//bool IsInZone(const Board* board , const Character* first , int zone) ;

class AdjacentCondition : public IConditions
{
    private :
        ConditionTarget first ;
        ConditionTarget Second ;
    public :
        AdjacentCondition(ConditionTarget first , ConditionTarget Second) ;
        bool check(GameContext& context) const override ;
};

class WonBattleCondition : public IConditions
{
    private :
        ConditionTarget Winner ;
    public :
        WonBattleCondition(ConditionTarget) ;
        bool check(GameContext& context) const override ;
};

class LossBattleCondition : public IConditions
{
    private :
        ConditionTarget Winner ;
    public :
        LossBattleCondition(ConditionTarget) ;
        bool check(GameContext& context) const override ;
};  

class AreDeadSisters : public IConditions
{
    private :
        ConditionTarget sisters ;
    public : 
        AreDeadSisters(ConditionTarget) ;
        bool check(GameContext& context) const override ;
};

