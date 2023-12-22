#pragma once
#include "raylib.h"
#include "Gamestate.h"
#include "GameLogic.h"

#include "ResourceManager.h"

//-----Update Header File-----//

//Add logic for what happens when the game updates


void TitleScreenUpdate(Gamestate* state); //How the game behaves in the main menu

void GameplayUpdate(Gamestate* state); //How the game behaves during gameplay

void ScorescreenUpdate(Gamestate* state); //How the scorescreen handles input

void TestScreenUpdate(Gamestate* state); //How the game behaves in test mode

void MenuWindowUpdate(Gamestate* state); //The menu behaviour, wherever it is opened.

void Update(Gamestate* state); //Update is run in Main, and will cycle one of the updates above. 