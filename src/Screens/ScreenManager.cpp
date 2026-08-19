#include "Screens/ScreenManager.hpp"
#include "raylib.h"
#include <typeinfo>

ScreenManager::ScreenManager() {
    game = std::make_unique<Game>();
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(890, 500, "Unmatched");
    SetTargetFPS(60);
}

ScreenManager::~ScreenManager() {
    CloseWindow();
}

void ScreenManager::ChangeScreen(std::unique_ptr<IScreen> newScreen) {
    current_screen = std::move(newScreen);
}

void ScreenManager::Run() {
    while (!WindowShouldClose()) {

        if (current_screen) current_screen->HandleInput();
        if (current_screen) current_screen->Update();
        
        BeginDrawing();
        if (current_screen) current_screen->Draw();
        EndDrawing();
    }
}

void ScreenManager::BackToHome(const int& width, const int& height){
    game = std::make_unique<Game>();
    SetWindowSize(width, height);

    int monitor = GetCurrentMonitor();
    SetWindowPosition(
        (GetMonitorWidth(monitor) - width) / 2,
        (GetMonitorHeight(monitor) - height) / 2
    );
}

Game& ScreenManager::GetGame(){
    return *game;
}