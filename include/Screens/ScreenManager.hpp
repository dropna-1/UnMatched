#pragma once
#include <memory>
#include "IScreen.hpp"

class ScreenManager {
private:
    std::unique_ptr<IScreen> current_screen;
public:
    ScreenManager();
    ~ScreenManager();
    void ChangeScreen(std::unique_ptr<IScreen> newScreen);
    void Run();
};