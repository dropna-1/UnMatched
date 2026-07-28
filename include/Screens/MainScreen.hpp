#pragma once
#include "IScreen.hpp"
#include "raylib.h"
#include <memory>

class MenuScreen : public IScreen {
private:
    Texture2D background;
    Font font;
    Rectangle btnPlay;
    Rectangle btnLoad;
    Rectangle btnExit;
public:
    MenuScreen();
    ~MenuScreen();
    void Update() override;
    void Draw() override;
    void HandleInput() override;
};