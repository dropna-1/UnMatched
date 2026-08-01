#include "Screens/PlayerSetupScreen.hpp"
#include "Screens/ScreenManager.hpp"
#include "Screens/MainScreen.hpp"
#include "Screens/HeroSelectionScreen.hpp"
#include "Game/Game.hpp"
#include <cstring>
#include <cstdlib>

PlayerSetupScreen::PlayerSetupScreen(ScreenManager* man) {
    this->manager = man;
    font = LoadFontEx("external/font/Griffy-Regular.ttf", 64, 0, 0);
    SetTextureFilter(font.texture, TEXTURE_FILTER_POINT);

    background = LoadTexture("external/images/player3.jpg");

    btnConfirm = {890/2.0f - 220, 400, 200, 55};
    btnBack = {890/2.0f + 20,  400, 200, 55};

    GuiSetFont(font);
}

PlayerSetupScreen::~PlayerSetupScreen() {
    UnloadFont(font);
    UnloadTexture(background);
}

void PlayerSetupScreen::HandleInput() {}

void PlayerSetupScreen::Update() {}

void PlayerSetupScreen::Draw() {
    if (background.id != 0) {
        DrawTexturePro(background,
            {0, 0, (float)background.width, (float)background.height},
            {0, 0, 890, 500}, {0, 0}, 0, WHITE);
    }
    DrawRectangle(0, 0, 890, 500, {0, 0, 0, 70});

    GuiSetFont(font);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 40);
    GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(SKYBLUE));
    GuiSetStyle(TEXTBOX, TEXT_COLOR_PRESSED, ColorToInt(WHITE));
    GuiSetStyle(TEXTBOX, TEXT_COLOR_FOCUSED, ColorToInt(GOLD));
    GuiSetStyle(TEXTBOX, BASE_COLOR_PRESSED, ColorToInt({40, 20, 60, 255}));

    // const char* title = "PLAYER SETUP";
    // Vector2 tSize = MeasureTextEx(font, title, 48, 1);
    // DrawTextEx(font, title, {(890 - tSize.x) / 2, 30}, 48, 1, GOLD);

    DrawTextEx(font, "Player 1", {120, 120}, 70, 1, RAYWHITE);

    GuiLabel({400, 120, 100, 40}, "Name:");
    if (GuiTextBox({510, 120, 280, 40}, name1, 32, editName1)) editName1 = !editName1;

    GuiLabel({400, 170, 100, 40}, "Age:");
    if (GuiTextBox({510, 170, 100, 40}, age1, 8, editAge1)) editAge1 = !editAge1;

    DrawTextEx(font, "Player 2", {120, 280}, 70, 1, RAYWHITE);

    GuiLabel({400, 280, 100, 40}, "Name:");
    if (GuiTextBox({510, 280, 280, 40}, name2, 32, editName2)) editName2 = !editName2;

    GuiLabel({400, 330, 100, 40}, "Age:");
    if (GuiTextBox({510, 330, 100, 40}, age2, 8, editAge2)) editAge2 = !editAge2;

    GuiSetStyle(BUTTON, TEXT_SIZE, 24);
    GuiSetStyle(BUTTON, BORDER_WIDTH, 3);

    if (GuiButton(btnConfirm, "CONFIRM")) {
        player1Name = name1;
        player2Name = name2;
        player1Age  = atoi(age1);
        player2Age  = atoi(age2);

        manager->GetGame().setPlayer1(player1Name, player1Age);
        manager->GetGame().setPlayer2(player2Name, player2Age);

        manager->GetGame().setupPlayers();

        manager->ChangeScreen(std::make_unique<HeroSelectionScreen>(manager));
        // TraceLog(LOG_INFO, "P1: %s (%d)  |  P2: %s (%d)", 
        //          player1Name.c_str(), player1Age, 
        //          player2Name.c_str(), player2Age);
    }

    if (GuiButton(btnBack, "BACK")) {
        manager->ChangeScreen(std::make_unique<MenuScreen>(manager));
    }
}