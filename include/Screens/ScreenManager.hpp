#pragma once
#include <memory>
#include "IScreen.hpp"

class Game;

class ScreenManager {
private:
    std::unique_ptr<IScreen> current_screen;
    Game* game = nullptr;
public:
    ScreenManager(Game* game);
    ~ScreenManager();
    void ChangeScreen(std::unique_ptr<IScreen> newScreen);
    void Run();
    Game* GetGame() const;
};