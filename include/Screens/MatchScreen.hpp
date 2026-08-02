#pragma once
#include "IScreen.hpp"
#include "raylib.h"
#include <memory>
#include "BoardView.hpp"
#include "StatusView.hpp"

class ScreenManager;

class MatchScreen : public IScreen {
private:
    Texture2D background;
    Font font;
    Rectangle btnHome;
    BoardView board;
    StatusView status;

    // Rectangle btnLoad;
    // Rectangle btnExit;
public:
    explicit MatchScreen(ScreenManager* man);
    ~MatchScreen();
    void Update() override;
    void Draw() override;
    void HandleInput() override;
};