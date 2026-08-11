#pragma once
#include <vector>
#include "Game/Characters/Character.hpp"
#include "Game/Common/Option.hpp"

class Game;
class Player;

enum class MoveMode
{
    Range,
    Zone,
    AnySpace,
    Neighboor
};

class PendingAction {
protected:
    RequestType type;
public:
    virtual std::vector<int> getOption(Game& game) = 0;
    virtual void submit(Game& game, int choice) = 0;
    RequestType getType() const;
    bool isFinished() const;
};
/*-----------------------------------------------------------------*/
class MoveAction : public PendingAction {
private:
    std::vector<int> spaces;
    Character* currentCharacter;
    Character* otherCharacter;
    MoveMode mode;
    int range = 0;
public:
    MoveAction(Character* current, Character* other, MoveMode mode, int value);
    std::vector<int> getOption(Game& game) override;
    void submit(Game& game, int choice) override;
};
/*-----------------------------------------------------------------*/
class RaveningAction : public PendingAction {
private:
    std::vector<Character*> allCharacters;
    std::vector<int> spaces;
    Character* selected = nullptr;
    int stage = 0;
public:
    RaveningAction(Game& game);
    std::vector<int> getOption(Game& game) override;
    void submit(Game& game, int choice) override;
};
/*-----------------------------------------------------------------*/
class ChooseCardAction : public PendingAction {
private:
    std::vector<int> selectedCards;
    Player* selected = nullptr;
    int minCards;
    int maxCards;
public:
    ChooseCardAction(Player* player, int min, int max);
    std::vector<int> getOption(Game& game) override;
    void submit(Game& game, int choice) override;
};
/*-----------------------------------------------------------------*/
class ShowCardAction : public PendingAction {
private:
    Player* selected = nullptr;
public:
    ShowCardAction(Player* player);
    std::vector<int> getOption(Game& game) override;
    void submit(Game& game, int choice) override;
};
/*-----------------------------------------------------------------*/
class DraculaAction : public PendingAction {
    std::vector<Character*> neighboors;
public:
    DraculaAction();
    std::vector<int> getOption(Game& game) override;
    void submit(Game& game, int choice) override;
};