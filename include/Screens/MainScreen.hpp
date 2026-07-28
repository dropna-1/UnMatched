#pragma once
#include "IScreen.hpp"
#include "raylib.h"
#include <memory>

class ScreenManager;

class MenuScreen : public IScreen {
private:
    Texture2D background;
    Font font;
    Rectangle btnPlay;
    Rectangle btnLoad;
    Rectangle btnExit;
public:
    explicit MenuScreen(ScreenManager* man);
    ~MenuScreen();
    void Update() override;
    void Draw() override;
    void HandleInput() override;
};