#pragma once
#include "IScreen.hpp"
#include "raylib.h"
#include <memory>

class LoadScreen : public IScreen {
private:
    Texture2D background;
    Font font;
    Rectangle btnBack;
public:
    explicit LoadScreen(ScreenManager* man);
    ~LoadScreen();
    void Update() override;
    void Draw() override;
    void HandleInput() override;
};