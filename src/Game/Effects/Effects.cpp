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
    int handSize =
        context.getEnemyPlayer()
            ->getHero()
            ->getDeck()
            ->getHandSize();

    if(handSize <= 0)
        return;

    int randomindex = rand() % handSize;

    Card* randomcard =
        context.getEnemyPlayer()
            ->getHero()
            ->getDeck()
            ->previewCard(randomindex);

    if(randomcard == nullptr)
        return;

    Card* currentCard = context.getCurrentCard();

    if(currentCard == nullptr)
        return;

    currentCard->setValue(
        currentCard->getValue() + randomcard->getBoost()
    );
    context.getEnemyPlayer()
        ->getHero()
        ->getDeck()
        ->discardFromHand(randomindex);
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
    auto* invisibleMan =
        context.getGame()->getInvisibleMan();

    if(invisibleMan == nullptr ||
       !invisibleMan->isOnFog() ||
       targets.empty() ||
       targets.front() == nullptr)
        return;

    Character* owner =
        targets.front();

    Player* opponentPlayer = nullptr;

    if(owner == context.getCurrentPlayer()->getHero().get())
    {
        opponentPlayer =
            context.getEnemyPlayer();
    }
    else if(owner == context.getEnemyPlayer()->getHero().get())
    {
        opponentPlayer =
            context.getCurrentPlayer();
    }

    if(opponentPlayer == nullptr)
        return;

    for(Character* character :
        opponentPlayer->getAllCharacters())
    {
        if(character == nullptr ||
           !character->isAlive())
            continue;

        for(const Fog& fog :
            invisibleMan->getFogs())
        {
            if(fog.isPlaced() &&
               fog.getPosition() ==
                   character->getPosition())
            {
                character->takeDamage(1);
                break;
            }
        }
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

    if(game == nullptr || game->getPendingCombat() == nullptr)
        return;

    auto& selection = game->getPendingCombat()->selection;

    if(selection.fog == nullptr ||
       selection.destination == -1)
    {
        game->requestAction(
            make_unique<FogMoveAction>(-1)
        );

        return;
    }

    selection.fog->setPosition(selection.destination);

    selection.fog = nullptr;
    selection.destination = -1;

    game->addAction();
}

void ReignOfTerrorEffect::execute(
    GameContext& context,
    const vector<Character*>& targets)
{
    auto* game = context.getGame();
    auto* invisibleMan = game->getInvisibleMan();

    if(invisibleMan == nullptr ||
       !invisibleMan->isOnFog())
        return;

    for(Character* character :
        game->getOtherPlayer()->getAllCharacters())
    {
        if(character == nullptr || !character->isAlive())
            continue;

        for(const Fog& fog : invisibleMan->getFogs())
        {
            if(fog.isPlaced() &&
               fog.getPosition() == character->getPosition())
            {
                character->takeDamage(2);
                break;
            }
        }
    }
}

void IntoThinAirEffect::execute(
    GameContext& context,
    const vector<Character*>& targets)
{
    auto* game = context.getGame();
    auto* invisibleMan = game->getInvisibleMan();

    if(invisibleMan == nullptr ||
       game->getPendingCombat() == nullptr)
        return;

    auto& selection = game->getPendingCombat()->selection;

    if(selection.effectStage == 0)
    {
        game->requestAction(
            make_unique<MoveAction>(
                invisibleMan,
                nullptr,
                MoveMode::Range,
                1
            )
        );

        selection.effectStage = 1;
        return;
    }

    if(selection.effectStage == 1)
    {
        selection.destination = -1;
        selection.effectStage = 2;

        game->requestAction(
            make_unique<FogMoveAction>(3)
        );

        return;
    }

    if(selection.effectStage == 2)
    {
        if(selection.fog == nullptr ||
           selection.destination == -1)
            return;

        selection.fog->setPosition(selection.destination);

        selection.fog = nullptr;
        selection.destination = -1;

        selection.effectStage = 0;
    }
}

void SlipAwayEffect::execute(
    GameContext& context,
    const vector<Character*>& targets)
{
    auto* game = context.getGame();
    auto* invisibleMan = game->getInvisibleMan();

    if(invisibleMan == nullptr ||
       game->getPendingCombat() == nullptr)
        return;

    auto& selection = game->getPendingCombat()->selection;

    if(selection.effectStage == 0)
    {
        selection.effectStage = 1;

        game->requestAction(
            make_unique<FogMoveAction>(
                3,
                true
            )
        );

        return;
    }

    if(selection.fog == nullptr ||
       selection.destination == -1)
        return;

    selection.fog->setPosition(selection.destination);

    invisibleMan->setPosition(selection.destination);

    selection.fog = nullptr;
    selection.destination = -1;

    selection.effectStage = 0;
}

