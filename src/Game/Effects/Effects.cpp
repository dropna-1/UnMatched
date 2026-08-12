#include "Game/Effects/Effects.hpp"
#include "Game/Effects/Conditions.hpp"
#include "Game/Ability/IAbility.hpp"
#include "Game/Pending/Pending.hpp"
#include "Game/Game.hpp"
#include <cstdlib>
#include <algorithm>
using namespace std ;

DamageEffect::DamageEffect(int damage) : damage(damage)
{
}

void DamageEffect::execute(GameContext& context, const vector<Character*>& targets)
{
    for(auto character : targets)
    {
        character->takeDamage(damage);
    }
}

HealEffect::HealEffect(int heal) : heal(heal)
{
}

void HealEffect::execute(GameContext& context,const vector<Character*>& targets)
{
    for(auto character : targets)
    {
        character->heal(heal);
    }
}

DrawCardEffect::DrawCardEffect(int count) : count(count) 
{
}

void DrawCardEffect::execute(GameContext& context, const vector<Character*>& targets )
{
    auto deck = context.getCurrentPlayer()->getHero()->getDeck() ;
    for(int i = 0 ; i < count ; i++)
    {
        deck->drawCard() ;
    }
}

MoveEffect::MoveEffect(int distance) : distance(distance)
{
}
void MoveEffect::execute(GameContext& context, const vector<Character*>& targets)
{
    for(auto c :targets)
    {
        if(context.getGame()->getPendingCombat()->selection.destination == -1)
        {
            context.getGame()->requestAction(make_unique<MoveAction>(c , nullptr, MoveMode::AnySpace , distance)) ;
            return ;
        }
        if(context.getGame()->canMove(context.getGame()->getPendingCombat()->selection.destination))
        {
            context.getGame()->move(c , context.getGame()->getPendingCombat()->selection.destination) ;
        }
    }
}

DiscardCardEffect::DiscardCardEffect(int count) : count(count) 
{
}

void DiscardCardEffect::execute(GameContext& context, const vector<Character*>& targets )
{
    if(context.getGame()->getPendingCombat()->selection.cards.empty())
    {
        context.getGame()->requestAction(make_unique<ChooseCardAction>(context.getCurrentPlayer() , 1 , count)) ;
        return ;
    }
    auto index = context.getGame()->getPendingCombat()->selection.cards ;
    if(count == 0 )
    {
        return ;
    }
    for(auto card : index)
    {
        context.getEnemyPlayer()->getHero()->getDeck()->discardFromHand(card) ;
    }
}

void CancelEffectsEffect::execute(GameContext& context, const vector<Character*>& targets)
{
    
    if(context.getDefenderCard() == context.getCurrentCard())
    {
        if(context.getEnemyPlayer()->getHero()->getAbility()->allowCancel(context.getAttackerCard(),context))
        {
            context.getAttackerCard()->getEffects().clear(); 
            return ;
        }
    }
    if(context.getAttackerCard() == context.getCurrentCard())
    {
        if(context.getEnemyPlayer()->getHero()->getAbility()->allowCancel(context.getDefenderCard(),context))
        {
            context.getDefenderCard()->getEffects().clear(); 
            return ;
        }
    }
}

void SwapEffect::execute(GameContext& context , const vector<Character*>& targets)
{
    auto SecondTarget = context.getTargets(EffectTarget::EnemyHero) ;
    for(auto c1 : targets)
    {
        for(auto c2 : SecondTarget)
        {
            auto temp = c1->getPosition() ;
            c1->setPosition(c2->getPosition()) ;
            c2->setPosition(temp) ;
        }
    }
}

void MoveToAdjacentEffect::execute(
    GameContext& context,
    const vector<Character*>& targets)
{
    Character* hero = context.getCurrentPlayer()->getHero().get();

    for(auto c : targets)
    {
        if(context.getGame()->getPendingCombat()->selection.destination == -1)
        {
            context.getGame()->requestAction(
                make_unique<MoveAction>(
                    c,
                    hero,
                    MoveMode::Neighboor,
                    -1
                )
            );

            return;
        }

        int destination =
            context.getGame()->getPendingCombat()->selection.destination;

        if(context.getGame()->canMove(destination))
        {
            context.getGame()->move(c, destination);
        }
    }
}

void DeduceEffect::execute(GameContext& context , const vector<Character*>& targets)
{
    if(context.getCurrentCard() == context.getDefenderCard())
    {
        int temp = context.getAttackerCard()->getBoost() ;
        context.getAttackerCard()->setBoost(context.getAttackerCard()->getValue()) ;
        context.getAttackerCard()->setValue(temp) ;
        return ;
    }
    if(context.getCurrentCard() == context.getAttackerCard())
    {
        int temp = context.getDefenderCard()->getBoost() ;
        context.getDefenderCard()->setBoost(context.getDefenderCard()->getValue()) ;
        context.getDefenderCard()->setValue(temp) ;
        return ;
    }
}

