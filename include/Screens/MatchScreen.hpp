#pragma once
#include "IScreen.hpp"
#include "raylib.h"
#include <memory>
#include <map>
#include "BoardView.hpp"
#include "StatusView.hpp"
#include "HandView.hpp"
#include "ActionsView.hpp"

enum class Stage {
    SideKickPlacementP1,
    SideKickPlacementP2,
    SelectManeuverCharacter,
    ChoiceNode,
    SelectAttackCharacter,
    SelectAttackCard,
    SelectDefenseCard,
    SelectSchemeCard,
    None
};

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
    void HandleSidekickPlacement(Game& game);
    void FillMessage();
    map<Stage, std::string> message;

public:
    explicit MatchScreen(ScreenManager* man);
    ~MatchScreen();
    void Update() override;
    void Draw() override;
    void HandleInput() override;
};