#pragma once
#include "raylib.h"

//-----Constants Header File-----//

//This file contain all fixed values that are not expected to change during run-time.

//-----WINDOW VALUES-----//
constexpr int SCREENWIDTH = 640;
constexpr int SCREENHEIGHT = 640;
constexpr int SCREENWIDTH_TENTH = SCREENHEIGHT / 10; //These are used sometimes to place items in specific places based on screen size.
constexpr int SCREENHEIGHT_TENTH = SCREENHEIGHT / 10; //Mostly the test room

//-----Menu Screen
constexpr Rectangle MENUWINDOW = {
    (float)(SCREENWIDTH / 10) * 2,
    (float)(SCREENHEIGHT / 10) * 2,
    (float)(SCREENWIDTH / 10) * 6,
    (float)(SCREENHEIGHT / 10) * 6 }; // Creates a box in covering the center of screen, regardless of size. For menus.
constexpr Rectangle MENUWINDOW_FRAME = {
    (float)(SCREENWIDTH / 10) * 2 + 12,
    (float)(SCREENHEIGHT / 10) * 2 + 12,
    (float)(SCREENWIDTH / 10) * 6 - 24,
    (float)(SCREENHEIGHT / 10) * 6 - 24 }; // Frames the Menu Window

//-----Buttons
constexpr Vector2 BUTTON_POSITION = {
    (float)(SCREENWIDTH / 10) * 3,
    (float)(SCREENHEIGHT / 10) }; // Centers the button on screen, it's Y-position being defined in Render.
constexpr Vector2 BUTTON_SIZE = {
    (float)((SCREENWIDTH / 10) * 4),
    (float)((SCREENHEIGHT / 10)) }; // How big the button will be drawn.

//-----UI
constexpr Vector2 UI_HEART{ 5.f,0.5f }; // Placement of hearts
constexpr Vector2 UI_HEART_SPACER{ 32.f,0.f }; // Spacer between position of hearts, to keep it consistent.
constexpr Vector2 UI_TIMER{ (SCREENWIDTH_TENTH * 7), 0.f }; // Placement of the timer.
constexpr Vector2 UI_SCORE{ SCREENWIDTH_TENTH + 5.f, SCREENHEIGHT - 32.f }; // Placement of score (current)
constexpr Vector2 UI_HISCORE{ (SCREENWIDTH_TENTH * 5), SCREENHEIGHT - 32.f }; // Placement of hiscore

//----Hiscore screen (uses the same window and the menu, but with scores written on it, instead.
constexpr int HISCORE_NAMES_XPOSITION = SCREENWIDTH_TENTH * 3;
constexpr int HISCORE_SCORES_XPOSITION = SCREENHEIGHT_TENTH * 6;
constexpr int HISCORE_YAXIS_OFFSET = SCREENHEIGHT_TENTH * 3;
constexpr Rectangle HISCORE_NAMEINPUT_WINDOW = {
    SCREENWIDTH_TENTH * 3,
    SCREENHEIGHT_TENTH * 4,
    SCREENWIDTH_TENTH * 4,
    SCREENHEIGHT_TENTH * 2  }; // Creates a box for the rendering screen
constexpr Rectangle HISCORE_NAMEINPUT_LINES = {
    SCREENWIDTH_TENTH * 3 + 6,
    SCREENHEIGHT_TENTH * 4 + 6,
    SCREENWIDTH_TENTH * 4 - 12,
    SCREENHEIGHT_TENTH * 2 - 12  }; // Creates a box for the rendering screen

//-----LEVEL WALL LINESEGMENT-----//
//The linesegments that make up the arena are an offset by about the size of an entity sprite (32 pixels)
constexpr Vector2 LINESEGMENT_TOPLEFT = { 32.f, 32.f };
constexpr Vector2 LINESEGMENT_BOTTOMRIGHT = { SCREENWIDTH - 32.f, SCREENHEIGHT - 32.f }; 

//-----LEVEL VALUES-----//
constexpr int PLAY_TIMER = 3600; // Frames until GameOver, 60fps, so 3600 frames = 60 seconds.
constexpr int PLAY_LIVES = 3; // Default number of hearts.

//-----ENTITY VALUES-----//
constexpr float MAXSMOVESPEED = 5.0f;
constexpr Vector2 ENEMY_LURCH{ 1.f ,0 }; //a lurching speed that is applied to the enemy whenever it reaches a stand-still. 
constexpr int BOMB_TIMER = 306; //Frame count until a bomb dies/explodes.
constexpr int BOMB_COOLDOWN = 60; //Frame count until a bomb can be placed again.
constexpr int BOMB_FUSE = 270; //Frame count Bombs are set to if struck by a blast
constexpr float BOMB_DROPDISTANCE = 42.f; //How much to multiply the normal of the player velocity by to determine where to place the bomb
constexpr float BOMB_KICKFORCE = 4.f; //How much to multiply the normal of the player velocity by to determine where to place the bomb
constexpr int BLAST_TIMER = 30; //How many frames blasts are alive for.
constexpr float BLAST_SPAWNDISTANCE = 16.f; //How much to multiply the normal of the player velocity by to determine where to place the bomb

//-----HISCORE VALUES-----//
constexpr int MAX_CHARACTERS = 3;

//-----MATH VALUES-----//
constexpr float LERP_T = 0.05f; //For lerping functions related to movement.
constexpr float MIN_BOUNCEVELOCITY = 1.f; //Minimum length of bouncevector allowed, if lower than this, add more bounce.
constexpr float VELOCITYMULTIPLIER = 1.5f; //For accelerating things, like in cases of bouncing, kicking, pushing, etc.