void ReviveSister::execute(GameContext& context, const vector<Character*>& targets)
{
    for(auto sister : targets)
    {
        if(!sister->isAlive())
        {
            if(context.getGame()->getPendingCombat()->selection.destination == -1)
            {
                context.getGame()->requestAction(
                    make_unique<MoveAction>(
                        context.getAttacker(),
                        nullptr,
                        MoveMode::Zone,
                        -1
                    )
                );

                return;
            }

            int destination =
                context.getGame()->getPendingCombat()->selection.destination;

            if(context.getGame()->canMove(destination))
            {
                sister->heal(sister->getMaxhp());
                context.getGame()->move(sister, destination);
            }
        }
    }
}

void AmbushEffect::execute(GameContext& context ,  const vector<Character*>& targets)
{
    int randomindex = (rand())%(context.getEnemyPlayer()->getHero()->getDeck()->getHandSize());
    Card* randomcard = context.getEnemyPlayer()->getHero()->getDeck()->previewCard(randomindex);
    context.getCurrentCard()->setValue(context.getCurrentCard()->getValue() + randomcard->getBoost()) ;
    context.getEnemyPlayer()->getHero()->getDeck()->discardFromHand(randomindex);
}

void GainActionEffect::execute(GameContext& context ,  const vector<Character*>& targets)
{
    context.getGame()->addAction() ;
}

void FeedingFrenzyEffect::execute(GameContext& context ,  const vector<Character*>& targets)
{
    int count = 0 ; 
    for(auto sister : targets)
    {
        if(areInSameZone(context.getBoard() , context.getDefender() , sister))
        {
            count++ ;
        }
    }
    context.getAttackerCard()->setValue(context.getAttackerCard()->getValue() + count) ;
}

void PreyUponEffect::execute(GameContext& context , const vector<Character*>& targets)
{
    int count = 0 ;
    for(auto opponet : targets)
    {
        if(areAdjacent(context.getBoard() , context.getAttacker() , opponet))
        {
            opponet->takeDamage(1) ;
            count++ ;
        }
    }
    context.getAttacker()->heal(count) ;
}

void LookIntoMyEyesEffect::execute(GameContext& context , const vector<Character*>& targets)
{
    context.getDefenderCard()->setValue(context.getDefenderCard()->getValue() + context.getAttackerCard()->getBoost()) ;
}

void ThirstEffect::execute(GameContext& context , const vector<Character*>& targets)
{
    for(auto c : targets)
    {
        if(context.getGame()->getPendingCombat()->selection.destination == -1)
        {
            context.getGame()->requestAction(make_unique<MoveAction>(c, context.getDefender(),MoveMode::Neighboor ,-1)) ;
            return ;
        }
        if(context.getGame()->canMove(context.getGame()->getPendingCombat()->selection.destination))
        {
            context.getGame()->move(c , context.getGame()->getPendingCombat()->selection.destination) ;
        }
    }
}

void RaveningEffect::execute(GameContext& context , const vector<Character*>& targets)
{
    if(context.getGame()->getPendingCombat()->selection.character == nullptr ||
       context.getGame()->getPendingCombat()->selection.destination == -1 )
    {
        context.getGame()->requestAction(make_unique<RaveningAction>(*context.getGame())) ;
        return ;
    }
    if(context.getGame()->canMove(context.getGame()->getPendingCombat()->selection.destination))
    {
        context.getGame()->move(context.getGame()->getPendingCombat()->selection.character , 
        context.getGame()->getPendingCombat()->selection.destination) ;
    }
    int count = 0  ;
    for(auto sister : targets)
    {
        if(areAdjacent(context.getBoard() , context.getGame()->getPendingCombat()->selection.character , sister))
        {
            count++ ;
        }
    }
    context.getGame()->getPendingCombat()->selection.character->takeDamage(count) ;
}

void BeastFormEffect::execute(GameContext& context , const vector<Character*>& targets)
{
    if(context.getGame()->getPendingCombat()->selection.cards.empty())
    {
        context.getGame()->requestAction(make_unique<ChooseCardAction>(context.getCurrentPlayer() , 0 , 
        context.getCurrentPlayer()->getHero()->getDeck()->getHandSize())) ;
        return ;
    }
    auto indexes = context.getGame()->getPendingCombat()->selection.cards ;
    sort(indexes.rbegin() , indexes.rend()); 
    int count = 0 ; 
    for(auto index : indexes)
    {
        context.getCurrentPlayer()->getHero()->getDeck()->discardFromHand(index) ;
        count++ ;
    }
    context.getAttackerCard()->setValue(context.getAttackerCard()->getValue() + count) ;
}

void ShowHandEffect::execute(GameContext& context , const vector<Character*>& targets)
{
    if(context.getGame()->getPendingCombat()->selection.destination == -1)
    {
        context.getGame()->requestAction(make_unique<ShowCardAction>(context.getEnemyPlayer())) ;
        return ;
    }
    return ;
}