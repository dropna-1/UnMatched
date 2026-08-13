#pragma once
#include "IScreen.hpp"
#include "raylib.h"
#include <memory>
#include <map>
#include "BoardView.hpp"
#include "StatusView.hpp"
#include "HandView.hpp"
#include "TipView.hpp"
#include "ActionsView.hpp"
#include "Game/Game.hpp"

class ScreenManager;

class MatchScreen : public IScreen {
private:
    Game* game = nullptr;
    Texture2D background;
    Font font;
    Rectangle btnSave;
    Rectangle btnHome;
    Rectangle btnSkip;
    BoardView board;
    StatusView status;
    HandView hand;
    ActionsView actions;
    TipView tip;

    Stage stage = Stage::SideKickPlacementP1;
    Character* selected = nullptr;

    int AttackCardIndex = -1;
    int DefenseCardIndex = -1;

    AttackOption option;

    void HandleStageInput();
    void HandleSidekickPlacement();
    void HandleCharacterSelect();
    void HandleMove();
    void HandleAbility();
    void HandlePlaySchemeCard();
    void HandlePlayCombatCard();

    void HandlePendingActionInput();
    void HandlePendingMove();
    void HandlePendingChooseCharacter();
    void HandlePendingChooseCard();

    void DrawSkip();

public:
    explicit MatchScreen(ScreenManager* man);
    ~MatchScreen();
    void Update() override;
    void Draw() override;
    void HandleInput() override;
};