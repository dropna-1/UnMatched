#include "Screens/MainScreen.hpp"
#include "Screens/ScreenManager.hpp"
#include "Screens/PlayerSetupScreen.hpp"
#include "Screens/LoadScreen.hpp"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <cstdio>

MenuScreen::MenuScreen(ScreenManager* man){
    this->manager = man;
    background = LoadTexture("external/images/player.jpg");
    font = LoadFontEx("external/font/Griffy-Regular.ttf", 120, 0, 0);
    
    if (background.id == 0) {}

    int screenW = 890;
    int screenH = 500;
    btnPlay = {(float)screenW/2 - 200, 200, 400, 70};
    btnLoad = {(float)screenW/2 - 200, 290, 400, 70};
    btnExit = {(float)screenW/2 - 200, 380, 400, 70};

    GuiSetFont(font);
}

MenuScreen::~MenuScreen() {
    UnloadTexture(background);
    UnloadFont(font);
}

void MenuScreen::Update() {}

void MenuScreen::Draw() {
    if (background.id != 0) {
        DrawTexturePro(
            background,
            (Rectangle){0, 0, (float)background.width, (float)background.height},
            (Rectangle){0, 0, 890, 500},
            (Vector2){0, 0},
            0.0f,
            WHITE
        );
    } else {
        ClearBackground(BLUE);
    }
    DrawRectangle(0, 0, 890, 500, {0, 0, 0, 150});

    Vector2 titleSize = MeasureTextEx(font, "UNMATCHED", 120, 1.0f);
    DrawTextEx(font, "UNMATCHED", (Vector2){(890 - titleSize.x) / 2, 20}, 
    120, 1.0f, GOLD);

    Vector2 subTitleSize = MeasureTextEx(font, "TACTICAL DUEL", 40, 1.0f);
    DrawTextEx(font, "TACTICAL DUEL", (Vector2){(890 - subTitleSize.x) / 2, 140}, 
    40, 1.0f, GOLD);

    GuiSetStyle(DEFAULT, TEXT_SIZE, 40);
    GuiSetStyle(BUTTON, BORDER_WIDTH, 4);
    GuiSetStyle(BUTTON, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);

    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt({40, 20, 60, 255}));
    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, ColorToInt({0, 0, 0, 255}));
    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt(GOLD));
    GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL, ColorToInt(GOLD));
    GuiSetStyle(BUTTON, BORDER_COLOR_FOCUSED, ColorToInt(RAYWHITE));

    if (GuiButton(btnPlay, "START")) {
        manager->ChangeScreen(std::make_unique<PlayerSetupScreen>(manager));
        return;
    }
    if (GuiButton(btnLoad, "LOAD GAME")) {
        manager->ChangeScreen(std::make_unique<LoadScreen>(manager));
        return;
    }
    if (GuiButton(btnExit, "EXIT")) {
        CloseWindow();
    }
}

void MenuScreen::HandleInput() {}