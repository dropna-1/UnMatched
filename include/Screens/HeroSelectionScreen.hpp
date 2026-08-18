// Game/Hero.hpp
#pragma once
#include <string>
#include <vector>
#include "raylib.h"
#include "IScreen.hpp"
#include "Game/Characters/Hero.hpp"
#include "Game/Enums/TypeEnums.hpp"

struct HeroInf {
    int id;
    std::string name;
    std::string title;
    std::string imagePath;
    HeroType type;
    Texture2D texture = {0};
};

inline std::vector<HeroInf> GetAllHeroes() {
    return {
        {0, "Dracula", "Lord of the Night", "external/images/dracula/test.png", HeroType::Dracula},
        {1, "Sherlock Holmes", "The Great Detective", "external/images/sherlock/holmsArt.png", HeroType::Sherlock},
        {2, "Invisible Man", "The Unseen Threat", "external/images/invisibleMan/invArt.png", HeroType::Invisibleman},
    };
}

class HeroSelectionScreen : public IScreen {
public:
    explicit HeroSelectionScreen(ScreenManager* mgr);
    ~HeroSelectionScreen() override;

    void HandleInput() override;
    void Update() override;
    void Draw() override;

private:
    Font font;
    Font font2;
    Texture2D background;
    std::vector<HeroInf> heroes;

    int currentPlayer = 0;
    int selectedHero[2] = {-1, -1};

    Rectangle btnNext;
    Rectangle btnConfirm;
    Rectangle btnBack;

    void DrawHeroCard(int index, Rectangle bounds);
};