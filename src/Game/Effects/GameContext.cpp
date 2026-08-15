#include "Game/Effects/GameContext.hpp"
#include "Game/Game.hpp"
using namespace std ;

GameContext::GameContext(
Player* currentPlayer, Player* enemyPlayer, Character* attacker , Character* defender , Board* board , Card* attackerCard, 
Card* defenderCard , Game* game) :
    currentPlayer(currentPlayer) , enemyPlayer(enemyPlayer) , attacker(attacker) , defender(defender) , board(board) ,
    attackerCard(attackerCard) , defenderCard(defenderCard) , game(game) , Winner(nullptr) , selectedCharacter(nullptr) ,currentCard(nullptr)
{
}

Player* GameContext::getCurrentPlayer() const
{
    return currentPlayer ;
}

Player* GameContext::getEnemyPlayer() const
{
    return enemyPlayer ;
}

Character* GameContext::getAttacker() const
{
    return attacker ;
}

Character* GameContext::getDefender() const
{
    return defender ;
}

Board* GameContext::getBoard() const
{
    return board ;
}

Card* GameContext::getAttackerCard() const
{
    return attackerCard ;
}

Card* GameContext::getDefenderCard() const
{
    return defenderCard ;
}

Game* GameContext::getGame() const
{
    return game ;
}

void GameContext::setCurrentCard(Card* currentcard)
{
    currentCard = currentcard;
}

Card* GameContext::getCurrentCard() const
{
    return currentCard ;
}

vector<Character*> GameContext::getTargets(EffectTarget target)
{
    switch(target)
    {
        case EffectTarget::FriendlyHero:
        {
            return {currentPlayer->getHero().get()};
        }

        case EffectTarget::EnemyHero:
        {
            return {enemyPlayer->getHero().get()};
        }

        case EffectTarget::FriendlySidekicks:
        {
            vector<Character*> result;

            for(auto& sidekick : currentPlayer->getHero()->getSidekicks())
            {
                result.push_back(sidekick.get());
            }

            return result;
        }

        case EffectTarget::EenmySidekicks:
        {
            vector<Character*> result;

            for(auto& sidekick : enemyPlayer->getHero()->getSidekicks())
            {
                result.push_back(sidekick.get());
            }

            return result;
        }

        case EffectTarget::FriendlyCharacters:
        {
            return currentPlayer->getAllCharacters();
        }

        case EffectTarget::EnemyCharacters:
        {
            return enemyPlayer->getAllCharacters();
        }

        // -----------------------------------------
        // Card Owner
        // -----------------------------------------

        case EffectTarget::CardOwner:
        {
            if(currentCard == attackerCard)
            {
                return {currentPlayer->getHero().get()};
            }

            if(currentCard == defenderCard)
            {
                return {enemyPlayer->getHero().get()};
            }

            return {};
        }

        case EffectTarget::CardOwnerSidekicks:
        {
            if(currentCard == attackerCard)
            {
                vector<Character*> result;

                for(auto& sidekick : currentPlayer->getHero()->getSidekicks())
                {
                    result.push_back(sidekick.get());
                }

                return result;
            }

            if(currentCard == defenderCard)
            {
                vector<Character*> result;

                for(auto& sidekick : enemyPlayer->getHero()->getSidekicks())
                {
                    result.push_back(sidekick.get());
                }

                return result;
            }

            return {};
        }

        case EffectTarget::CardOwnerCharacters:
        {
            if(currentCard == attackerCard)
            {
                return currentPlayer->getAllCharacters();
            }

            if(currentCard == defenderCard)
            {
                return enemyPlayer->getAllCharacters();
            }

            return {};
        }

        // -----------------------------------------
        // Card Opponent
        // -----------------------------------------

        case EffectTarget::CardOpponent:
        {
            if(currentCard == attackerCard)
            {
                return {enemyPlayer->getHero().get()};
            }

            if(currentCard == defenderCard)
            {
                return {currentPlayer->getHero().get()};
            }

            return {};
        }

        case EffectTarget::CardOpponentSidekicks:
        {
            if(currentCard == attackerCard)
            {
                vector<Character*> result;

                for(auto& sidekick : enemyPlayer->getHero()->getSidekicks())
                {
                    result.push_back(sidekick.get());
                }

                return result;
            }

            if(currentCard == defenderCard)
            {
                vector<Character*> result;

                for(auto& sidekick : currentPlayer->getHero()->getSidekicks())
                {
                    result.push_back(sidekick.get());
                }

                return result;
            }

            return {};
        }

        case EffectTarget::CardOpponentCharacters:
        {
            if(currentCard == attackerCard)
            {
                return enemyPlayer->getAllCharacters();
            }

            if(currentCard == defenderCard)
            {
                return currentPlayer->getAllCharacters();
            }

            return {};
        }

        case EffectTarget::CardOpponentFighter:
        {
            if(currentCard == attackerCard)
            {
                return {defender};
            }

            if(currentCard == defenderCard)
            {
                return {attacker};
            }

            return {};
        }

        // -----------------------------------------
        // Combat
        // -----------------------------------------

        case EffectTarget::Attacker:
        {
            return {attacker};
        }

        case EffectTarget::Defender:
        {
            return {defender};
        }

        case EffectTarget::SelectedCharacter:
        {
            Character* selected =
                game->getPendingCombat()->selection.character;

            if(selected == nullptr)
                return {};

            return {selected};
        }
        default:
        {
            return {};
        }
    }
}