void LurkingEffect::execute(
    GameContext& context,
    const vector<Character*>& targets)
{
    auto* game = context.getGame();
    auto* invisibleMan = game->getInvisibleMan();

    if(game == nullptr ||
       invisibleMan == nullptr ||
       game->getPendingCombat() == nullptr ||
       targets.empty() ||
       targets.front() == nullptr)
        return;

    Character* owner =
        targets.front();

    Player* ownerPlayer = nullptr;

    if(owner == context.getCurrentPlayer()->getHero().get())
    {
        ownerPlayer =
            context.getCurrentPlayer();
    }
    else if(owner == context.getEnemyPlayer()->getHero().get())
    {
        ownerPlayer =
            context.getEnemyPlayer();
    }

    if(ownerPlayer == nullptr ||
       ownerPlayer->getHero() == nullptr)
        return;

    auto& selection =
        game->getPendingCombat()->selection;

    // Draw 1 card from the CARD OWNER's deck.
    if(selection.effectStage == 0)
    {
        ownerPlayer
            ->getHero()
            ->getDeck()
            ->drawCard();

        selection.character = nullptr;
        selection.fog = nullptr;
        selection.destination = -1;

        selection.effectStage = 1;

        game->requestAction(
            make_unique<LurkingAction>()
        );

        return;
    }

    // Move Invisible Man to the selected fog space.
    if(selection.character == invisibleMan)
    {
        if(selection.destination == -1)
            return;

        invisibleMan->setPosition(
            selection.destination
        );

        selection.character = nullptr;
        selection.destination = -1;
        selection.effectStage = 0;

        return;
    }

    // Move the selected fog.
    if(selection.fog != nullptr)
    {
        if(selection.destination == -1)
            return;

        selection.fog->setPosition(
            selection.destination
        );

        selection.fog = nullptr;
        selection.destination = -1;
        selection.effectStage = 0;
    }
}

void StepLightlyEffect::execute(
    GameContext& context,
    const vector<Character*>& targets)
{
    auto* game = context.getGame();
    auto* invisibleMan = game->getInvisibleMan();

    if(invisibleMan == nullptr ||
       game->getPendingCombat() == nullptr)
        return;

    auto& selection = game->getPendingCombat()->selection;

    if(selection.effectStage == 0)
    {
        game->requestAction(
            make_unique<ChooseCharacterAction>(
                SelectionMode::Neighboors,
                invisibleMan
            )
        );

        selection.effectStage = 1;
        return;
    }

    if(selection.effectStage == 1)
    {
        if(selection.character == nullptr)
            return;

        const int damage =
            invisibleMan->isOnFog() ? 3 : 1;

        selection.character->takeDamage(damage);

        selection.character = nullptr;

        selection.effectStage = 2;

        game->requestAction(
            make_unique<FogMoveAction>(2)
        );

        return;
    }

    if(selection.effectStage == 2)
    {
        if(selection.fog == nullptr ||
           selection.destination == -1)
            return;

        selection.fog->setPosition(selection.destination);

        selection.fog = nullptr;
        selection.destination = -1;

        selection.effectStage = 0;
    }
}

void CodedNotesEffect::execute(
    GameContext& context,
    const vector<Character*>& targets)
{
    auto* game = context.getGame();

    if(game == nullptr ||
       game->getPendingCombat() == nullptr ||
       targets.empty() ||
       targets.front() == nullptr)
        return;

    Character* owner =
        targets.front();

    Player* ownerPlayer = nullptr;

    if(owner == context.getCurrentPlayer()->getHero().get())
    {
        ownerPlayer =
            context.getCurrentPlayer();
    }
    else if(owner == context.getEnemyPlayer()->getHero().get())
    {
        ownerPlayer =
            context.getEnemyPlayer();
    }

    if(ownerPlayer == nullptr ||
       ownerPlayer->getHero() == nullptr)
        return;

    auto* deck =
        ownerPlayer->getHero()->getDeck().get();

    auto& selection =
        game->getPendingCombat()->selection;

    if(selection.effectStage == 0)
    {
        for(int i = 0; i < 3; ++i)
            deck->drawCard();

        selection.cards.clear();
        selection.showHand = false;
        selection.canFinish = false;

        selection.effectStage = 1;

        game->requestAction(
            make_unique<ChooseCardAction>(
                ownerPlayer,
                2,
                2,
                *game
            )
        );

        return;
    }

    if(selection.effectStage == 1)
    {
        if(selection.cards.size() != 2)
            return;

        vector<shared_ptr<Card>> cardsToMove;

        const auto& hand =
            deck->getHand();

        for(int index : selection.cards)
        {
            if(index < 0 ||
               index >= static_cast<int>(hand.size()))
                return;

            cardsToMove.push_back(
                hand[index]
            );
        }

        deck->putCardsOnTop(
            cardsToMove
        );

        selection.cards.clear();
        selection.showHand = false;
        selection.canFinish = false;

        selection.effectStage = 0;
    }
}

