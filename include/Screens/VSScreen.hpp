#pragma once
#include "IScreen.hpp"
#include "Screens/HeroSelectionScreen.hpp"
#include "raylib.h"
#include <vector>

class VSScreen : public IScreen {
public:
    explicit VSScreen(ScreenManager* mgr);
    ~VSScreen() override;

    void HandleInput() override;
    void Update() override;
    void Draw() override;

private:
    Font font;
    Font font2;
    Texture2D background;
    std::vector<HeroInf> heroes;

    Rectangle btnStart;
    Rectangle btnBack;
};