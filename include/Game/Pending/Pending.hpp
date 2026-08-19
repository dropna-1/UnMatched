#pragma once
#include <vector>
#include "Game/Characters/Character.hpp"
#include "Game/Common/Option.hpp"

class Game;
class Player;
class Fog;

class PendingAction {
protected:
    RequestType type;
public:
    virtual std::vector<int> getOption(Game& game) = 0;
    virtual void submit(Game& game, int choice) = 0;
    RequestType getType() const;
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
    Character* getCurrentCharacter() const;
    Character* getOtherCharacter() const;
    MoveMode getMode() const;
    int getRange() const;
};
/*-----------------------------------------------------------------*/
class RaveningAction : public PendingAction {
private:
    std::vector<Character*> allCharacters;
    Character* selected = nullptr;
    int stage = 0;
public:
    RaveningAction(Game& game);
    std::vector<int> getOption(Game& game) override;
    void submit(Game& game, int choice) override;
    Character* getSelected() const;
    int getStage() const;
    void restoreState(Character* c, const int& state);
};
/*-----------------------------------------------------------------*/
class ChooseCardAction : public PendingAction {
private:
    std::vector<int> selectedCards;
    Player* selected = nullptr;
    int minCards;
    int maxCards;
public:
    ChooseCardAction(Player* player, int min, int max, Game& game);
    std::vector<int> getOption(Game& game) override;
    void submit(Game& game, int choice) override;
    Player* getSelectedPlayer() const;
    int getMinCards() const;
    int getMaxCards() const;
    const std::vector<int>& getSelectedCards() const;
    void restoreState(const std::vector<int>& cards);
};
/*-----------------------------------------------------------------*/
class ShowCardAction : public PendingAction {
private:
    Player* selected = nullptr;
public:
    ShowCardAction(Player* player);
    std::vector<int> getOption(Game& game) override;
    void submit(Game& game, int choice) override;
    Player* getSelectedPlayer() const;
};
/*-----------------------------------------------------------------*/
class ChooseCharacterAction : public PendingAction {
private:
    SelectionMode mode;
    std::vector<Character*> characters;
    Character* pc = nullptr;
public:
    ChooseCharacterAction(SelectionMode mode, Character* c);
    std::vector<int> getOption(Game& game) override;
    void submit(Game& game, int choice) override;
    SelectionMode getMode() const;
    Character* getCharacter() const;
    void restoreState(Character* c, SelectionMode mod);
};
/*-----------------------------------------------------------------*/
class DeleteCardAction : public PendingAction {
private:
    Player* selected = nullptr;
public:
    DeleteCardAction(Game& game, Player* player);
    std::vector<int> getOption(Game& game) override;
    void submit(Game& game, int choice) override;
    Player* getSelectedPlayer() const;
    void restoreState(Player* s);
};
/*-----------------------------------------------------------------*/
class DraculaAction : public PendingAction {
    std::vector<Character*> neighboors;
public:
    DraculaAction();
    std::vector<int> getOption(Game& game) override;
    void submit(Game& game, int choice) override;
};
/*-----------------------------------------------------------------*/
class FogMoveAction : public PendingAction {
    Fog* selected = nullptr;
    int range = -1;
    int stage = 0;
public:
    FogMoveAction(const int& range);
    std::vector<int> getOption(Game& game) override;
    void submit(Game& game, int choice) override;
    Fog* getSelected() const;
    int getRange() const;
    int getStage() const;
    void restoreState(Fog& fog, const int& range, const int& stage);
};