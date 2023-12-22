#pragma once
#include "Entity.h"
#include "Gamestate.h"
#include "ResourceManager.h"


//-----Render Header File-----//
//Add rendering functions.


//-----RENDERING FUNCTIONS-----//

void RenderTitleScreen(Gamestate* state); // Renders the main menu screen by drawing the textures in ResourceManager.h

void RenderGameplayScreen(Gamestate* state); // Renders the gameplay screen, and all the entities inside it.

void RenderScoresScreen(Gamestate* state); // Renders the score screen using titlescreen render as background, and then drawing the scoreboard on top. Also draws the name input window if new highscore has been achieved.

void RenderTestScreen(Gamestate* state); // Renders Testroom where collisions can be tested with the mouse. Draws out the shapes that have been loaded into the entity lists.

void RenderMenu(Gamestate* state); // Draws a menuscreen over everything, and displays different things depending on game mode. 

void Render(Gamestate* state); //Render is run in Main, and will cycle one of the Render methods above based on state. 