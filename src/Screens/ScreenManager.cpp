#include "Screens/ScreenManager.hpp"
#include "raylib.h"
#include <typeinfo>

ScreenManager::ScreenManager() {
    game = std::make_unique<Game>();

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(890, 500, "Unmatched");
    SetTargetFPS(60);
}

ScreenManager::~ScreenManager()
{
    next_screen.reset();
    current_screen.reset();
    game.reset();

    CloseWindow();
}

void ScreenManager::ChangeScreen(std::unique_ptr<IScreen> newScreen) 
{
    if (!current_screen) {
        current_screen = std::move(newScreen);
        return;
    }
    next_screen = std::move(newScreen);
}

void ScreenManager::Run() {

    while (!WindowShouldClose()) {

        if (current_screen)
            current_screen->HandleInput();
        if (current_screen)
            current_screen->Update();
        BeginDrawing();
        if (current_screen)
            current_screen->Draw();
        EndDrawing();

        if (next_screen) 
        {
            if (reset_game_pending) 
            {
                game = std::make_unique<Game>();
                SetWindowSize(pending_width, pending_height);
                int monitor = GetCurrentMonitor();
                SetWindowPosition(
                    (GetMonitorWidth(monitor) - pending_width) / 2,
                    (GetMonitorHeight(monitor) - pending_height) / 2
                );

                reset_game_pending = false;
            }

            current_screen = std::move(next_screen);
        }
    }
}

void ScreenManager::BackToHome(const int& width, const int& height) {
    pending_width = width;
    pending_height = height;
    reset_game_pending = true;
}

Game& ScreenManager::GetGame() {
    return *game;
}