vector<Character*> GameContext::resolve(ConditionTarget target) const
{
    switch(target)
    {
        case ConditionTarget::FriendlyHero:
        {
            return {currentPlayer->getHero().get()};
        }

        case ConditionTarget::EnemyHero:
        {
            return {enemyPlayer->getHero().get()};
        }

        case ConditionTarget::FriendlySidekicks:
        {
            vector<Character*> result;

            for(auto& sidekick : currentPlayer->getHero()->getSidekicks())
            {
                result.push_back(sidekick.get());
            }

            return result;
        }

        case ConditionTarget::EnemySidekicks:
        {
            vector<Character*> result;

            for(auto& sidekick : enemyPlayer->getHero()->getSidekicks())
            {
                result.push_back(sidekick.get());
            }

            return result;
        }

        case ConditionTarget::FriendlyCharacters:
        {
            return currentPlayer->getAllCharacters();
        }

        case ConditionTarget::EnemyCharacters:
        {
            return enemyPlayer->getAllCharacters();
        }

        // -----------------------------------------
        // Card Owner
        // -----------------------------------------

        case ConditionTarget::CardOwner:
        {
            if(currentCard == attackerCard)
            {
                return {currentPlayer->getHero().get()};
            }

            if(currentCard == defenderCard)
            {
                return {enemyPlayer->getHero().get()};
            }

            return {};
        }

        case ConditionTarget::CardOwnerSidekicks:
        {
            if(currentCard == attackerCard)
            {
                vector<Character*> result;

                for(auto& sidekick : currentPlayer->getHero()->getSidekicks())
                {
                    result.push_back(sidekick.get());
                }

                return result;
            }

            if(currentCard == defenderCard)
            {
                vector<Character*> result;

                for(auto& sidekick : enemyPlayer->getHero()->getSidekicks())
                {
                    result.push_back(sidekick.get());
                }

                return result;
            }

            return {};
        }

        case ConditionTarget::CardOwnerCharacters:
        {
            if(currentCard == attackerCard)
            {
                return currentPlayer->getAllCharacters();
            }

            if(currentCard == defenderCard)
            {
                return enemyPlayer->getAllCharacters();
            }

            return {};
        }

        // -----------------------------------------
        // Card Opponent
        // -----------------------------------------

        case ConditionTarget::CardOpponent:
        {
            if(currentCard == attackerCard)
            {
                return {enemyPlayer->getHero().get()};
            }

            if(currentCard == defenderCard)
            {
                return {currentPlayer->getHero().get()};
            }

            return {};
        }

        case ConditionTarget::CardOpponentSidekicks:
        {
            if(currentCard == attackerCard)
            {
                vector<Character*> result;

                for(auto& sidekick : enemyPlayer->getHero()->getSidekicks())
                {
                    result.push_back(sidekick.get());
                }

                return result;
            }

            if(currentCard == defenderCard)
            {
                vector<Character*> result;

                for(auto& sidekick : currentPlayer->getHero()->getSidekicks())
                {
                    result.push_back(sidekick.get());
                }

                return result;
            }

            return {};
        }

        case ConditionTarget::CardOpponentCharacters:
        {
            if(currentCard == attackerCard)
            {
                return enemyPlayer->getAllCharacters();
            }

            if(currentCard == defenderCard)
            {
                return currentPlayer->getAllCharacters();
            }

            return {};
        }

        case ConditionTarget::CardOpponentFighter:
        {
            if(currentCard == attackerCard)
            {
                return {defender};
            }

            if(currentCard == defenderCard)
            {
                return {attacker};
            }

            return {};
        }

        // -----------------------------------------
        // Combat
        // -----------------------------------------

        case ConditionTarget::Attacker:
        {
            return {attacker};
        }

        case ConditionTarget::Defender:
        {
            return {defender};
        }

        default:
        {
            return {};
        }
    }
}

void GameContext::setWinner(Character* TheWinner)
{
    Winner = TheWinner ;
}

Character* GameContext::getWinner() const
{
    return Winner ;
}

void GameContext::setSelectedCharacter(Character* selected)
{
    selectedCharacter = selected ;
}

Character* GameContext::getSelectedCharacter() const
{
    return selectedCharacter ;
}

void GameContext::setSelectedCardsIndex(vector<int> indexes)
{
    seletedCardsIndex = indexes ;
}

vector<int> GameContext::getSelectedCardsIndex() const 
{
    return seletedCardsIndex ;
}