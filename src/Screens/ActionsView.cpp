#include "Screens/ActionsView.hpp"
#include "raygui.h"

void ActionsView::setGame(Game* game){
    this->game = game;
}

void ActionsView::Draw(Rectangle m) {

    float btnW = m.width*7/8;
    float btnH = m.height*3/8;
    btnManeuver = {
        m.x + btnW/14,
        m.y + btnH/3 - 5,
        btnW,
        btnH
    };

    btnCombat = {
        m.x + btnW/14,
        m.y + m.height/2,
        btnW/2 - 2,
        btnH
    };

    btnScheme = {
        m.x + btnW/2 + btnW/14 + 4,
        m.y + m.height/2,
        btnW/2 - 4,
        btnH
    };

    DrawRectangleRoundedLines(m, 0.1f, 1, WHITE);

    GuiSetStyle(DEFAULT, TEXT_SIZE, 40);
    GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL, ColorToInt({20, 20, 20, 255}));
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt({0, 0, 125, 255}));
    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, ColorToInt({BLACK}));
    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt(WHITE));

    if (GuiButton(btnManeuver, "Maneuver")) {
        return;
    }

    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt({90,0,0,255}));
    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt(WHITE));

    if (GuiButton(btnCombat, "Combat")) {
        return;
    }

    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt({140,140,0,255}));
    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt(WHITE));

    if (GuiButton(btnScheme, "Scheme")) {
        return;
    }

    // GuiSetStyle(BUTTON, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);

    // GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL, ColorToInt(GOLD));
    // GuiSetStyle(BUTTON, BORDER_COLOR_FOCUSED, ColorToInt(RAYWHITE));

}