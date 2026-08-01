#include "Screens/MatchScreen.hpp"
#include "Screens/ScreenManager.hpp"
#include "screens/MainScreen.hpp"
#include "Game/Game.hpp"
#include "raygui.h"

MatchScreen::MatchScreen(ScreenManager* mgr) {
    this->manager = mgr;

    int monitor = GetCurrentMonitor();
    int monitorWidth  = GetMonitorWidth(monitor);
    int monitorHeight = GetMonitorHeight(monitor);

    SetWindowSize(monitorWidth, monitorHeight);
    SetWindowPosition(0, 0);

    font = LoadFontEx("external/font/RubikDirt-Regular.ttf", 64, 0, 0);

    float btnW = 160;
    float btnH = 50;
    btnHome = {
        40.0f,
        (float)GetScreenHeight() - btnH - 30.0f,
        btnW,
        btnH
    };

}

MatchScreen::~MatchScreen() {
    UnloadFont(font);
    if (background.id != 0)
        UnloadTexture(background);
}

void MatchScreen::HandleInput() {}
void MatchScreen::Update() {}

void MatchScreen::Draw() {
    if (background.id != 0) {
        DrawTexturePro(background,
            {0, 0, (float)background.width, (float)background.height},
            {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()}, {0, 0}, 0, WHITE);
    }
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), {0, 0, 0, 70});

    GuiSetStyle(DEFAULT, TEXT_SIZE, 30);
    GuiSetFont(font);
    Game& game = manager->GetGame();

    if (GuiButton(btnHome, "HOME")) {
        SetWindowSize(890, 500);

    int monitor = GetCurrentMonitor();
    SetWindowPosition(
        (GetMonitorWidth(monitor) - 890) / 2,
        (GetMonitorHeight(monitor) - 500) / 2
);
        manager->ChangeScreen(std::make_unique<MenuScreen>(manager));
    }
}