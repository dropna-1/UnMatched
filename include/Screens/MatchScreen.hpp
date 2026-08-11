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

class ScreenManager;
class Game;

class MatchScreen : public IScreen {
private:
    Game* game = nullptr;
    Texture2D background;
    Font font;
    Rectangle btnSave;
    Rectangle btnHome;
    BoardView board;
    StatusView status;
    HandView hand;
    ActionsView actions;
    TipView tip;

    Stage stage = Stage::SideKickPlacementP1;
    PendingStage pStage = PendingStage::None;
    Character* selected = nullptr;

    void HandleStage();
    void HandleSidekickPlacement();
    void HandleCharacterSelect();
    void HandleMove();
    void HandleAbility();
    void HandlePlaycard();
    void HandlePendingAction();

public:
    explicit MatchScreen(ScreenManager* man);
    ~MatchScreen();
    void Update() override;
    void Draw() override;
    void HandleInput() override;
};