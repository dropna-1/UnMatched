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

    DrawTextEx(font, "Player 1", {120, 120}, 70, 1, RAYWHITE);

    GuiLabel({400, 120, 100, 40}, "Name:");
    if (GuiTextBox({510, 120, 280, 40}, name1, 12, editName1)) editName1 = !editName1;

    GuiLabel({400, 170, 100, 40}, "Age:");
    if (GuiTextBox({510, 170, 100, 40}, age1, 8, editAge1)) editAge1 = !editAge1;

    DrawTextEx(font, "Player 2", {120, 280}, 70, 1, RAYWHITE);

    GuiLabel({400, 280, 100, 40}, "Name:");
    if (GuiTextBox({510, 280, 280, 40}, name2, 12, editName2)) editName2 = !editName2;

    GuiLabel({400, 330, 100, 40}, "Age:");
    if (GuiTextBox({510, 330, 100, 40}, age2, 8, editAge2)) editAge2 = !editAge2;

    if(Error != nullptr){
        Vector2 tSize = MeasureTextEx(font, Error, 30, 1);
        DrawTextEx(font, Error, {(890 - tSize.x) / 2, 460}, 30, 1, GOLD);
    }

    GuiSetStyle(BUTTON, TEXT_SIZE, 24);
    GuiSetStyle(BUTTON, BORDER_WIDTH, 3);

    if (GuiButton(btnConfirm, "CONFIRM")) {
        Error = ValidateInput();
        if(Error != nullptr){
            return;
        }
        player1Name = name1;
        player2Name = name2;
        player1Age  = atoi(age1);
        player2Age  = atoi(age2);

        manager->GetGame().setPlayer1(player1Name, player1Age);
        manager->GetGame().setPlayer2(player2Name, player2Age);

        manager->GetGame().setupPlayers();

        manager->ChangeScreen(std::make_unique<HeroSelectionScreen>(manager));
        return;
    }

    if (GuiButton(btnBack, "BACK")) {
        manager->ChangeScreen(std::make_unique<MenuScreen>(manager));
    }
}

const char* PlayerSetupScreen::ValidateInput()
{
    if(strlen(name1) == 0)
        return "Player 1 name is empty";
    if(strlen(name2) == 0)
        return "Player 2 name is empty";

    if(strlen(age1) == 0 || strlen(age2) == 0)
        return "Age cannot be empty";

    for(int i = 0; age1[i] != '\0'; i++)
    {
        if(!isdigit(age1[i]))
            return "Age must contain only numbers";
    }
    for(int i = 0; age2[i] != '\0'; i++)
    {
        if(!isdigit(age2[i]))
            return "Age must contain only numbers";
    }

    int a1 = atoi(age1);
    int a2 = atoi(age2);

    if(a1 < 1 || a1 > 120)
        return "Player 1 age is invalid";
    if(a2 < 1 || a2 > 120)
        return "Player 2 age is invalid";

    return nullptr;
}