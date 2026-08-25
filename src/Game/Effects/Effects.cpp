#include "Game/Effects/Effects.hpp"
#include "Game/Effects/Conditions.hpp"
#include "Game/Ability/IAbility.hpp"
#include "Game/Pending/Pending.hpp"
#include "Game/Game.hpp"
#include "Game/Characters/InvisibleMan.hpp"
#include "Game/Cards/Deck.hpp"
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

void EmergeFromMistEffect::execute(
    GameContext& context,
    const vector<Character*>& targets)
{
    auto* invisibleMan = context.getGame()->getInvisibleMan();

    if(invisibleMan == nullptr)
        return;

    if(!invisibleMan->startedTurnOnFog())
        return;

    Card* currentCard = context.getCurrentCard();

    if(currentCard == nullptr)
        return;

    currentCard->setValue(5);
}

void DreamingOfRevengeEffect::execute(
    GameContext& context,
    const vector<Character*>& targets)
{
    auto* invisibleMan = context.getGame()->getInvisibleMan();

    if(invisibleMan == nullptr)
        return;

    if(!invisibleMan->isOnFog())
        return;

    auto* enemyPlayer = context.getEnemyPlayer();

    if(enemyPlayer == nullptr)
        return;

    for(Character* character : enemyPlayer->getAllCharacters())
    {
        if(character == nullptr)
            continue;

        if(!character->isAlive())
            continue;

        bool onFog = false;

        for(const Fog& fog : invisibleMan->getFogs())
        {
            if(!fog.isPlaced())
                continue;

            if(fog.getPosition() == character->getPosition())
            {
                onFog = true;
                break;
            }
        }

        if(onFog)
            character->takeDamage(1);
    }
}

void ImpossibleToSeeEffect::execute(
    GameContext& context,
    const vector<Character*>& targets)
{
    Card* currentCard = context.getCurrentCard();

    if(currentCard == nullptr)
        return;

    Card* opponentCard = nullptr;

    if(currentCard == context.getAttackerCard())
    {
        opponentCard = context.getDefenderCard();
    }
    else if(currentCard == context.getDefenderCard())
    {
        opponentCard = context.getAttackerCard();
    }

    if(opponentCard == nullptr)
        return;

    opponentCard->setValue(0);
    opponentCard->lockValue();
}

void RollingFogEffect::execute(
    GameContext& context,
    const vector<Character*>& targets)
{
    auto* game = context.getGame();
    auto& selection = game->getPendingCombat()->selection;

    // First execution:
    // ask the player to choose a fog and its destination.
    if(selection.fog == nullptr || selection.destination == -1)
    {
        game->requestAction(
            make_unique<FogMoveAction>(-1)
        );

        return;
    }

    // Resumed execution:
    // move the selected fog.
    selection.fog->setPosition(selection.destination);

    // Clear the temporary selection so it cannot affect later effects.
    selection.fog = nullptr;
    selection.destination = -1;

    // Gain 1 action.
    game->addAction();
}

void ReignOfTerrorEffect::execute(
    GameContext& context,
    const vector<Character*>& targets)
{
    auto* game = context.getGame();
    auto* invisibleMan = game->getInvisibleMan();

    // Reign of Terror only works while Invisible Man is on a fog.
    if(invisibleMan == nullptr || !invisibleMan->isOnFog())
        return;

    auto& selection = game->getPendingCombat()->selection;

    // First execution: ask the player to choose one opposing fighter.
    if(selection.character == nullptr)
    {
        game->requestAction(
            make_unique<ChooseCharacterAction>(
                SelectionMode::Other,
                nullptr
            )
        );

        return;
    }

    // Resumed execution: deal 2 damage to the selected fighter.
    selection.character->takeDamage(2);

    // Clear the temporary selection.
    selection.character = nullptr;
}

void IntoThinAirEffect::execute(
    GameContext& context,
    const vector<Character*>& targets)
{
    auto* game = context.getGame();
    auto* invisibleMan = game->getInvisibleMan();

    if(invisibleMan == nullptr)
        return;

    auto& selection = game->getPendingCombat()->selection;

    // ---------------------------------------------------------
    // Stage 0:
    // Move Invisible Man up to 1 space.
    // ---------------------------------------------------------
    if(stage == 0)
    {
        game->requestAction(
            make_unique<MoveAction>(
                invisibleMan,
                nullptr,
                MoveMode::Range,
                1
            )
        );

        stage = 1;
        return;
    }

    // ---------------------------------------------------------
    // Stage 1:
    // Invisible Man has moved.
    // Now the opponent moves a fog up to 3 spaces.
    // ---------------------------------------------------------
    if(stage == 1)
    {
        // The destination used by MoveAction belongs only
        // to the first movement. Clear it before FogMoveAction.
        selection.destination = -1;

        game->requestAction(
            make_unique<FogMoveAction>(3)
        );

        stage = 2;
        return;
    }

    // ---------------------------------------------------------
    // Stage 2:
    // Fog movement has completed.
    // Nothing else is required.
    // ---------------------------------------------------------
}

