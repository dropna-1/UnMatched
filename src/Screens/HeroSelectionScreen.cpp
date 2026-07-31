#include "Screens/HeroSelectionScreen.hpp"
#include "Screens/PlayerSetupScreen.hpp"
#include "Screens/ScreenManager.hpp"
#include "Screens/VSScreen.hpp"
#include "Game/Game.hpp"
#include "raygui.h"
#include <iostream>

HeroSelectionScreen::HeroSelectionScreen(ScreenManager* mgr) {
    this->manager = mgr;

    font = LoadFontEx("external/font/Griffy-Regular.ttf", 64, 0, 0);
    font2 = LoadFontEx("external/font/RubikDirt-Regular.ttf", 64, 0, 0);
    SetTextureFilter(font.texture, TEXTURE_FILTER_POINT);

    background = LoadTexture("external/images/selection.jpg");

    heroes = GetAllHeroes();

    for (auto& hero : heroes) {
        hero.texture = LoadTexture(hero.imagePath.c_str());
        if (hero.texture.id != 0) {
            SetTextureFilter(hero.texture, TEXTURE_FILTER_BILINEAR);
        }
    }

    btnNext    = { 890/2.0f - 380, 430, 350, 50 };
    btnConfirm = { 890/2.0f - 380, 430, 350, 50 };
    btnBack    = { 890/2.0f + 30,  430, 350, 50 };
}

HeroSelectionScreen::~HeroSelectionScreen() {
    UnloadFont(font);
    UnloadFont(font2);
    UnloadTexture(background);

    for (auto& hero : heroes) {
        if (hero.texture.id != 0) UnloadTexture(hero.texture);
    }
}

void HeroSelectionScreen::HandleInput() {}
void HeroSelectionScreen::Update() {}

void HeroSelectionScreen::DrawHeroCard(int index, Rectangle bounds) {
    const HeroInf& hero = heroes[index];

    bool isSelectedByCurrent = (selectedHero[currentPlayer] == index);
    bool isTakenByOther = (selectedHero[1 - currentPlayer] == index);
    bool available = !isTakenByOther;

    Color bg = available ? Color{35, 22, 50, 230} : Color{25, 25, 25, 180};
    if (isSelectedByCurrent) bg = Color{70, 35, 100, 255};

    DrawRectangleRounded(bounds, 0.05f, 10, bg);
    DrawRectangleRoundedLines(bounds, 0.1f, 10,
                              isSelectedByCurrent ? GOLD : (available ? GRAY : DARKGRAY));


    if (hero.texture.id != 0) {
        float imgSize = 170;
        Rectangle src = {0, 0, (float)hero.texture.width, (float)hero.texture.height};
        Rectangle dst = {
            bounds.x + (bounds.width - imgSize) / 2,
            bounds.y + 12,
            imgSize,
            imgSize
        };

        Color tint = available ? WHITE : Color{80, 80, 80, 180};
        DrawTexturePro(hero.texture, src, dst, {0, 0}, 0, tint);
    } else {
        DrawRectangle(bounds.x + 30, bounds.y + 15, 110, 110, DARKGRAY);
        DrawText("No Image", bounds.x + 55, bounds.y + 60, 18, GRAY);
    }

    Vector2 nameSize = MeasureTextEx(font2, hero.name.c_str(), 25, 1);
    DrawTextEx(font2, hero.name.c_str(),
               {bounds.x + (bounds.width - nameSize.x)/2, bounds.y + 190},
               25, 1, available ? GOLD : GRAY);


    Vector2 titleSize = MeasureTextEx(font2, hero.title.c_str(), 18, 1);
    DrawTextEx(font2, hero.title.c_str(),
               {bounds.x + (bounds.width - titleSize.x)/2, bounds.y + 220},
               18, 1, available ? LIGHTGRAY : DARKGRAY);


    if (available && CheckCollisionPointRec(GetMousePosition(), bounds) &&
        IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        selectedHero[currentPlayer] = index;
    }
}

void HeroSelectionScreen::Draw() {
    if (background.id != 0) {
        DrawTexturePro(background,
            {0, 0, (float)background.width, (float)background.height},
            {0, 0, 890, 500}, {0, 0}, 0, WHITE);
    }
    DrawRectangle(0, 0, 890, 500, {0, 0, 0, 70});

    GuiSetFont(font);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 30);

    const char* playerName = manager->GetGame()->getCurrentPlayer()->getName().c_str();
    if(currentPlayer == 1)
        playerName = manager->GetGame()->getOtherPlayer()->getName().c_str();
        
    const char* title = TextFormat("%s, choose your Hero", playerName);
    Vector2 tSize = MeasureTextEx(font, title, 50, 1);
    DrawTextEx(font, title, {(890 - tSize.x)/2.0f, 15}, 50, 1, GOLD);


    DrawTextEx(font, TextFormat("P1: %s", selectedHero[0] >= 0 ? heroes[selectedHero[0]].name.c_str() : "---"),
               {65, 75}, 40, 1, selectedHero[0] >= 0 ? GOLD : GRAY);
    DrawTextEx(font, TextFormat("P2: %s", selectedHero[1] >= 0 ? heroes[selectedHero[1]].name.c_str() : "---"),
                {890-305, 75}, 40, 1, selectedHero[1] >= 0 ? GOLD : GRAY);


    float cardW = 240;
    float cardH = 250;
    float startY = 140;
    float totalWidth = (3 * cardW) + (2 * 20);
    float startX = (890 - totalWidth) / 2.0f;

    for (int i = 0; i < 3; i++) {
        Rectangle card = {
            startX + i * (cardW + 20),
            startY,
            cardW,
            cardH
        };
        DrawHeroCard(i, card);
    }


    GuiSetStyle(BUTTON, BORDER_WIDTH, 3);
    GuiSetStyle(BUTTON, TEXT_SIZE, 22);

    if (currentPlayer == 0){
        if (selectedHero[0] >= 0){
            if (GuiButton(btnNext, "NEXT PLAYER")) {
                currentPlayer = 1;
                manager->GetGame()->choiceHero(
                    *manager->GetGame()->getCurrentPlayer(), heroes[selectedHero[0]].type
                );
            }
        }
    }
    else{
        if (selectedHero[1] >= 0){
            if (GuiButton(btnConfirm, "START GAME")) {
                manager->GetGame()->choiceHero(
                    *manager->GetGame()->getOtherPlayer(), heroes[selectedHero[1]].type
                );
                manager->GetGame()->setupGame();
                manager->ChangeScreen(std::make_unique<VSScreen>(manager));
            }
        }
    }

    if (GuiButton(btnBack, "BACK")) {
        manager->ChangeScreen(std::make_unique<PlayerSetupScreen>(manager));
    }
}