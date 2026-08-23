#pragma once

#include "IEffects.hpp"
#include "GameContext.hpp"
#include "Game/Cards/Deck.hpp"


class DamageEffect : public IEffect
{
    private :
        int damage ;
    public :
        DamageEffect(int damage) ;
        void execute(GameContext& context , const vector<Character*>& targets) override ;
};

class DrawCardEffect : public IEffect
{
    private:
        int count ;
    public:
        DrawCardEffect(int count);
        void execute(GameContext& context , const vector<Character*>& targets) override ;
};

class HealEffect : public IEffect
{   
    private:
        int heal;
    public:
        HealEffect(int heal);
        void execute(GameContext& context , const vector<Character*>& targets) override ;
};

class MoveEffect : public IEffect
{
    private:
        int distance;
    public:
        MoveEffect(int distance);
        void execute(GameContext& context , const vector<Character*>& targets) override ;
};

class DiscardCardEffect : public IEffect
{
    private :
        int count ;
    public :
        DiscardCardEffect(int count) ;
        void execute(GameContext& context , const vector<Character*>& targets) override ;
};

class CancelEffectsEffect : public IEffect
{
    public :
        void execute(GameContext& context , const vector<Character*>& targets) override ;   
};

class SwapEffect : public IEffect
{
    private :
        EffectTarget first ;
        EffectTarget second ;
    public :
        void execute(GameContext& context , const vector<Character*>& targets) override ;
};

class MoveToAdjacentEffect : public IEffect  
{
    public :
        void execute(GameContext& context , const vector<Character*>& targets) override ;

};

class DeduceEffect : public IEffect
{
    public : 
        void execute(GameContext& context , const vector<Character*>& targets) override ;
};

class ReviveSister : public IEffect
{
    public :
        void execute(GameContext& context , const vector<Character*>& targets) override ;
};

class AmbushEffect : public IEffect 
{
    public : 
        void execute(GameContext& context , const vector<Character*>& targets) override ;
};

class GainActionEffect : public IEffect 
{
    public :
        void execute(GameContext& context , const vector<Character*>& targets) override ;
};

class FeedingFrenzyEffect : public IEffect 
{
    public :
        void execute(GameContext& context , const vector<Character*>& targets) override ;
};

class RaveningSeduction : public IEffect
{
    public : 
        void execute(GameContext& context , const vector<Character*>& targets) override ;
};

class PreyUponEffect : public IEffect
{
    public :
        void execute(GameContext& context , const vector<Character*>& targets) override ;

};

class LookIntoMyEyesEffect : public IEffect 
{
    public :
        void execute(GameContext& context , const vector<Character*>& targets) override ;
};

class ThirstEffect : public IEffect 
{
    public :
        void execute(GameContext& context , const vector<Character*>& targets) override ;
};

class RaveningEffect : public IEffect
{
    public :
        void execute(GameContext& context , const vector<Character*>& targets) override ;
};

class BeastFormEffect : public IEffect
{
    public :
        void execute(GameContext& context , const vector<Character*>& targets) override ;
};

class ShowHandEffect : public IEffect
{
    public :
        void execute(GameContext& context , const vector<Character*>& targets) override ;
};

class EmergeFromMistEffect : public IEffect
{
    public:
        void execute( GameContext& context, const vector<Character*>& targets ) override;
};