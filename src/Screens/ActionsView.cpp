#include "Screens/ActionsView.hpp"
#include "raygui.h"

void ActionsView::setGame(Game* game){
    this->game = game;
}

void ActionsView::Draw(Rectangle m) {
    // if (background.id != 0) {
    //     DrawTexturePro(
    //         background,
    //         (Rectangle){0, 0, (float)background.width, (float)background.height},
    //         (Rectangle){0, 0, 890, 500},
    //         (Vector2){0, 0},
    //         0.0f,
    //         WHITE
    //     );
    // } else {
    //     ClearBackground(BLUE);
    // }
    // DrawRectangle(0, 0, 890, 500, {0, 0, 0, 150});

    btnCombat = {
        m.x + m.width/13,
        m.y+(m.height - m.height/3)/2,
        m.width*3/13,
        m.height/3
    };

    btnManeuver = {
        m.x + m.width*5/13,
        m.y+(m.height - m.height/3)/2,
        m.width*3/13,
        m.height/3
    };

    btnScheme = {
        m.x + m.width*9/13,
        m.y+(m.height - m.height/3)/2,
        m.width*3/13,
        m.height/3
    };

    GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL, ColorToInt(WHITE));
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt({124,0,0,255}));
    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt(BLACK));

    if (GuiButton(btnCombat, "Combat")) {
        return;
    }

    if (GuiButton(btnManeuver, "Maneuver")) {
        return;
    }

    if (GuiButton(btnScheme, "Scheme")) {
        return;
    }

    // GuiSetStyle(DEFAULT, TEXT_SIZE, 40);
    // GuiSetStyle(BUTTON, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);

    // GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, ColorToInt({0, 0, 0, 255}));
    // GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL, ColorToInt(GOLD));
    // GuiSetStyle(BUTTON, BORDER_COLOR_FOCUSED, ColorToInt(RAYWHITE));

}