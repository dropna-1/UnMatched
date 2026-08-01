#pragma once
#include "IScreen.hpp"
#include "raylib.h"
#include <memory>

class ScreenManager;

class MatchScreen : public IScreen {
private:
    Texture2D background;
    Font font;
    Rectangle btnHome;
    // Rectangle btnLoad;
    // Rectangle btnExit;
public:
    explicit MatchScreen(ScreenManager* man);
    ~MatchScreen();
    void Update() override;
    void Draw() override;
    void HandleInput() override;
};