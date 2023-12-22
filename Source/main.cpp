/*******************************************************************************************
*
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   To test examples, just press F6 and execute raylib_compile_execute script
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   Example originally created with raylib 1.0, last time updated with raylib 1.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2013-2022 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "Update.h"
#include "Render.h"
#include "ResourceManager.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{    
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = SCREENWIDTH;
    const int screenHeight = SCREENHEIGHT;

    Gamestate state;

    InitWindow(screenWidth, screenHeight, "Bombergirl (not bomberman, I swear!)");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    //--------------------------------------------------------------------------------------

    InitAudioDevice();

    LoadAssetsFromFile();
    LoadScore();
 

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        Update(&state);

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        Render(&state);

        // put render here

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    CloseAudioDevice();
    
    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}