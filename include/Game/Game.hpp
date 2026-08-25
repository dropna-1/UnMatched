#pragma once
#include <iostream>
#include <vector>
#include <deque>
#include <optional>
#include <random>
#include "Game/Board/board.hpp"
#include "Game/Characters/Hero.hpp"
#include "Game/Factory/HeroFactory.hpp"
#include "Game/Effects/GameContext.hpp"
#include "Game/Player/player.hpp"
#include "Game/Pending/Pending.hpp"
#include "Game/Common/Option.hpp"
#include "Game/Common/SaveData.hpp"

class Character;
class InvisibleMan;
class Card;
class Fog;

enum class CombatStage{
    DefenseImmediate,
    AttackImmediate,

    DefenseDuring,
    AttackDuring,

    DealDamage,

    DefenseAfter,
    AttackAfter,

    Discard,
    Finished
};

struct PendingSelection{
    Character* character = nullptr;
    Fog* fog = nullptr;
    std::vector<int> cards;
    int destination = -1;
    bool showHand = false;
    bool canFinish = false;
};

struct PendingCombat{
    AttackOption option;

    std::shared_ptr<Card> attackCard;
    std::shared_ptr<Card> defenseCard;

    GameContext context;
    CombatStage stage;
    PendingSelection selection;

    PendingCombat(AttackOption option, std::shared_ptr<Card> attackCard, 
        std::shared_ptr<Card> defenseCard, GameContext& context) : option(option), 
        attackCard(attackCard), defenseCard(defenseCard), context(context),
        stage(CombatStage::DefenseImmediate) {}
};

class Game {

    Board board;
    Player player1;
    Player player2;
    std::shared_ptr<Hero> dracula;
    std::shared_ptr<Hero> sherlock;
    std::shared_ptr<Hero> invisible;
    Player* currentPlayer = nullptr;
    Player* otherPlayer = nullptr;
    int actionsRemaining = 2;
    std::deque<unique_ptr<PendingAction>> pendingActions;
    std::unique_ptr<PendingCombat> pendingCombat;
    std::mt19937 rng{std::random_device{}()};

    bool canUseAbility = false;
    MatchScreenSave loadedMatchScreen;
    
public:

    Game();
    void setupGame();
    void changeTurn();
    void nextTurn();
    Player* checkWinner();
    Board& getBoard();
    std::shared_ptr<Hero>& getDracula();
    InvisibleMan* getInvisibleMan() const;
    std::unique_ptr<PendingCombat>& getPendingCombat();
    void clearPendingCombat();
    void setCanUseAbility(const bool& use);
    InvisibleMan* asInvisible(Character* c) const;
    /*-----------------------------------------------------------------*/
    void setPlayer1(const string& name, const int& age);
    void setPlayer2(const string& name, const int& age);
    Player* getCurrentPlayer();
    Player* getOtherPlayer();
    void setupPlayers();
    void choiceHero(Player& player, HeroType choice);
    std::vector<int> getSidekickPlacement(Character* character);
    std::vector<int> getPlacementSpaces(Character* character);
    std::vector<int> getFogPlacement(InvisibleMan* inv);
    const std::vector<std::shared_ptr<Card>>& showOtherHand();
    /*-----------------------------------------------------------------*/
    std::vector<int> getAvailableMoves(Character* character, const int& spacing);
    std::vector<int> getFogMoves(Fog* fog, const int& spacing, bool emptySpaceOnly = false);
    std::vector<int> getAllSpaces();
    bool canMove(int to) const;
    void move(Character* character, const int& pos);
    int boost(Character* self, const int& cardIndex);
    bool canManever();
    void performManeuver(Character* character, const int& pos);
    std::vector<int> getFreeSpacesNearby(Character* character);
    /*------------------------------------------------------------------*/
    void requestAction(std::unique_ptr<PendingAction> action);
    bool hasPendingAction() const;
    PendingAction* currentPendingAction();
    void completePendingAction();
    /*------------------------------------------------------------------*/
    void useAction();
    int getRemainingActions() const;
    void addAction();
    void resetAction();
    /*------------------------------------------------------------------*/
    std::vector<AttackOption> getAttackableTargets();
    std::vector<int> getPlayableAttackCard(Character* attacker);
    std::vector<int> getPlayableDefenseCard(Character* defender);
    bool canDefense(Character* character);
    bool canAttack();
    std::vector<Character*> getEnemiesNearby(Character* own);
    /*------------------------------------------------------------------*/
    void playScheme(Character* source, const int& schemeCardIndex);
    void continuePlayScheme();
    std::vector<int> getSchemeCards(Character* character);
    bool canPlayScheme();
    /*------------------------------------------------------------------*/
    int calculateDamage(Card* attack, Card* defense);
    void combat(AttackOption option, const int& attackCardIndex, 
        std::optional<int> defenseCardIndex);
    void continueCombat();
    /*------------------------------------------------------------------*/
    GameSave createSaveData(const MatchScreenSave& screenSave) const;
    bool SaveGame(int slot, const MatchScreenSave& screenSave) const;
    bool LoadGame(int slot);
    static std::shared_ptr<Deck> restoreDeck(const DeckSave& save, HeroType heroType);
    std::unique_ptr<PendingAction> restorePendingAction(const PendingSave& save);
    const MatchScreenSave& getLoadedMatchScreen() const;
    CharacterRef makeCharacterRef(Character* character) const;
    Character* resolveCharacterRef(const CharacterRef& ref);
    std::vector<PendingSave> createPendingSave() const;
    std::optional<PendingCombatSave> createPendingCombatSave() const;
    std::shared_ptr<Card> findCardById(const std::string& id, Character* c) const;
    std::unique_ptr<PendingCombat> restorePendingCombat(const PendingCombatSave& save);
};