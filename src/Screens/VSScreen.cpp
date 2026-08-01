#include "Screens/VSScreen.hpp"
#include "Screens/ScreenManager.hpp"
#include "Screens/MatchScreen.hpp"
#include "Game/Game.hpp"
#include "raygui.h"

VSScreen::VSScreen(ScreenManager* mgr) {
    this->manager = mgr;

    font = LoadFontEx("external/font/Griffy-Regular.ttf", 120, 0, 0);
    font2 = LoadFontEx("external/font/RubikDirt-Regular.ttf", 64, 0, 0);
    SetTextureFilter(font.texture, TEXTURE_FILTER_POINT);
    background = LoadTexture("external/images/vs.jpg");

    heroes = GetAllHeroes();

    for (auto& hero : heroes) {
        hero.texture = LoadTexture(hero.imagePath.c_str());
        if (hero.texture.id != 0) {
            SetTextureFilter(hero.texture, TEXTURE_FILTER_BILINEAR);
        }
    }

    btnStart = {890/2.0f - 160, 420, 320, 55};
    btnBack  = {80, 430, 140, 45};
}

VSScreen::~VSScreen() {
    UnloadFont(font);
    UnloadFont(font2);
    UnloadTexture(background);
    for (auto& hero : heroes) {
        if (hero.texture.id != 0) UnloadTexture(hero.texture);
    }
}

void VSScreen::HandleInput() {}
void VSScreen::Update() {}

void VSScreen::Draw() {
    if (background.id != 0) {
        DrawTexturePro(background,
            {0, 0, (float)background.width, (float)background.height},
            {0, 0, 890, 500}, {0, 0}, 0, WHITE);
    }
    DrawRectangle(0, 0, 890, 500, {0, 0, 0, 70});

    GuiSetFont(font);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 30);

    Game& game = manager->GetGame();

    int id1, id2;
    for(auto hero : heroes){
        if(hero.type == game.getCurrentPlayer()->getHero()->getHeroType())
            id1 = hero.id;
        else if(hero.type == game.getOtherPlayer()->getHero()->getHeroType())
            id2 = hero.id;
    }

    float leftX = 80;
    float cardY = 110;
    float imgSize = 200;

    DrawTextEx(font2, game.getCurrentPlayer()->getName().c_str(), {leftX, 70}, 24, 1, RAYWHITE);

    if (id1 >= 0 && id1 < (int)heroes.size()) {
        const HeroInf& h = heroes[id1];

        if (h.texture.id != 0) {
            Rectangle src = {0, 0, (float)h.texture.width, (float)h.texture.height};
            Rectangle dst = {leftX, cardY, imgSize, imgSize};
            DrawTexturePro(h.texture, src, dst, {0,0}, 0, WHITE);
        }

        DrawTextEx(font2, h.name.c_str(), {leftX, cardY + imgSize + 10}, 26, 1, GOLD);
        DrawTextEx(font2, h.title.c_str(), {leftX, cardY + imgSize + 42}, 16, 1, LIGHTGRAY);
    }

    float rightX = 890 - 80 - imgSize;

    DrawTextEx(font2, game.getOtherPlayer()->getName().c_str(), {rightX, 70}, 24, 1, RAYWHITE);

    if (id2 >= 0 && id2 < (int)heroes.size()) {
        const HeroInf& h = heroes[id2];

        if (h.texture.id != 0) {
            Rectangle src = {0, 0, (float)h.texture.width, (float)h.texture.height};
            Rectangle dst = {rightX, cardY, imgSize, imgSize};
            DrawTexturePro(h.texture, src, dst, {0,0}, 0, WHITE);
        }

        DrawTextEx(font2, h.name.c_str(), {rightX, cardY + imgSize + 10}, 26, 1, GOLD);
        DrawTextEx(font2, h.title.c_str(), {rightX, cardY + imgSize + 42}, 16, 1, LIGHTGRAY);
    }

    GuiSetStyle(BUTTON, BORDER_WIDTH, 3);
    GuiSetStyle(BUTTON, TEXT_SIZE, 24);

    if (GuiButton(btnStart, "START BATTLE")) {
        manager->GetGame().setupGame();
        TraceLog(LOG_INFO, "Battle starting: %s vs %s",
                 heroes[id1].name.c_str(), heroes[id2].name.c_str());
        manager->ChangeScreen(std::make_unique<MatchScreen>(manager));
    }

    if (GuiButton(btnBack, "BACK")) {
        manager->ChangeScreen(std::make_unique<HeroSelectionScreen>(manager));
    }
}