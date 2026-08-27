#include "Screens/LoadScreen.hpp"
#include "Game/Common/SaveManager.hpp"
#include "Screens/ScreenManager.hpp"
#include "Screens/MatchScreen.hpp"
#include "Screens/MainScreen.hpp"
#include <string>
#include "raygui.h"

LoadScreen::LoadScreen(ScreenManager* man){
    this->manager = man;
    background = LoadTexture("external/images/load.jpg");
    font = LoadFontEx("external/font/Griffy-Regular.ttf", 64, 0, 0);

    btnBack = {
        float(GetScreenWidth() - 200)/2,
        float(GetScreenHeight() - 70),
        200,
        50
    };

    GuiSetFont(font);
}

LoadScreen::~LoadScreen() {
    UnloadTexture(background);
    UnloadFont(font);
}

void LoadScreen::Update() {}

void LoadScreen::Draw() {
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
    DrawRectangle(0, 0, 890, 500, {0, 0, 0, 70});
    
    bool canLoad = false;
    GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL, ColorToInt(BLACK));
    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt({140, 0, 190, 255}));

    float StartX = 110;
    for(int i = 1; i <= 5; i++){
        if(SaveManager::slotExists(i)){
            Rectangle Save = {StartX, 70, 118, 200};
            Rectangle DeleteSave = {StartX, 280, 118, 50};
            StartX += 138;
            std::string text = "Save " + std::to_string(i);
            if(GuiButton(Save, text.c_str()))
            {
                canLoad = manager->GetGame().LoadGame(i);
            }
            if(GuiButton(DeleteSave, "X")){
                SaveManager::deleteSlot(i);
            }
        }
        if(canLoad){
            manager->ChangeScreen(std::make_unique<MatchScreen>(manager));
            return;
        }
    }

    if(GuiButton(btnBack, "Back")){
        manager->ChangeScreen(std::make_unique<MenuScreen>(manager));
        return;
    }
}

void LoadScreen::HandleInput() {}