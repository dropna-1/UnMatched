#pragma once

#include "Game/Characters/Hero.hpp"
#include "Game/Characters/SideKick.hpp"
#include "Game/Board/board.hpp"
#include "Game/Player/player.hpp"
#include "Game/Cards/Cards.hpp"

class Card ;
class Player;
class Game;

class GameContext
{
    private :
        Player* currentPlayer ;
        Player* enemyPlayer;
        Character* attacker;
        Character* defender;
        Board* board;
        Character* Winner ;
        Character* selectedCharacter ;
        Card* attackerCard ;
        Card* defenderCard ;
        Game* game ;
        Card* currentCard ;
        std::vector<int> seletedCardsIndex ;

    public :
        GameContext(Player* , Player*, Character* , Character* , Board* , Card* ,Card* , Game*);
        Player* getCurrentPlayer() const ;
        Player* getEnemyPlayer() const ;
        Character* getAttacker() const ;
        Character* getDefender() const ; 
        Board* getBoard() const ;
        Card* getAttackerCard() const ;
        Card* getDefenderCard() const ;
        std::vector<Character*> getTargets(EffectTarget target);
        void setWinner(Character*) ;
        Character* getWinner() const ;
        void setCurrentCard(Card*) ;
        Card* getCurrentCard() const ;
        void setSelectedCharacter(Character*) ;
        Character* getSelectedCharacter() const ;
        std::vector<Character*> resolve(ConditionTarget target) const;
        void setSelectedCardsIndex(std::vector<int>)  ;
        std::vector<int> getSelectedCardsIndex() const  ;
        Game* getGame() const ;
};