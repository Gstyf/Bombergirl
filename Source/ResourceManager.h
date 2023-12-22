#pragma once
#include "raylib.h"
#include <string>

//-----Resource Manager Header File-----//
//Add functions to load textures, sounds, etc. into the game, and to load data into and out of files.

//-----HISCORE-----//

inline std::string highScoreArrayNames[5]; //Array where the 3-letter names of score holders are held
inline int highScoreArrayNumbers[5]; //Array where the score of the holders are held
inline bool newHiscore = false; //when true, change behaviour of RenderScoreScreen and Update.
inline int newHiscoreSlot = 0; //Tracks which spot ont he hiscore to place a new hiscore
inline int inputCharacterCount = 0; //Limits the user to only 3 characters for names.
inline char inputName[3] = "";

inline extern void LoadScore(); //Load scores from score text file into Array;
inline extern void WriteScore(); //If GAME ends, check if score is higher than top 5, and ovewrite. Will need a sorting system, I guess...
inline extern int UpdateHiscores(int currentScore, std::string* names, int* hiscore); //Check if playerscore beat any existing highscore, and if so insert it to table and sort.
inline extern void ResetCharacterInput(); // Clears the characterInput, so the same name won't automatically show again for a subsequent hiscore.
inline extern void FactoryResetScore(); // Cleans the hiscore to original factory settings (in case someone got a really high score.

//-----RESOURCES-----//
//Just textures, really. The RayLib sounds didn't play nice. :(
inline Texture2D mainMenuTextures[4];
inline Texture2D characterSprites[10];
inline Texture2D bombSprites[10];
inline Texture2D explosionSprites[7];
inline Texture2D blastsSprites[4];
inline Texture2D enemySprites[4];
inline Texture2D boxSprites[2];
inline Texture2D heartSprite;

//-----ASSET LOADING-----//
void LoadAssetsFromFile(); //Loads all the assets in the Resource Folder using a lot of loops.