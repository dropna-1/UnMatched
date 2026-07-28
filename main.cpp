#include "Screens/MainScreen.hpp"
#include "Screens/ScreenManager.hpp"


int main() {
    ScreenManager man;
    man.ChangeScreen(std::make_unique<MenuScreen>());
    man.Run();
    return 0;
}
