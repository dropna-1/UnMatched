#pragma once
#include "raylib.h"
#include <memory>

class Game;

class ActionsView{
private:
    Texture2D background;
    Font font;
    Rectangle btnCombat;
    Rectangle btnManeuver;
    Rectangle btnScheme;
    Game* game;
public:
    // explicit ActionsView();
    void setGame(Game* game);
    // ~ActionsView();
    void Draw(Rectangle m);
};