void ConfoundEffect::execute(
    GameContext& context,
    const vector<Character*>& targets)
{
    auto* game = context.getGame();
    auto* invisibleMan = game->getInvisibleMan();

    if(game == nullptr ||
       invisibleMan == nullptr ||
       game->getPendingCombat() == nullptr ||
       targets.empty() ||
       targets.front() == nullptr)
        return;

    Character* owner =
        targets.front();

    Player* ownerPlayer = nullptr;
    Player* opponentPlayer = nullptr;

    if(owner == context.getCurrentPlayer()->getHero().get())
    {
        ownerPlayer =
            context.getCurrentPlayer();

        opponentPlayer =
            context.getEnemyPlayer();
    }
    else if(owner == context.getEnemyPlayer()->getHero().get())
    {
        ownerPlayer =
            context.getEnemyPlayer();

        opponentPlayer =
            context.getCurrentPlayer();
    }

    if(ownerPlayer == nullptr ||
       opponentPlayer == nullptr)
        return;

    auto& selection =
        game->getPendingCombat()->selection;

    if(selection.effectStage == 0)
    {
        selection.cards.clear();
        selection.showHand = false;
        selection.canFinish = false;

        selection.fog = nullptr;
        selection.destination = -1;
        selection.effectFogIndex = 0;

        selection.effectStage = 1;

        game->requestAction(
            make_unique<ChooseCardAction>(
                opponentPlayer,
                0,
                1,
                *game
            )
        );

        return;
    }

    if(selection.effectStage != 1)
        return;

    // Opponent chose to discard a card.
    if(!selection.cards.empty())
    {
        int index =
            selection.cards.front();

        opponentPlayer
            ->getHero()
            ->getDeck()
            ->discardFromHand(index);

        selection.cards.clear();
        selection.showHand = false;
        selection.canFinish = false;

        selection.effectFogIndex = 0;
        selection.fog = nullptr;
        selection.destination = -1;

        selection.effectStage = 0;

        return;
    }

    // Opponent did not discard.
    // Card owner may now move every placed fog token.
    while(
        selection.effectFogIndex <
        static_cast<int>(
            invisibleMan->getFogs().size()
        )
    )
    {
        Fog& fog =
            invisibleMan->getFogs()
                [selection.effectFogIndex];

        if(!fog.isPlaced())
        {
            ++selection.effectFogIndex;
            continue;
        }

        if(selection.fog != &fog ||
           selection.destination == -1)
        {
            game->requestAction(
                make_unique<FogMoveAction>(
                    -1,
                    false,
                    &fog
                )
            );

            return;
        }

        fog.setPosition(
            selection.destination
        );

        selection.fog = nullptr;
        selection.destination = -1;

        ++selection.effectFogIndex;
    }

    selection.effectStage = 0;
    selection.effectFogIndex = 0;
    selection.fog = nullptr;
    selection.destination = -1;
}

void CovertPreparationEffect::execute(
    GameContext& context,
    const vector<Character*>& targets)
{
    auto* game = context.getGame();
    auto* invisibleMan = game->getInvisibleMan();

    if(game == nullptr ||
       invisibleMan == nullptr ||
       game->getPendingCombat() == nullptr ||
       targets.empty() ||
       targets.front() == nullptr)
        return;

    Character* owner =
        targets.front();

    Player* ownerPlayer = nullptr;

    if(owner == context.getCurrentPlayer()->getHero().get())
    {
        ownerPlayer =
            context.getCurrentPlayer();
    }
    else if(owner == context.getEnemyPlayer()->getHero().get())
    {
        ownerPlayer =
            context.getEnemyPlayer();
    }

    if(ownerPlayer == nullptr ||
       ownerPlayer->getHero() == nullptr)
        return;

    auto& selection =
        game->getPendingCombat()->selection;

    //stage0
    if(selection.effectStage == 0)
    {
        ownerPlayer
            ->getHero()
            ->getDeck()
            ->drawCard();

        selection.fog = nullptr;
        selection.destination = -1;

        selection.effectStage = 1;

        game->requestAction(
            make_unique<FogMoveAction>(2)
        );

        return;
    }
    //stage 1
    if(selection.effectStage == 1)
    {
        if(selection.fog == nullptr ||
           selection.destination == -1)
            return;

        Fog* firstFog =
            selection.fog;

        firstFog->setPosition(
            selection.destination
        );

        selection.fog = nullptr;
        selection.destination = -1;

        selection.effectStage = 2;

        // The second fog must be different from the first.
        game->requestAction(
            make_unique<FogMoveAction>(
                2,
                false,
                nullptr,
                firstFog
            )
        );

        return;
    }
    //stage2
    if(selection.effectStage == 2)
    {
        if(selection.fog == nullptr ||
           selection.destination == -1)
            return;

        selection.fog->setPosition(
            selection.destination
        );

        selection.fog = nullptr;
        selection.destination = -1;

        selection.effectStage = 0;
    }
}