void SlipAwayEffect::execute(
    GameContext& context,
    const vector<Character*>& targets)
{
    auto* game = context.getGame();
    auto* invisibleMan = game->getInvisibleMan();

    if(invisibleMan == nullptr)
        return;

    auto& selection = game->getPendingCombat()->selection;

    // ---------------------------------------------------------
    // First execution:
    // Choose a fog and move it up to 3 spaces,
    // but only to a space without a fighter.
    // ---------------------------------------------------------
    if(waitingForFog)
    {
        game->requestAction(
            make_unique<FogMoveAction>(
                3,
                true
            )
        );

        waitingForFog = false;
        return;
    }

    // ---------------------------------------------------------
    // Resumed execution:
    // FogMoveAction has already selected the fog and destination.
    // Move the fog first, then place Invisible Man there.
    // ---------------------------------------------------------
    if(selection.fog == nullptr || selection.destination == -1)
        return;

    selection.fog->setPosition(selection.destination);

    invisibleMan->setPosition(selection.destination);

    // Clear temporary selection.
    selection.fog = nullptr;
    selection.destination = -1;
}

void LurkingEffect::execute(
    GameContext& context,
    const vector<Character*>& targets)
{
    auto* game = context.getGame();
    auto* invisibleMan = game->getInvisibleMan();

    if(invisibleMan == nullptr)
        return;

    auto& selection = game->getPendingCombat()->selection;

    // ---------------------------------------------------------
    // Stage 0:
    // Draw 1 card, then ask the player to choose the effect.
    // ---------------------------------------------------------
    if(stage == 0)
    {
        game->getCurrentPlayer()
            ->getHero()
            ->getDeck()
            ->drawCard();

        game->requestAction(
            make_unique<LurkingAction>()
        );

        stage = 1;
        return;
    }

    // ---------------------------------------------------------
    // Stage 1:
    // LurkingAction has completed.
    // Determine which effect was selected.
    // ---------------------------------------------------------

    // Option 1:
    // Move Invisible Man to the selected fog.
    if(selection.character == invisibleMan)
    {
        if(selection.destination != -1)
        {
            invisibleMan->setPosition(selection.destination);
        }

        selection.character = nullptr;
        selection.destination = -1;

        return;
    }

    // Option 2:
    // Move the selected fog.
    if(selection.fog != nullptr)
    {
        if(selection.destination != -1)
        {
            selection.fog->setPosition(selection.destination);
        }

        selection.fog = nullptr;
        selection.destination = -1;

        return;
    }
}

void StepLightlyEffect::execute(
    GameContext& context,
    const vector<Character*>& targets)
{
    auto* game = context.getGame();
    auto* invisibleMan = game->getInvisibleMan();

    if(invisibleMan == nullptr)
        return;

    auto& selection = game->getPendingCombat()->selection;

    // ---------------------------------------------------------
    // Stage 0:
    // Choose one adjacent fighter.
    // ---------------------------------------------------------
    if(stage == 0)
    {
        game->requestAction(
            make_unique<ChooseCharacterAction>(
                SelectionMode::Neighboors,
                invisibleMan
            )
        );

        stage = 1;
        return;
    }

    // ---------------------------------------------------------
    // Stage 1:
    // Deal damage to the selected fighter.
    // ---------------------------------------------------------
    if(stage == 1)
    {
        if(selection.character == nullptr)
            return;

        int damage = 1;

        if(invisibleMan->isOnFog())
            damage = 3;

        selection.character->takeDamage(damage);

        // The character selection is no longer needed.
        selection.character = nullptr;

        // Now the opponent moves a fog.
        game->requestAction(
            make_unique<FogMoveAction>(2)
        );

        stage = 2;
        return;
    }

    // ---------------------------------------------------------
    // Stage 2:
    // FogMoveAction has completed.
    // Nothing else is required.
    // ---------------------------------------------------------
}

void CodedNotesEffect::execute(
    GameContext& context,
    const vector<Character*>& targets)
{
    auto* game = context.getGame();
    auto* player = context.getCurrentPlayer();
    auto* deck = player->getHero()->getDeck().get();

    auto& selection = game->getPendingCombat()->selection;

    // ---------------------------------------------------------
    // Stage 0:
    // Draw 3 cards.
    // ---------------------------------------------------------
    if(stage == 0)
    {
        for(int i = 0; i < 3; ++i)
            deck->drawCard();

        stage = 1;

        game->requestAction(
            make_unique<ChooseCardAction>(
                player,
                1,
                1,
                *game
            )
        );

        return;
    }

    // ---------------------------------------------------------
    // Stage 1:
    // First card selected.
    // ---------------------------------------------------------
    if(stage == 1)
    {
        if(selection.cards.empty())
            return;

        selectedCards.push_back(selection.cards[0]);

        selection.cards.clear();
        selection.showHand = false;

        stage = 2;

        game->requestAction(
            make_unique<ChooseCardAction>(
                player,
                1,
                1,
                *game
            )
        );

        return;
    }

    // ---------------------------------------------------------
    // Stage 2:
    // Second card selected.
    // ---------------------------------------------------------
    if(stage == 2)
    {
        if(selection.cards.empty())
            return;

        selectedCards.push_back(selection.cards[0]);

        selection.cards.clear();
        selection.showHand = false;

        // Get the actual cards before removing them from the hand.
        vector<shared_ptr<Card>> cardsToMove;

        for(int index : selectedCards)
        {
            const auto& hand = deck->getHand();

            if(index >= 0 && index < static_cast<int>(hand.size()))
                cardsToMove.push_back(hand[index]);
        }

        if(cardsToMove.size() != 2)
            return;

        deck->putCardsOnTop(cardsToMove);

        selectedCards.clear();

        stage = 3;
        return;
    }
}