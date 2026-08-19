#pragma once
#include <memory>
#include "IScreen.hpp"
#include "Game/Game.hpp"

class Game;

class ScreenManager {
private:
    std::unique_ptr<IScreen> current_screen;
    std::unique_ptr<Game> game;
public:
    ScreenManager();
    ~ScreenManager();
    void ChangeScreen(std::unique_ptr<IScreen> newScreen);
    void Run();
    void BackToHome(const int& width, const int& height);
    Game& GetGame();
};