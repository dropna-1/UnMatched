#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <raylib.h>
using namespace std;

int main()
{
    InitWindow(800, 600, "Unmatched");

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        if (GuiButton({50, 50, 150, 40}, "Attack"))
        {
            // ...
        }

        EndDrawing();
    }

    CloseWindow();
}
