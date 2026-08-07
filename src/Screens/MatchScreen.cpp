#include "Screens/MatchScreen.hpp"
#include "Screens/ScreenManager.hpp"
#include "screens/MainScreen.hpp"
#include "Game/Game.hpp"
#include "Game/Cards/Deck.hpp"
#include "raygui.h"

MatchScreen::MatchScreen(ScreenManager* mgr) {
    this->manager = mgr;

    actions.setGame(&mgr->GetGame());

    int monitor = GetCurrentMonitor();
    int monitorWidth  = GetMonitorWidth(monitor);
    int monitorHeight = GetMonitorHeight(monitor);

    SetWindowSize(monitorWidth, monitorHeight);
    SetWindowPosition(0, 0);

    font = LoadFontEx("external/font/GermaniaOne-Regular.ttf", 64, 0, 0);

    float btnW = 160;
    float btnH = 50;
    btnHome = {
        0.0f,
        (float)GetScreenHeight() - btnH - 30,
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
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), {0, 0, 0, 255});

    GuiSetStyle(DEFAULT, TEXT_SIZE, 30);
    GuiSetFont(font);

    Game& game = manager->GetGame();

    float x = GetScreenWidth();
    float y = GetScreenHeight();
    float boardW = (x*4)/6;
    float boardH = (y*2)/3;
    Rectangle m = {x/6+10, 20, boardW-20, boardH};
    board.Draw(game.getBoard(), m, *game.getCurrentPlayer(), *game.getOtherPlayer());

    status.DrawPlayerPanel(*game.getCurrentPlayer(), 2, 2, x/6, boardH+18);
    status.DrawPlayerPanel(*game.getOtherPlayer(), (x*5)/6-2, 2, x/6, boardH+18);

    Rectangle h = {(x*2)/3, boardH+20, x/3, y/3-30-20};
    hand.Draw(*game.getCurrentPlayer()->getHero()->getDeck(), h);

    Rectangle b = {2, boardH+20, x/3, y/3-30-20-50};
    actions.Draw(b);

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