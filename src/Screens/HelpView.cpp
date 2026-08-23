#include "Screens/HelpView.hpp"
#include "raygui.h"

HelpView::HelpView() {
    pages = {
        {
            "GENERAL",
            {
                "Unmatched is a tactical duel for two players.",
                "Each player controls a unique Hero and companions.",
                "Goal: reduce the enemy Hero HP to 0.",
                "",
                "At game start:",
                "- Heroes are placed on the board.",
                "- Sidekicks or Fog tokens are placed.",
                "- Each player draws a starting hand."
            }
        },
        {
            "TURN STRUCTURE",
            {
                "Each turn you have 2 actions.",
                "You can use them for:",
                "",
                "1) MANEUVER",
                "   Move one character, then draw 1 card.",
                "",
                "2) COMBAT",
                "   Attack an adjacent enemy with a card.",
                "   Defender may play a defense card or skip.",
                "",
                "3) SCHEME",
                "   Play a scheme card and resolve its effect.",
                "",
                "If your hand exceeds 7 cards, discard down to 7."
            }
        },
        {
            "MOVEMENT & COMBAT",
            {
                "MOVEMENT",
                "- Move along connected spaces.",
                "- Cannot stop on occupied spaces.",
                "- Secret passages may link distant spaces.",
                "",
                "COMBAT",
                "- Choose attacker and a valid target.",
                "- Play an Attack or Versatile card.",
                "- Defender may play Defense or Versatile.",
                "- Damage depends on card values and effects.",
                "- Effects can trigger Immediately, During, or After."
            }
        },
        {
            "CARDS & SIDEKICKS",
            {
                "CARD TYPES",
                "- Attack: used when you fight.",
                "- Defense: used when attacked.",
                "- Versatile: attack or defense.",
                "- Scheme: special action effect.",
                "",
                "SIDEKICKS",
                "- Most heroes have sidekicks.",
                "- Place them in valid spaces at start.",
                "- They can move, fight, and be defeated.",
                "- Only Hero HP decides victory."
            }
        },
        {
            "INVISIBLE MAN",
            {
                "SPECIAL RULES",
                "Invisible Man has no sidekicks.",
                "He uses Fog tokens instead.",
                "",
                "FOG PLACEMENT",
                "- Place Fog on almost any space.",
                "- Cannot place Fog on another Fog.",
                "",
                "PORTALS",
                "- While standing on Fog,",
                "  he can move to another Fog space.",
                "",
                "Fog is a marker, not a fighter."
            }
        },
        {
            "INTERFACE",
            {
                "UI TIPS",
                "- Highlighted spaces = legal targets/moves.",
                "- Click a character to select it.",
                "- Click a highlighted space to confirm.",
                "- Gray buttons are disabled (not legal now).",
                "- SKIP appears when defense can be passed.",
                "- MENU: save / quit / return to title.",
                "",
                "Good luck, and outplay your opponent!"
            }
        }
    };
}

void HelpView::Open() {
    open = true;
    pageIndex = 0;
}

void HelpView::Close() {
    open = false;
}

bool HelpView::IsOpen() const {
    return open;
}

void HelpView::Update() {
    if (!open) return;

    if (IsKeyPressed(KEY_ESCAPE)) {
        Close();
    }
}

void HelpView::Draw(Font font) {
    if (!open) return;

    const float sw = (float)GetScreenWidth();
    const float sh = (float)GetScreenHeight();

    DrawRectangle(0, 0, (int)sw, (int)sh, Fade(BLACK, 0.72f));

    float boxW = 720.0f;
    float boxH = 520.0f;
    Rectangle box = {
        (sw - boxW) * 0.5f,
        (sh - boxH) * 0.5f,
        boxW,
        boxH
    };

    DrawRectangleRounded(box, 0.04f, 10, Color{28, 20, 40, 250});
    DrawRectangleRoundedLines(box, 0.04f, 10, GOLD);

    const HelpPage& page = pages[pageIndex];
    Vector2 titleSize = MeasureTextEx(font, page.title, 34, 1);
    DrawTextEx(font, page.title,
               {box.x + (boxW - titleSize.x) * 0.5f, box.y + 22},
               34, 1, GOLD);

    const char* pageLabel = TextFormat("Page %d / %d", pageIndex + 1, (int)pages.size());
    DrawTextEx(font, pageLabel, {box.x + 28, box.y + 28}, 18, 1, GRAY);

    float y = box.y + 80;
    for (const char* line : page.lines) {
        DrawTextEx(font, line, {box.x + 40, y}, 22, 1, RAYWHITE);
        y += 30;
        if (y > box.y + boxH - 90) break;
    }

    GuiSetStyle(BUTTON, BORDER_WIDTH, 2);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);

    btnPrev  = { box.x + 30, box.y + boxH - 58, 120, 40 };
    btnClose = { box.x + boxW * 0.5f - 70, box.y + boxH - 58, 140, 40 };
    btnNext  = { box.x + boxW - 150, box.y + boxH - 58, 120, 40 };

    if (pageIndex == 0) GuiSetState(STATE_DISABLED);
    if (GuiButton(btnPrev, "PREV")) {
        if (pageIndex > 0) pageIndex--;
    }
    GuiSetState(STATE_NORMAL);

    if (GuiButton(btnClose, "CLOSE")) {
        Close();
    }

    if (pageIndex >= (int)pages.size() - 1) GuiSetState(STATE_DISABLED);
    if (GuiButton(btnNext, "NEXT")) {
        if (pageIndex < (int)pages.size() - 1) pageIndex++;
    }
    GuiSetState(STATE_NORMAL);
}