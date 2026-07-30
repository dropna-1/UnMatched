#include "Screens/MainScreen.hpp"
#include "Screens/ScreenManager.hpp"
#include "Game/Game.hpp"


int main() {
    Game game;
    ScreenManager man(&game);
    man.ChangeScreen(std::make_unique<MenuScreen>(&man));
    man.Run();
    return 0;
}
