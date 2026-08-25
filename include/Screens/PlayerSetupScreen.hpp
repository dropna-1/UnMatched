#pragma once
#include "IScreen.hpp"
#include "raylib.h"
#include "raygui.h"
#include <string>
#include <memory>

class ScreenManager;

class PlayerSetupScreen : public IScreen {
public:
    explicit PlayerSetupScreen(ScreenManager* man);
    ~PlayerSetupScreen() override;

    void HandleInput() override;
    void Update() override;
    void Draw() override;
    const char* ValidateInput();

    std::string player1Name;
    std::string player2Name;
    int player1Age = 18;
    int player2Age = 18;

private:
    Font font;
    Texture2D background;

    char name1[12] = "Player 1";
    char name2[12] = "Player 2";
    char age1[8]  = "18";
    char age2[8]  = "18";

    bool editName1 = false;
    bool editName2 = false;
    bool editAge1  = false;
    bool editAge2  = false;

    const char* Error = nullptr;

    Rectangle btnConfirm;
    Rectangle btnBack;
};