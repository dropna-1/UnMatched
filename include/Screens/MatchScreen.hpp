#pragma once
#include "IScreen.hpp"
#include "raylib.h"
#include <memory>
#include <map>
#include "BoardView.hpp"
#include "StatusView.hpp"
#include "HandView.hpp"
#include "ActionsView.hpp"

class ScreenManager;

class MatchScreen : public IScreen {
private:
    Texture2D background;
    Font font;
    Rectangle btnSave;
    Rectangle btnHome;
    BoardView board;
    StatusView status;
    HandView hand;
    ActionsView actions;
    Stage stage = Stage::SideKickPlacementP1;
    map<Stage, std::string> message;
    Character* selected = nullptr;

    void HandleSidekickPlacement(Game& game);
    void HandleCharacterSelect(Game& game);
    void HandleMove(Game& game);
    void FillMessage();

public:
    explicit MatchScreen(ScreenManager* man);
    ~MatchScreen();
    void Update() override;
    void Draw() override;
    void HandleInput() override;
};