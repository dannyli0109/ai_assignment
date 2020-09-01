#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "raylib.h"
#include "Game.h"
int main(void)
{
    {
        Game game;
        int width = 1080;
        int height = 1080;

        InitWindow(width, height, "Assignment");

        game.Init();

        while (!WindowShouldClose())
        {
            game.Update();
            BeginDrawing();
            ClearBackground(DARKGREEN);
            game.Draw();
            EndDrawing();
        }
        game.Shutdown();
        CloseWindow();
    }
    _CrtDumpMemoryLeaks();
    return 0;
}