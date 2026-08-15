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

void DrawCardEffect::execute(
    GameContext& context,
    const vector<Character*>& targets)
{
    for(auto character : targets)
    {
        if(character == context.getCurrentPlayer()->getHero().get())
        {
            for(int i = 0; i < count; i++)
                context.getCurrentPlayer()
                    ->getHero()
                    ->getDeck()
                    ->drawCard();
        }
        else if(character == context.getEnemyPlayer()->getHero().get())
        {
            for(int i = 0; i < count; i++)
                context.getEnemyPlayer()
                    ->getHero()
                    ->getDeck()
                    ->drawCard();
        }
    }
}

MoveEffect::MoveEffect(int distance) : distance(distance)
{
}

void MoveEffect::execute(
    GameContext& context,
    const vector<Character*>& targets)
{
    for(auto c : targets)
    {
        auto& selection =
            context.getGame()->getPendingCombat()->selection;

        if(selection.destination == -1)
        {
            if(distance == -1)
            {
                context.getGame()->requestAction(
                    make_unique<MoveAction>(
                        c,
                        nullptr,
                        MoveMode::AnySpace,
                        -1
                    )
                );
            }
            else
            {
                context.getGame()->requestAction(
                    make_unique<MoveAction>(
                        c,
                        nullptr,
                        MoveMode::Range,
                        distance
                    )
                );
            }

            return;
        }

        if(context.getGame()->canMove(
            selection.destination))
        {
            context.getGame()->move(
                c,
                selection.destination
            );
        }
    }
}

DiscardCardEffect::DiscardCardEffect(int count) : count(count) 
{
}

void DiscardCardEffect::execute(
    GameContext& context,
    const vector<Character*>& targets)
{
    auto* game = context.getGame();
    auto& selection = game->getPendingCombat()->selection;

    if(selection.cards.empty())
    {
        game->requestAction(
            make_unique<ChooseCardAction>(
                context.getEnemyPlayer(),
                1,
                1,
                *game
            )
        );

        return;
    }

    int cardIndex = selection.cards[0];

    context.getEnemyPlayer()
        ->getHero()
        ->getDeck()
        ->discardFromHand(cardIndex);
}

void CancelEffectsEffect::execute(
    GameContext& context,
    const vector<Character*>& targets)
{
    if(context.getDefenderCard() == context.getCurrentCard())
    {
        if(context.getAttackerCard() == nullptr)
            return;

        if(context.getEnemyPlayer()
            ->getHero()
            ->getAbility()
            ->allowCancel(
                context.getAttackerCard(),
                context))
        {
            context.getAttackerCard()
                ->getEffects()
                .clear();
        }

        return;
    }

    if(context.getAttackerCard() == context.getCurrentCard())
    {
        if(context.getDefenderCard() == nullptr)
            return;

        if(context.getEnemyPlayer()
            ->getHero()
            ->getAbility()
            ->allowCancel(
                context.getDefenderCard(),
                context))
        {
            context.getDefenderCard()
                ->getEffects()
                .clear();
        }
    }
}

void SwapEffect::execute(
    GameContext& context,
    const vector<Character*>& targets)
{
    auto& selection = context.getGame()->getPendingCombat()->selection;

    if(selection.character == nullptr)
    {
        if(targets.empty() || targets.front() == nullptr)
            return;

        context.getGame()->requestAction(
            make_unique<ChooseCharacterAction>(
                SelectionMode::Other,
                targets.front()
            )
        );

        return;
    }

    Character* selectedCharacter = selection.character;

    for(Character* source : targets)
    {
        if(source == nullptr || selectedCharacter == nullptr)
            continue;

        int sourcePosition = source->getPosition();
        int selectedPosition = selectedCharacter->getPosition();

        source->setPosition(selectedPosition);
        selectedCharacter->setPosition(sourcePosition);
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

void DeduceEffect::execute(
    GameContext& context,
    const vector<Character*>& targets)
{
    if(context.getCurrentCard() == context.getDefenderCard())
    {
        if(context.getAttackerCard() == nullptr)
            return;

        context.getAttackerCard()->setValue(
            context.getAttackerCard()->getBoost()
        );

        return;
    }

    if(context.getCurrentCard() == context.getAttackerCard())
    {
        if(context.getDefenderCard() == nullptr)
            return;

        context.getDefenderCard()->setValue(
            context.getDefenderCard()->getBoost()
        );

        return;
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
    if(context.getEnemyPlayer()->getHero()->getDeck()->getHandSize() == 0)
    {
        return ; 
    }
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
        if(!sister->isAlive())
            continue;

        if(areInSameZone(
            context.getBoard(),
            context.getDefender(),
            sister))
        {
            count++;
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
        if(!sister->isAlive())
        {
            continue ;
        }
        if(areAdjacent(context.getBoard() , context.getGame()->getPendingCombat()->selection.character , sister))
        {
            count++ ;
        }
    }
    context.getGame()->getPendingCombat()->selection.character->takeDamage(count) ;
}

void BeastFormEffect::execute(
    GameContext& context,
    const vector<Character*>& targets)
{
    auto& selection =
        context.getGame()->getPendingCombat()->selection;

    // هنوز انتخاب کارت انجام نشده
    if(!selection.showHand)
    {
        context.getGame()->requestAction(
            make_unique<ChooseCardAction>(
                context.getCurrentPlayer(),
                0,
                context.getCurrentPlayer()
                    ->getHero()
                    ->getDeck()
                    ->getHandSize() , 
                *context.getGame() 
            )
        );

        return;
    }

    // انتخاب انجام شده
    // حتی اگر cards خالی باشد، یعنی بازیکن 0 کارت انتخاب کرده
    int count = static_cast<int>(selection.cards.size());

    if(count > 0)
    {
        auto deck =
            context.getCurrentPlayer()
                ->getHero()
                ->getDeck();

        vector<int> indexes = selection.cards;

        // از آخر به اول حذف می‌کنیم تا indexها به هم نریزند
        sort(indexes.rbegin(), indexes.rend());

        for(int index : indexes)
        {
            deck->discardFromHand(index);
        }
    }

    // به ازای هر کارت حذف‌شده +1
    context.getAttackerCard()->setValue(
        context.getAttackerCard()->getValue() + count
    );

    // برای اجرای بعدی پاکش کن
    selection.cards.clear();
    selection.showHand = false;
}

void ShowHandEffect::execute(
    GameContext& context,
    const vector<Character*>& targets)
{
    auto* game = context.getGame();
    auto& selection = game->getPendingCombat()->selection;

    if(!selection.showHand)
    {
        game->requestAction(
            make_unique<ShowCardAction>(
                context.getEnemyPlayer()
            )
        );

        return;
    }
}